#pragma once
#include "client.hpp"
// #include <QUdpSocket>
#include <qqml.h>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/manual_control/manual_control.h>
#include <mavsdk/plugins/action/action.h>


class ClientMavLink : public Client
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ClientMavLink(QObject *parent = nullptr);

private:
    // QUdpSocket *socket = nullptr;
    // QHostAddress server;
    // const quint16 port = 51324;

    mavsdk::Mavsdk mavsdk;

    std::shared_ptr<mavsdk::ManualControl> manual_control;

    std::shared_ptr<mavsdk::Action> action;

public slots:
    bool start(const QString &server) override;

    bool stop() override;

    bool
    send(float roll, float pitch, float yaw, float throttle,
         float aux1 = NAN, float aux2 = NAN, float aux3 = NAN,
         float aux4 = NAN, float aux5 = NAN, float aux6 = NAN)  override;

signals:

};

