#ifndef INPUTMIRRORMANAGER_H
#define INPUTMIRRORMANAGER_H

#include "inputprovider.h"
#include "inputsessionlogger.h"
#include <QObject>
#include <QUdpSocket>
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

public slots:
    void onButtonPressed(InputProvider::SNESButton button);
    void onButtonReleased(InputProvider::SNESButton button);

private:
    void sendToTargets(quint16 after, quint16 pressed, quint16 released);
    QByteArray buildRetroArchPacket(InputProvider::SNESButton button, bool pressed) const;

    InputSessionLogger m_logger;
    QUdpSocket m_udp;
    bool m_shareEnabled;
    QVector<MirrorRemoteTarget> m_targets;
    quint16 m_bits;
};

#endif
