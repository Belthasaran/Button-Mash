#include "inputmirrormanager.h"
#include "buttonmashremoteprovider.h"
#include "snesbitorder.h"
#include <QDateTime>
#include <QHostAddress>
#include <QSettings>
#include <QtEndian>
#include <cstring>

InputMirrorManager::InputMirrorManager(QObject *parent)
    : QObject(parent)
    , m_shareEnabled(false)
    , m_bits(0)
{
}

void InputMirrorManager::setShareEnabled(bool enabled)
{
    m_shareEnabled = enabled;
}

void InputMirrorManager::setTargets(const QVector<MirrorRemoteTarget> &targets)
{
    m_targets = targets;
}

void InputMirrorManager::loadSettings(QSettings &settings)
{
    m_shareEnabled = settings.value(QStringLiteral("mirror/shareEnabled"), false).toBool();
    m_logger.setBinLog(settings.value(QStringLiteral("mirror/binEnabled"), false).toBool(),
                       settings.value(QStringLiteral("mirror/binPath")).toString());
    m_logger.setFullTextLog(settings.value(QStringLiteral("mirror/fullTextEnabled"), false).toBool(),
                            settings.value(QStringLiteral("mirror/fullTextPath")).toString());
    m_logger.setSyncOnlyTextLog(settings.value(QStringLiteral("mirror/syncTextEnabled"), false).toBool(),
                                settings.value(QStringLiteral("mirror/syncTextPath")).toString());
    m_logger.setLastNDashboard(settings.value(QStringLiteral("mirror/lastNEnabled"), false).toBool(),
                               settings.value(QStringLiteral("mirror/lastNPath")).toString(),
                               settings.value(QStringLiteral("mirror/lastN"), 10).toInt());

    m_targets.clear();
    const int n = settings.beginReadArray(QStringLiteral("mirror/targets"));
    for (int i = 0; i < n; ++i) {
        settings.setArrayIndex(i);
        MirrorRemoteTarget t;
        t.protocol = settings.value(QStringLiteral("protocol"), QStringLiteral("ButtonMash")).toString();
        t.host = settings.value(QStringLiteral("host"), QStringLiteral("127.0.0.1")).toString();
        t.port = quint16(settings.value(QStringLiteral("port"), 27151).toUInt());
        t.enabled = settings.value(QStringLiteral("enabled"), true).toBool();
        m_targets.append(t);
    }
    settings.endArray();
}

void InputMirrorManager::saveSettings(QSettings &settings) const
{
    settings.setValue(QStringLiteral("mirror/shareEnabled"), m_shareEnabled);
    settings.setValue(QStringLiteral("mirror/binEnabled"), m_logger.binEnabled());
    settings.setValue(QStringLiteral("mirror/binPath"), m_logger.binPath());
    settings.setValue(QStringLiteral("mirror/fullTextEnabled"), m_logger.fullTextEnabled());
    settings.setValue(QStringLiteral("mirror/fullTextPath"), m_logger.fullTextPath());
    settings.setValue(QStringLiteral("mirror/syncTextEnabled"), m_logger.syncOnlyTextEnabled());
    settings.setValue(QStringLiteral("mirror/syncTextPath"), m_logger.syncOnlyTextPath());
    settings.setValue(QStringLiteral("mirror/lastNEnabled"), m_logger.lastNEnabled());
    settings.setValue(QStringLiteral("mirror/lastNPath"), m_logger.lastNBasePath());
    settings.setValue(QStringLiteral("mirror/lastN"), m_logger.lastN());

    settings.beginWriteArray(QStringLiteral("mirror/targets"), m_targets.size());
    for (int i = 0; i < m_targets.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(QStringLiteral("protocol"), m_targets.at(i).protocol);
        settings.setValue(QStringLiteral("host"), m_targets.at(i).host);
        settings.setValue(QStringLiteral("port"), m_targets.at(i).port);
        settings.setValue(QStringLiteral("enabled"), m_targets.at(i).enabled);
    }
    settings.endArray();
}

