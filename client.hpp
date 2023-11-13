#pragma once
#include <QObject>
#include <QUdpSocket>
#include <qqml.h>


class Client : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Client(QObject *parent = nullptr);

private:
//    void connect(const QString &server);

    QUdpSocket *socket = nullptr;
    QHostAddress server;
    const quint16 port = 51324;

public slots:
    void connect(const QString &server, bool on);

//    void disconnect();

//    void send(const std::vector<float> &channels);
//    void send(float );

    // send channels in TAER order:
    // 1: (T) throttle
    // 2: (A) roll
    // 3: (E) pitch
    // 4: (R) yaw
    void send_channels(float throttle, float roll, float pitch, float yaw,
                       float aux1 = NAN, float aux2 = NAN, float aux3 = NAN, float aux4 = NAN, float aux5 = NAN, float aux6 = NAN);

signals:

};

