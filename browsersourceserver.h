#ifndef BROWSERSOURCESERVER_H
#define BROWSERSOURCESERVER_H

#include "inputprovider.h"
#include "skinparser.h"

#include <QMap>
#include <QObject>
#include <QSettings>
#include <QSet>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QWebSocket>
#include <QWebSocketServer>

class BrowserSourceServer : public QObject
{
    Q_OBJECT
public:
    explicit BrowserSourceServer(QObject *parent = nullptr);
    ~BrowserSourceServer() override;

    static bool enabled();
    static quint16 port();
    static void loadSettings(QSettings &settings);
    static void saveSettings(QSettings &settings);
    static void setEnabled(bool enabled);
    static void setPort(quint16 port);
    static QString urlForView(const QString &view, quint16 port);

    bool start(const RegularSkin &mainSkin, const PianoSkin &pianoSkin, quint16 port);
    void stop();
    bool isRunning() const { return m_running; }
    quint16 serverPort() const { return m_tcp.serverPort(); }

    void setDelai(unsigned int delai);

    static int selfTest();

public slots:
    void onButtonPressed(InputProvider::SNESButton button);
    void onButtonReleased(InputProvider::SNESButton button);

private slots:
    void onTcpNewConnection();
    void onWsNewConnection();
    void onWsDisconnected();

private:
    struct InputChange {
        QString button;
        bool pressed = false;
    };

    void serveHttp(QTcpSocket *socket, const QByteArray &request, const QString &path);
    void upgradeWebSocket(QTcpSocket *socket, const QByteArray &request, const QString &view);
    QByteArray readResource(const QString &resourcePath) const;
    QString mimeTypeForPath(const QString &path) const;
    QByteArray readSkinAsset(const QString &prefix, const QString &relPath, bool *ok) const;
    QString buttonName(InputProvider::SNESButton button) const;
    QJsonObject buildMainSkinJson() const;
    QJsonObject buildPianoSkinJson() const;
    QByteArray buildInitMessage(const QString &view) const;
    void sendInit(QWebSocket *socket, const QString &view);
    void broadcastInput(const QList<InputChange> &changes);
    void dispatchButton(InputProvider::SNESButton button, bool pressed);
    QString parseViewFromPath(const QString &path) const;
    QString parseViewFromQuery(const QString &query) const;

    static bool s_enabled;
    static quint16 s_port;

    QTcpServer m_tcp;
    QWebSocketServer *m_wsServer;
    QList<QWebSocket *> m_clients;
    QMap<QWebSocket *, QString> m_clientViews;

    RegularSkin m_mainSkin;
    PianoSkin m_pianoSkin;
    QString m_mainRoot;
    QString m_pianoRoot;
    int m_mainSceneWidth;
    int m_mainSceneHeight;
    QSet<QString> m_pressed;
    quint16 m_listenPort;
    bool m_running;
    unsigned int m_delai;
    QString m_pendingWsView;
};

#endif // BROWSERSOURCESERVER_H
