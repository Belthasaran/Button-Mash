#include "browsersourceserver.h"

#include "skinpath.h"

#include "skinparser.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QSettings>
#include <QTcpSocket>
#include <QEventLoop>
#include <QTimer>
#include <QUrlQuery>
#include <QWebSocket>

namespace {

const char kEnabledKey[] = "mirror/browserSourceEnabled";
const char kPortKey[] = "mirror/browserSourcePort";
const quint16 kDefaultPort = 28765;

QString colorToCss(const QColor &color)
{
    return QStringLiteral("rgb(%1,%2,%3)")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue());
}

bool parseHttpRequest(const QByteArray &request, QString *method, QString *path, QString *query)
{
    const int lineEnd = request.indexOf("\r\n");
    if (lineEnd < 0)
        return false;
    const QString line = QString::fromUtf8(request.left(lineEnd));
    const QStringList parts = line.split(QLatin1Char(' '));
    if (parts.size() < 2)
        return false;
    *method = parts.at(0);
    const QString target = parts.at(1);
    const int q = target.indexOf(QLatin1Char('?'));
    if (q >= 0) {
        *path = target.left(q);
        *query = target.mid(q + 1);
    } else {
        *path = target;
        *query = QString();
    }
    return true;
}

} // namespace

bool BrowserSourceServer::s_enabled = false;
quint16 BrowserSourceServer::s_port = kDefaultPort;

BrowserSourceServer::BrowserSourceServer(QObject *parent)
    : QObject(parent)
    , m_wsServer(new QWebSocketServer(QStringLiteral("ButtonMashBrowser"),
                                      QWebSocketServer::NonSecureMode,
                                      this))
    , m_mainSceneWidth(0)
    , m_mainSceneHeight(0)
    , m_listenPort(0)
    , m_running(false)
    , m_delai(0)
{
    connect(&m_tcp, &QTcpServer::newConnection, this, &BrowserSourceServer::onTcpNewConnection);
    connect(m_wsServer, &QWebSocketServer::newConnection, this, &BrowserSourceServer::onWsNewConnection);
}

BrowserSourceServer::~BrowserSourceServer()
{
    stop();
}

bool BrowserSourceServer::enabled()
{
    return s_enabled;
}

quint16 BrowserSourceServer::port()
{
    return s_port;
}

void BrowserSourceServer::loadSettings(QSettings &settings)
{
    s_enabled = settings.value(QLatin1String(kEnabledKey), false).toBool();
    s_port = quint16(settings.value(QLatin1String(kPortKey), kDefaultPort).toUInt());
    if (s_port < 1024)
        s_port = kDefaultPort;
}

void BrowserSourceServer::saveSettings(QSettings &settings)
{
    settings.setValue(QLatin1String(kEnabledKey), s_enabled);
    settings.setValue(QLatin1String(kPortKey), s_port);
}

void BrowserSourceServer::setEnabled(bool enabled)
{
    s_enabled = enabled;
}

void BrowserSourceServer::setPort(quint16 port)
{
    s_port = port;
}

QString BrowserSourceServer::urlForView(const QString &view, quint16 port)
{
    const QString path = (view == QLatin1String("stacked") || view == QLatin1String("piano"))
                             ? view
                             : QStringLiteral("main");
    return QStringLiteral("http://127.0.0.1:%1/%2").arg(port).arg(path);
}

void BrowserSourceServer::setDelai(unsigned int delai)
{
    m_delai = delai;
}

bool BrowserSourceServer::start(const RegularSkin &mainSkin, const PianoSkin &pianoSkin, quint16 port)
{
    stop();
    m_mainSkin = mainSkin;
    m_pianoSkin = pianoSkin;
    m_mainRoot = QFileInfo(mainSkin.file).absolutePath();
    m_pianoRoot = pianoSkin.file.isEmpty() ? QString() : QFileInfo(pianoSkin.file).absolutePath();
    m_pressed.clear();
    m_listenPort = port;

    const QPixmap bg(m_mainRoot + QLatin1Char('/') + mainSkin.background);
    m_mainSceneWidth = bg.width();
    m_mainSceneHeight = bg.height();
    if (m_mainSceneWidth <= 0 || m_mainSceneHeight <= 0) {
        m_mainSceneWidth = 400;
        m_mainSceneHeight = 200;
    }

    if (!m_tcp.listen(QHostAddress::LocalHost, port))
        return false;

    m_running = true;
    return true;
}