void InputMirrorManager::startSession()
{
    m_bits = 0;
    if (!m_shareEnabled)
        return;
    // startLogging mints Ed25519 keys and derives session id.
    m_logger.startLogging();
}

void InputMirrorManager::stopSession()
{
    m_logger.stopLogging();
}

QByteArray InputMirrorManager::buildRetroArchPacket(InputProvider::SNESButton button, bool pressed)
{
    int id = -1;
    switch (button) {
    case InputProvider::B: id = 0; break;
    case InputProvider::Y: id = 1; break;
    case InputProvider::Select: id = 2; break;
    case InputProvider::Start: id = 3; break;
    case InputProvider::Up: id = 4; break;
    case InputProvider::Down: id = 5; break;
    case InputProvider::Left: id = 6; break;
    case InputProvider::Right: id = 7; break;
    case InputProvider::A: id = 8; break;
    case InputProvider::X: id = 9; break;
    case InputProvider::L: id = 10; break;
    case InputProvider::R: id = 11; break;
    }
    if (id < 0)
        return QByteArray();

    // Explicit LE wire layout matching RetroArchRemotePadProvider receive
    // (18 usable bytes; trailing 2 pad bytes for common sizeof==20 hosts).
    QByteArray pkt(20, '\0');
    qToLittleEndian<qint32>(0, pkt.data() + 0);       // port
    qToLittleEndian<qint32>(1, pkt.data() + 4);       // JOYPAD
    qToLittleEndian<qint32>(0, pkt.data() + 8);       // index
    qToLittleEndian<qint32>(id, pkt.data() + 12);     // id
    qToLittleEndian<quint16>(pressed ? 1 : 0, pkt.data() + 16); // state
    return pkt;
}

void InputMirrorManager::sendToTargets(quint16 after, quint16 pressed, quint16 released)
{
    for (const MirrorRemoteTarget &t : m_targets) {
        if (!t.enabled || t.host.isEmpty())
            continue;
        QByteArray pkt;
        if (t.protocol == QLatin1String("RetroArchRemotePad")) {
            // Send one RetroArch message per edge bit — handled in onButton* instead.
            Q_UNUSED(after);
            Q_UNUSED(pressed);
            Q_UNUSED(released);
            continue;
        } else {
            pkt = ButtonMashRemoteProvider::buildPacket(after, pressed, released, 0,
                                                        m_logger.state().sessionId(), true);
        }
        if (!pkt.isEmpty())
            m_udp.writeDatagram(pkt, QHostAddress(t.host), t.port);
    }
}

void InputMirrorManager::onButtonPressed(InputProvider::SNESButton button)
{
    const quint16 mask = SnesBitOrder::maskForButton(button);
    const quint16 before = m_bits;
    m_bits = quint16(m_bits | mask);
    const quint16 pressed = quint16(m_bits & ~before);
    if (m_shareEnabled) {
        m_logger.onButton(button, true);
        // RetroArch targets: per-button packets
        for (const MirrorRemoteTarget &t : m_targets) {
            if (!t.enabled)
                continue;
            if (t.protocol == QLatin1String("RetroArchRemotePad")) {
                QByteArray pkt = buildRetroArchPacket(button, true);
                if (!pkt.isEmpty())
                    m_udp.writeDatagram(pkt, QHostAddress(t.host), t.port);
            }
        }
        sendToTargets(m_bits, pressed, 0);
    }
}

void InputMirrorManager::onButtonReleased(InputProvider::SNESButton button)
{
    const quint16 mask = SnesBitOrder::maskForButton(button);
    const quint16 before = m_bits;
    m_bits = quint16(m_bits & ~mask);
    const quint16 released = quint16(before & ~m_bits);
    if (m_shareEnabled) {
        m_logger.onButton(button, false);
        for (const MirrorRemoteTarget &t : m_targets) {
            if (!t.enabled)
                continue;
            if (t.protocol == QLatin1String("RetroArchRemotePad")) {
                QByteArray pkt = buildRetroArchPacket(button, false);
                if (!pkt.isEmpty())
                    m_udp.writeDatagram(pkt, QHostAddress(t.host), t.port);
            }
        }
        sendToTargets(m_bits, 0, released);
    }
}
