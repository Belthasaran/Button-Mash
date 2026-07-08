#include "buttonmashremoteprovider.h"
#include "snesbitorder.h"
#include <QDateTime>
#include <QHostAddress>
#include <QtEndian>
#include <cstring>

static const char kBmirMagic[4] = { 'B', 'M', 'I', 'R' };

ButtonMashRemoteProvider::ButtonMashRemoteProvider(quint16 port, QObject *parent)
    : InputProvider(parent)
    , m_port(port)
    , m_socket(new QUdpSocket(this))
    , m_state(0)
    , m_listening(false)
{
    connect(m_socket, &QUdpSocket::readyRead, this, &ButtonMashRemoteProvider::onReadyRead);
}

ButtonMashRemoteProvider::~ButtonMashRemoteProvider()
{
    stop();
}

void ButtonMashRemoteProvider::setPort(quint16 port)
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

void ButtonMashRemoteProvider::start()
{
    if (m_listening)
        return;
    m_listening = m_socket->bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void ButtonMashRemoteProvider::stop()
{
    m_socket->close();
    m_listening = false;
}

bool ButtonMashRemoteProvider::isReady()
{
    return m_listening && m_socket->state() == QAbstractSocket::BoundState;
}

QString ButtonMashRemoteProvider::statusText()
{
    if (isReady())
        return tr("Listening for Button Mash remote on UDP %1").arg(m_port);
    return tr("Not listening for Button Mash remote");
}

QString ButtonMashRemoteProvider::name() const
{
    return QStringLiteral("Remote Button Mash (%1)").arg(m_port);
}

QByteArray ButtonMashRemoteProvider::buildPacket(quint16 after, quint16 pressed, quint16 released,
                                                 quint64 timestampMs, const QByteArray &sessionId, bool fullState)
{
    QByteArray pkt;
    pkt.reserve(4 + 1 + 1 + 8 + 16 + 2 + 2 + 2);
    pkt.append(kBmirMagic, 4);
    pkt.append(char(0x01)); // version
    pkt.append(char(fullState ? 0x03 : 0x01));
    if (timestampMs == 0)
        timestampMs = quint64(QDateTime::currentMSecsSinceEpoch());
    quint64 beTs = qToBigEndian(timestampMs);
    pkt.append(reinterpret_cast<const char *>(&beTs), 8);
    QByteArray sid = sessionId;
    if (sid.size() < 16)
        sid.append(QByteArray(16 - sid.size(), '\0'));
    else if (sid.size() > 16)
        sid = sid.left(16);
    pkt.append(sid);
    quint16 beAfter = qToBigEndian(after);
    quint16 bePressed = qToBigEndian(pressed);
    quint16 beReleased = qToBigEndian(released);
    pkt.append(reinterpret_cast<const char *>(&beAfter), 2);
    pkt.append(reinterpret_cast<const char *>(&bePressed), 2);
    pkt.append(reinterpret_cast<const char *>(&beReleased), 2);
    return pkt;
}

void ButtonMashRemoteProvider::applyBitvector(quint16 after, quint16 pressed, quint16 released, bool fullState)
{
    if (fullState) {
        quint16 before = m_state;
        pressed = quint16(after & ~before);
        released = quint16(before & ~after);
        m_state = after;
    } else {
        m_state = quint16((m_state | pressed) & ~released);
    }

    for (int bit = 4; bit < 16; ++bit) {
        quint16 mask = quint16(1u << bit);
        InputProvider::SNESButton button = SnesBitOrder::buttonForBitIndex(bit);
        if (pressed & mask)
            emit buttonPressed(button);
        if (released & mask)
            emit buttonReleased(button);
    }
}

bool ButtonMashRemoteProvider::handleDatagram(const QByteArray &datagram)
{
    if (datagram.size() < 4 + 1 + 1 + 8 + 16 + 6)
        return false;
    if (std::memcmp(datagram.constData(), kBmirMagic, 4) != 0)
        return false;
    const uchar version = uchar(datagram.at(4));
    if (version != 0x01)
        return false;
    const uchar flags = uchar(datagram.at(5));
    const bool fullState = (flags & 0x02) != 0;
    int off = 4 + 1 + 1 + 8 + 16;
    quint16 after = qFromBigEndian<quint16>(reinterpret_cast<const uchar *>(datagram.constData() + off));
    quint16 pressed = qFromBigEndian<quint16>(reinterpret_cast<const uchar *>(datagram.constData() + off + 2));
    quint16 released = qFromBigEndian<quint16>(reinterpret_cast<const uchar *>(datagram.constData() + off + 4));
    applyBitvector(after, pressed, released, fullState);
    return true;
}

void ButtonMashRemoteProvider::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(m_socket->pendingDatagramSize()));
        m_socket->readDatagram(datagram.data(), datagram.size());
        handleDatagram(datagram);
    }
}