void BrowserSourceServer::stop()
{
    for (QWebSocket *client : m_clients)
        client->close();
    m_clients.clear();
    m_clientViews.clear();
    if (m_tcp.isListening())
        m_tcp.close();
    m_running = false;
    m_listenPort = 0;
}

QString BrowserSourceServer::buttonName(InputProvider::SNESButton button) const
{
    switch (button) {
    case InputProvider::A: return QStringLiteral("a");
    case InputProvider::B: return QStringLiteral("b");
    case InputProvider::X: return QStringLiteral("x");
    case InputProvider::Y: return QStringLiteral("y");
    case InputProvider::L: return QStringLiteral("l");
    case InputProvider::R: return QStringLiteral("r");
    case InputProvider::Start: return QStringLiteral("start");
    case InputProvider::Select: return QStringLiteral("select");
    case InputProvider::Up: return QStringLiteral("up");
    case InputProvider::Down: return QStringLiteral("down");
    case InputProvider::Left: return QStringLiteral("left");
    case InputProvider::Right: return QStringLiteral("right");
    }
    return QString();
}

QString BrowserSourceServer::parseViewFromPath(const QString &path) const
{
    if (path == QLatin1String("/stacked"))
        return QStringLiteral("stacked");
    if (path == QLatin1String("/piano"))
        return QStringLiteral("piano");
    return QStringLiteral("main");
}

QString BrowserSourceServer::parseViewFromQuery(const QString &query) const
{
    const QUrlQuery q(query);
    const QString view = q.queryItemValue(QStringLiteral("view"));
    if (view == QLatin1String("stacked") || view == QLatin1String("piano"))
        return view;
    return QStringLiteral("main");
}

QJsonObject BrowserSourceServer::buildMainSkinJson() const
{
    QJsonObject skin;
    skin.insert(QStringLiteral("width"), m_mainSceneWidth);
    skin.insert(QStringLiteral("height"), m_mainSceneHeight);
    skin.insert(QStringLiteral("background"),
                QStringLiteral("/assets/main/") + m_mainSkin.background);

    QJsonArray buttons;
    for (auto it = m_mainSkin.buttons.constBegin(); it != m_mainSkin.buttons.constEnd(); ++it) {
        const RegularButtonSkin &but = it.value();
        QJsonObject obj;
        obj.insert(QStringLiteral("name"), but.name);
        obj.insert(QStringLiteral("image"), QStringLiteral("/assets/main/") + but.image);
        obj.insert(QStringLiteral("x"), but.x);
        obj.insert(QStringLiteral("y"), but.y);
        obj.insert(QStringLiteral("width"), but.width);
        obj.insert(QStringLiteral("height"), but.height);
        buttons.append(obj);
    }
    skin.insert(QStringLiteral("buttons"), buttons);
    return skin;
}

QJsonObject BrowserSourceServer::buildPianoSkinJson() const
{
    if (m_pianoSkin.name.isEmpty())
        return QJsonObject();

    QJsonObject skin;
    skin.insert(QStringLiteral("width"), m_pianoSkin.width);
    skin.insert(QStringLiteral("height"), m_pianoSkin.height);
    skin.insert(QStringLiteral("bgcolor"), colorToCss(m_pianoSkin.bgcolor));

    QJsonArray buttons;
    for (auto it = m_pianoSkin.buttons.constBegin(); it != m_pianoSkin.buttons.constEnd(); ++it) {
        const PianoButton &but = it.value();
        QJsonObject obj;
        obj.insert(QStringLiteral("name"), but.name);
        obj.insert(QStringLiteral("x"), but.x);
        obj.insert(QStringLiteral("width"), but.width);
        obj.insert(QStringLiteral("color"), colorToCss(but.color));
        buttons.append(obj);
    }
    skin.insert(QStringLiteral("buttons"), buttons);

    QJsonObject tag;
    tag.insert(QStringLiteral("width"), m_pianoSkin.labelArea.width);
    tag.insert(QStringLiteral("height"), m_pianoSkin.labelArea.height);
    tag.insert(QStringLiteral("bgcolor"), colorToCss(m_pianoSkin.labelArea.bgcolor));
    tag.insert(QStringLiteral("fontName"), m_pianoSkin.labelArea.fontName);
    tag.insert(QStringLiteral("fontBold"), m_pianoSkin.labelArea.fontBold);
    tag.insert(QStringLiteral("fontSize"), m_pianoSkin.labelArea.fontSize);
    skin.insert(QStringLiteral("tag"), tag);
    return skin;
}

