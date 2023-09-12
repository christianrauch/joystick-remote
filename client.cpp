#include "client.hpp"
#include <iostream>

Client::Client(QObject *parent)
    : QObject{parent}
{
//    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(send_channels()));
//    timer->start(1000);
}

void
Client::connect(const QString &server, bool on)
{
    // socket etc.
    if (on)
        qInfo() << "connect: " << server.toStdString();
    else
        qInfo() << "disconnect";
}

//void
//Client::disconnect()
//{
//    // socket etc.
//    qInfo() << "disconnect";
//}

//void
//Client::send(const std::vector<float> &channels)
//{
//    qInfo() << "send " << channels;
//}

// TODO: fix channel order

void
Client::send_channels(float a)
{
//    std::cout << "send" << std::endl;
    qInfo() << "send " << a;
//    qFatal() << "send";
}
