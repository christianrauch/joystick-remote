#pragma once
#include "client.hpp"
#include <QUdpSocket>
#include <qqml.h>


class ClientMavLink : public Client
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ClientMavLink(QObject *parent = nullptr);

private:
    QUdpSocket *socket = nullptr;
    QHostAddress server;
    const quint16 port = 51324;

public slots:
    void start(const QString &server) override;

    void stop() override;

    void
    send(float roll, float pitch, float yaw, float throttle,
         float aux1 = NAN, float aux2 = NAN, float aux3 = NAN,
         float aux4 = NAN, float aux5 = NAN, float aux6 = NAN)  override;

signals:

};