QByteArray BrowserSourceServer::buildInitMessage(const QString &view) const
{
    QJsonObject root;
    root.insert(QStringLiteral("type"), QStringLiteral("init"));
    root.insert(QStringLiteral("view"), view);
    root.insert(QStringLiteral("mainSkin"), buildMainSkinJson());

    const QJsonObject piano = buildPianoSkinJson();
    if (!piano.isEmpty())
        root.insert(QStringLiteral("pianoSkin"), piano);

    QJsonArray pressed;
    for (const QString &name : m_pressed)
        pressed.append(name);
    root.insert(QStringLiteral("pressed"), pressed);

    return QJsonDocument(root).toJson(QJsonDocument::Compact);
}

void BrowserSourceServer::sendInit(QWebSocket *socket, const QString &view)
{
    if (socket == nullptr)
        return;
    socket->sendTextMessage(QString::fromUtf8(buildInitMessage(view)));
}

void BrowserSourceServer::broadcastInput(const QList<InputChange> &changes)
{
    if (changes.isEmpty())
        return;

    QJsonArray arr;
    for (const InputChange &change : changes) {
        QJsonObject obj;
        obj.insert(QStringLiteral("button"), change.button);
        obj.insert(QStringLiteral("pressed"), change.pressed);
        arr.append(obj);
    }

    QJsonObject root;
    root.insert(QStringLiteral("type"), QStringLiteral("input"));
    root.insert(QStringLiteral("ts"), QDateTime::currentMSecsSinceEpoch());
    root.insert(QStringLiteral("changes"), arr);

    const QString payload = QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
    for (QWebSocket *client : m_clients)
        client->sendTextMessage(payload);
}

void BrowserSourceServer::dispatchButton(InputProvider::SNESButton button, bool pressed)
{
    const QString name = buttonName(button);
    if (name.isEmpty())
        return;

    const bool wasPressed = m_pressed.contains(name);
    if (pressed) {
        if (!wasPressed)
            m_pressed.insert(name);
    } else {
        m_pressed.remove(name);
    }

    if (wasPressed == pressed)
        return;

    InputChange change;
    change.button = name;
    change.pressed = pressed;
    broadcastInput({change});
}

void BrowserSourceServer::onButtonPressed(InputProvider::SNESButton button)
{
    if (m_delai == 0) {
        dispatchButton(button, true);
        return;
    }
    QTimer::singleShot(static_cast<int>(m_delai), this, [this, button]() {
        dispatchButton(button, true);
    });
}

void BrowserSourceServer::onButtonReleased(InputProvider::SNESButton button)
{
    if (m_delai == 0) {
        dispatchButton(button, false);
        return;
    }
    QTimer::singleShot(static_cast<int>(m_delai), this, [this, button]() {
        dispatchButton(button, false);
    });
}

void BrowserSourceServer::onTcpNewConnection()
{
    while (QTcpSocket *socket = m_tcp.nextPendingConnection()) {
        auto processSocket = [this, socket]() {
            const QByteArray peeked = socket->peek(16384);
            if (!peeked.contains("\r\n\r\n"))
                return;

            QString method;
            QString path;
            QString query;
            if (!parseHttpRequest(peeked, &method, &path, &query)) {
                socket->disconnectFromHost();
                return;
            }

            if (method == QLatin1String("GET") && path == QLatin1String("/ws")) {
                const QString view = parseViewFromQuery(query);
                socket->disconnect(this);
                upgradeWebSocket(socket, peeked, view);
                return;
            }

            const QByteArray request = socket->read(peeked.size());
            if (method != QLatin1String("GET")) {
                const QByteArray response =
                    "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
                socket->write(response);
                socket->disconnectFromHost();
                return;
            }
            serveHttp(socket, request, path);
        };

        connect(socket, &QTcpSocket::readyRead, this, processSocket);
        if (socket->bytesAvailable() > 0)
            processSocket();
    }
}

