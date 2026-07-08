#ifndef RETROARCHREMOTEPADPROVIDER_H
#define RETROARCHREMOTEPADPROVIDER_H

#include "inputprovider.h"
#include <QUdpSocket>
#include <QHash>

class RetroArchRemotePadProvider : public InputProvider
{
    Q_OBJECT
public:
    explicit RetroArchRemotePadProvider(quint16 port = 55400, QObject *parent = nullptr);
    ~RetroArchRemotePadProvider() override;

    void start() override;
    void stop() override;
    bool isReady() override;
    QString statusText() override;
    QString name() const override;

    quint16 port() const { return m_port; }
    void setPort(quint16 port);

    // Exposed for tests: parse one datagram payload.
    bool handleDatagram(const QByteArray &datagram);

private slots:
    void onReadyRead();

private:
    quint16 m_port;
    QUdpSocket *m_socket;
    QHash<int, bool> m_pressed; // Retro joypad id -> pressed
    bool m_listening;

    static InputProvider::SNESButton mapJoypadId(int id);
};

#endif
