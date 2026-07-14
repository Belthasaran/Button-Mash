#ifndef INPUTMIRRORMANAGER_H
#define INPUTMIRRORMANAGER_H

#include "inputprovider.h"
#include "inputsessionlogger.h"
#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QVector>

struct MirrorRemoteTarget
{
    QString protocol; // "RetroArchRemotePad" or "ButtonMash"
    QString host;
    quint16 port = 55400;
    bool enabled = true;
};

class InputMirrorManager : public QObject
{
    Q_OBJECT
public:
    explicit InputMirrorManager(QObject *parent = nullptr);

    InputSessionLogger *logger() { return &m_logger; }

    void setShareEnabled(bool enabled);
    bool shareEnabled() const { return m_shareEnabled; }

    void setTargets(const QVector<MirrorRemoteTarget> &targets);
    QVector<MirrorRemoteTarget> targets() const { return m_targets; }

    void loadSettings(class QSettings &settings);
    void saveSettings(class QSettings &settings) const;

    void startSession();
    void stopSession();

    /** Little-endian remote_message: 20 bytes (18 usable + 2 pad). */
    static QByteArray buildRetroArchPacket(InputProvider::SNESButton button, bool pressed);

    /** Exponential backoff step for idle resync (testable). */
    static int nextResyncIntervalMs(int currentIntervalMs);

public slots:
    void onButtonPressed(InputProvider::SNESButton button);
    void onButtonReleased(InputProvider::SNESButton button);

private slots:
    void onResyncTimer();

private:
    void sendToTargets(quint16 after, quint16 pressed, quint16 released);
    void sendFullStateResync();
    void armResyncTimer();
    void stopResyncTimer();
    void noteInputActivity();

    InputSessionLogger m_logger;
    QUdpSocket m_udp;
    QTimer m_resyncTimer;
    bool m_shareEnabled;
    QVector<MirrorRemoteTarget> m_targets;
    quint16 m_bits;
    qint64 m_lastInputChangeMs = 0;
    int m_resyncIntervalMs = 300;
};

#endif