void BrowserSourceServer::upgradeWebSocket(QTcpSocket *socket, const QByteArray &request, const QString &view)
{
    Q_UNUSED(request);
    m_pendingWsView = view;
    m_wsServer->handleConnection(socket);
}

void BrowserSourceServer::onWsNewConnection()
{
    while (QWebSocket *ws = m_wsServer->nextPendingConnection()) {
        if (m_clients.contains(ws))
            continue;
        m_clients.append(ws);
        const QString view = m_pendingWsView.isEmpty() ? QStringLiteral("main") : m_pendingWsView;
        m_pendingWsView.clear();
        m_clientViews.insert(ws, view);
        connect(ws, &QWebSocket::disconnected, this, &BrowserSourceServer::onWsDisconnected);
        sendInit(ws, view);
    }
}

void BrowserSourceServer::onWsDisconnected()
{
    QWebSocket *ws = qobject_cast<QWebSocket *>(sender());
    if (ws == nullptr)
        return;
    m_clients.removeAll(ws);
    m_clientViews.remove(ws);
    ws->deleteLater();
}

QString BrowserSourceServer::mimeTypeForPath(const QString &path) const
{
    if (path.endsWith(QLatin1String(".html"), Qt::CaseInsensitive))
        return QStringLiteral("text/html; charset=utf-8");
    if (path.endsWith(QLatin1String(".js"), Qt::CaseInsensitive))
        return QStringLiteral("application/javascript; charset=utf-8");
    if (path.endsWith(QLatin1String(".css"), Qt::CaseInsensitive))
        return QStringLiteral("text/css; charset=utf-8");
    if (path.endsWith(QLatin1String(".png"), Qt::CaseInsensitive))
        return QStringLiteral("image/png");
    if (path.endsWith(QLatin1String(".gif"), Qt::CaseInsensitive))
        return QStringLiteral("image/gif");
    if (path.endsWith(QLatin1String(".jpg"), Qt::CaseInsensitive)
        || path.endsWith(QLatin1String(".jpeg"), Qt::CaseInsensitive))
        return QStringLiteral("image/jpeg");
    return QStringLiteral("application/octet-stream");
}

QByteArray BrowserSourceServer::readResource(const QString &resourcePath) const
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly))
        return QByteArray();
    return file.readAll();
}

QByteArray BrowserSourceServer::readSkinAsset(const QString &prefix, const QString &relPath, bool *ok) const
{
    QString root;
    if (prefix == QLatin1String("main"))
        root = m_mainRoot;
    else if (prefix == QLatin1String("piano"))
        root = m_pianoRoot;
    else {
        if (ok)
            *ok = false;
        return QByteArray();
    }

    if (root.isEmpty()) {
        if (ok)
            *ok = false;
        return QByteArray();
    }

    const QString clean = QUrl::fromPercentEncoding(relPath.toUtf8());
    QString err;
    const QString fullPath = SkinPath::resolveSkinRelativePath(root, clean, &err, true);
    if (fullPath.isEmpty()) {
        if (ok)
            *ok = false;
        return QByteArray();
    }

    QFile file(fullPath);
    if (!file.open(QIODevice::ReadOnly)) {
        if (ok)
            *ok = false;
        return QByteArray();
    }
    if (ok)
        *ok = true;
    return file.readAll();
}

void BrowserSourceServer::serveHttp(QTcpSocket *socket, const QByteArray &request, const QString &path)
{
    Q_UNUSED(request);

    QByteArray body;
    QString contentType = QStringLiteral("text/plain; charset=utf-8");
    int status = 200;

    if (path.startsWith(QLatin1String("/assets/main/"))) {
        bool ok = false;
        body = readSkinAsset(QStringLiteral("main"), path.mid(QStringLiteral("/assets/main/").size()), &ok);
        if (!ok)
            status = 404;
        else
            contentType = mimeTypeForPath(path);
    } else if (path.startsWith(QLatin1String("/assets/piano/"))) {
        bool ok = false;
        body = readSkinAsset(QStringLiteral("piano"), path.mid(QStringLiteral("/assets/piano/").size()), &ok);
        if (!ok)
            status = 404;
        else
            contentType = mimeTypeForPath(path);
    } else if (path.startsWith(QLatin1String("/static/"))) {
        const QString rel = path.mid(QStringLiteral("/static/").size());
        body = readResource(QStringLiteral(":/browser-source/") + rel);
        if (body.isEmpty())
            status = 404;
        else
            contentType = mimeTypeForPath(path);
    } else if (path == QLatin1String("/") || path == QLatin1String("/main")
               || path == QLatin1String("/stacked") || path == QLatin1String("/piano")) {
        const QString view = parseViewFromPath(path == QLatin1String("/") ? QStringLiteral("/main") : path);
        body = readResource(QStringLiteral(":/browser-source/") + view + QStringLiteral(".html"));
        if (body.isEmpty())
            status = 404;
        else
            contentType = QStringLiteral("text/html; charset=utf-8");
    } else {
        status = 404;
    }

    QByteArray response;
    if (status == 404) {
        response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
    } else {
        response = QByteArray("HTTP/1.1 200 OK\r\n")
            + "Content-Type: " + contentType.toUtf8() + "\r\n"
            + "Content-Length: " + QByteArray::number(body.size()) + "\r\n"
            + "Connection: close\r\n\r\n"
            + body;
    }
    socket->write(response);
    socket->disconnectFromHost();
}

