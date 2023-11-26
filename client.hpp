#pragma once
#include <QObject>


class Client : public QObject
{
public:
    explicit Client(QObject *parent = nullptr) {}

public slots:
    virtual bool
    start(const QString &server) = 0;

    virtual bool
    stop() = 0;

    virtual bool
    send(float roll, float pitch, float yaw, float throttle,
         float aux1 = NAN, float aux2 = NAN, float aux3 = NAN,
         float aux4 = NAN, float aux5 = NAN, float aux6 = NAN) = 0;

};

