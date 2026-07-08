#include "retroarchremotepadprovider.h"
#include <QHostAddress>
#include <QtEndian>
#include <cstring>

enum {
    RETRO_DEVICE_JOYPAD = 1,
    RETRO_DEVICE_ANALOG = 5
};

// Wire layout (little-endian on PC RetroArch hosts). Useful payload is 18 bytes;
// RetroArch's native sizeof is often 20 with trailing alignment padding.
// Field offsets: port@0 device@4 index@8 id@12 state@16.

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
    // Need at least through state (offset 16 + 2). Extra trailing padding/junk ignored.
    if (datagram.size() < kWireBytes || datagram.size() > kMaxUdpDatagram)
        return false;

    const uchar *p = reinterpret_cast<const uchar *>(datagram.constData());
    const qint32 device = qFromLittleEndian<qint32>(p + 4);
    const qint32 id = qFromLittleEndian<qint32>(p + 12);
    const quint16 state = qFromLittleEndian<quint16>(p + 16);

    if (device != RETRO_DEVICE_JOYPAD)
        return false;
    if (id < 0 || id > 11)
        return false;

    SNESButton button = mapJoypadId(id);

    const bool nowPressed = state != 0;
    const bool wasPressed = m_pressed.value(id, false);
    if (nowPressed == wasPressed)
        return true;
    m_pressed[id] = nowPressed;
    if (nowPressed)
        emit buttonPressed(button);
    else
        emit buttonReleased(button);
    return true;
}

void RetroArchRemotePadProvider::onReadyRead()
{
    char discard[1024];
    while (m_socket->hasPendingDatagrams()) {
        const qint64 sz = m_socket->pendingDatagramSize();
        if (sz < 0) {
            m_socket->readDatagram(discard, sizeof(discard));
            continue;
        }
        if (sz > kMaxUdpDatagram) {
            m_socket->readDatagram(discard, sizeof(discard));
            continue;
        }
        if (sz < kWireBytes) {
            m_socket->readDatagram(discard, sizeof(discard));
            continue;
        }
        QByteArray datagram;
        datagram.resize(int(sz));
        const qint64 n = m_socket->readDatagram(datagram.data(), datagram.size());
        if (n < kWireBytes)
            continue;
        if (n != datagram.size())
            datagram.resize(int(n));
        handleDatagram(datagram);
    }
}
