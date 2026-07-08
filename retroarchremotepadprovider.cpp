#include "retroarchremotepadprovider.h"
#include <QHostAddress>
#include <QtEndian>
#include <cstring>

// Match RetroArch remote_message layout (x86_64 Linux: 20 bytes).
struct RemoteMessageWire
{
    qint32 port;
    qint32 device;
    qint32 index;
    qint32 id;
    quint16 state;
};

#if defined(__GNUC__) || defined(__clang__)
static_assert(sizeof(RemoteMessageWire) == 20 || sizeof(RemoteMessageWire) == 24,
              "Unexpected remote_message size");
#endif

enum {
    RETRO_DEVICE_JOYPAD = 1,
    RETRO_DEVICE_ANALOG = 5
};

RetroArchRemotePadProvider::RetroArchRemotePadProvider(quint16 port, QObject *parent)
    : InputProvider(parent)
    , m_port(port)
    , m_socket(new QUdpSocket(this))
    , m_listening(false)
{
    connect(m_socket, &QUdpSocket::readyRead, this, &RetroArchRemotePadProvider::onReadyRead);
}

RetroArchRemotePadProvider::~RetroArchRemotePadProvider()
{
    stop();
}

void RetroArchRemotePadProvider::setPort(quint16 port)
{
    if (m_port == port)
        return;
    bool was = m_listening;
    if (was)
        stop();
    m_port = port;
    if (was)
        start();
}

void RetroArchRemotePadProvider::start()
{
    if (m_listening)
        return;
    m_listening = m_socket->bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void RetroArchRemotePadProvider::stop()
{
    m_socket->close();
    m_listening = false;
}

bool RetroArchRemotePadProvider::isReady()
{
    // Ready as a selectable source before bind; SkinSelector starts listening on Start.
    return true;
}

QString RetroArchRemotePadProvider::statusText()
{
    if (m_listening && m_socket->state() == QAbstractSocket::BoundState)
        return tr("Listening for RetroArch RemotePad on UDP %1").arg(m_port);
    return tr("Ready to listen for RetroArch RemotePad on UDP %1").arg(m_port);
}

QString RetroArchRemotePadProvider::name() const
{
    return QStringLiteral("Remote RetroArch RemotePad (%1)").arg(m_port);
}

InputProvider::SNESButton RetroArchRemotePadProvider::mapJoypadId(int id)
{
    switch (id) {
    case 0: return B;
    case 1: return Y;
    case 2: return Select;
    case 3: return Start;
    case 4: return Up;
    case 5: return Down;
    case 6: return Left;
    case 7: return Right;
    case 8: return A;
    case 9: return X;
    case 10: return L;
    case 11: return R;
    default: return B; // invalid — caller checks id range before use
    }
}

bool RetroArchRemotePadProvider::handleDatagram(const QByteArray &datagram)
{
    if (datagram.size() < int(sizeof(RemoteMessageWire)))
        return false;

    RemoteMessageWire msg;
    std::memcpy(&msg, datagram.constData(), sizeof(msg));
    // RetroArch sends native host endian integers (little-endian on PC).
    if (msg.device != RETRO_DEVICE_JOYPAD)
        return false;
    if (msg.id < 0 || msg.id > 11)
        return false;

    SNESButton button = mapJoypadId(msg.id);

    const bool nowPressed = msg.state != 0;
    const bool wasPressed = m_pressed.value(msg.id, false);
    if (nowPressed == wasPressed)
        return true;
    m_pressed[msg.id] = nowPressed;
    if (nowPressed)
        emit buttonPressed(button);
    else
        emit buttonReleased(button);
    return true;
}

void RetroArchRemotePadProvider::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(m_socket->pendingDatagramSize()));
        m_socket->readDatagram(datagram.data(), datagram.size());
        handleDatagram(datagram);
    }
}