int BrowserSourceServer::selfTest()
{
    const QString appDir = QCoreApplication::applicationDirPath();
    QStringList skinCandidates = {
        appDir + QStringLiteral("/Skins/Default/skin.xml"),
        appDir + QStringLiteral("/../Skins/Default/skin.xml"),
        appDir + QStringLiteral("/../../Skins/Default/skin.xml")
    };
    QString skinPath;
    for (const QString &candidate : skinCandidates) {
        if (QFileInfo::exists(candidate)) {
            skinPath = candidate;
            break;
        }
    }
    if (skinPath.isEmpty())
        return 1;

    const QString pianoPath = QFileInfo(skinPath).absolutePath() + QStringLiteral("/pianodisplay.xml");

    RegularSkin mainSkin = SkinParser::parseRegularSkin(skinPath);
    PianoSkin pianoSkin;
    if (QFileInfo::exists(pianoPath))
        pianoSkin = SkinParser::parsePianoSkin(pianoPath);

    BrowserSourceServer server;
    if (!server.start(mainSkin, pianoSkin, 0))
        return 2;
    const quint16 port = server.serverPort();
    if (port == 0)
        return 3;

    QTcpSocket http;
    http.connectToHost(QHostAddress::LocalHost, port);
    if (!http.waitForConnected(3000))
        return 4;
    http.write("GET /main HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n");
    {
        QEventLoop loop;
        QTimer::singleShot(3000, &loop, &QEventLoop::quit);
        QObject::connect(&http, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
        QObject::connect(&http, &QTcpSocket::disconnected, &loop, &QEventLoop::quit);
        loop.exec();
    }
    const QByteArray httpResp = http.readAll();
    if (!httpResp.contains("200 OK") || !httpResp.contains("text/html"))
        return 6;
    http.close();

    QWebSocket ws;
    bool gotInit = false;
    bool wsConnected = false;
    QObject::connect(&ws, &QWebSocket::connected, [&]() { wsConnected = true; });
    QObject::connect(&ws, &QWebSocket::textMessageReceived, [&](const QString &msg) {
        const QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        if (doc.isObject() && doc.object().value(QStringLiteral("type")).toString() == QLatin1String("init"))
            gotInit = true;
    });
    ws.open(QUrl(QStringLiteral("ws://127.0.0.1:%1/ws?view=main").arg(port)));
    {
        QEventLoop loop;
        QTimer::singleShot(3000, &loop, &QEventLoop::quit);
        QObject::connect(&ws, &QWebSocket::connected, &loop, &QEventLoop::quit);
        loop.exec();
    }
    if (!wsConnected)
        return 7;

    for (int i = 0; i < 50 && !gotInit; ++i) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
    if (!gotInit)
        return 8;

    bool gotInput = false;
    QObject::disconnect(&ws, nullptr, nullptr, nullptr);
    QObject::connect(&ws, &QWebSocket::textMessageReceived, [&](const QString &msg) {
        const QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        if (doc.isObject() && doc.object().value(QStringLiteral("type")).toString() == QLatin1String("input"))
            gotInput = true;
    });
    server.onButtonPressed(InputProvider::Up);
    for (int i = 0; i < 50 && !gotInput; ++i) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
    if (!gotInput)
        return 9;

    ws.close();
    server.stop();
    return 0;
}
