#ifndef BUTTONMASHREMOTEPROVIDER_H
#define BUTTONMASHREMOTEPROVIDER_H

#include "inputprovider.h"
#include <QUdpSocket>

class ButtonMashRemoteProvider : public InputProvider
{
    Q_OBJECT
public:
    // When true, trailing XXH3 is computed; when false, trailer is eight zero bytes.
    static constexpr bool kBmirXxh3Enabled = true;
    static constexpr int kBmirBodyBytes = 36;
    static constexpr int kBmirXxh3Bytes = 8;
    static constexpr int kMaxUdpDatagram = 4096;

    explicit ButtonMashRemoteProvider(quint16 port = 27151, QObject *parent = nullptr);
    ~ButtonMashRemoteProvider() override;

    void start() override;
    void stop() override;
    bool isReady() override;
    QString statusText() override;
    QString name() const override;

    quint16 port() const { return m_port; }
    void setPort(quint16 port);

    bool handleDatagram(const QByteArray &datagram);
    static QByteArray buildPacket(quint16 after, quint16 pressed, quint16 released,
                                  quint64 timestampMs = 0, const QByteArray &sessionId = QByteArray(),
                                  bool fullState = true);

private slots:
    void onReadyRead();

private:
    void applyBitvector(quint16 after, quint16 pressed, quint16 released, bool fullState);

    quint16 m_port;
    QUdpSocket *m_socket;
    quint16 m_state;
    bool m_listening;
};

#endif
