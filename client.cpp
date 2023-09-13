#include "client.hpp"
#include <iostream>
#include <QBuffer>


Client::Client(QObject *parent)
    : QObject{parent}
{
//    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(send_channels()));
//    timer->start(1000);
}

//void
//Client::connect(const QString &server)
//{
//    socket.bind(QHostAddress(server), 51324);
//}

void
Client::connect(const QString &server, bool on)
{
    // socket etc.
    if (on) {
        qInfo() << "connect: " << server.toStdString();
//        this->connect(server);
        this->server = QHostAddress(server);
        socket.bind(this->server, port);
    }
    else {
        qInfo() << "disconnect";
        socket.close();
    }
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
Client::send_channels(float throttle, float roll, float pitch, float yaw,
                      float aux1, float aux2, float aux3, float aux4)
{
//    std::cout << "send" << std::endl;
//    if (!socket.isOpen())
//        return;

//    qInfo() << "send " << throttle;
//    qFatal() << "send";

    throttle = (-throttle + 1) / 2;
    yaw = (1 + yaw ) / 2;
    pitch = (-pitch + 1) / 2;
    roll = (1 + roll ) / 2;

    qInfo() << "TAER: " << throttle << ", " << roll << ", " << pitch << ", " << yaw << ", " << aux1 << ", " << aux2;

//    QByteArray data;
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    // network byte order
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setFloatingPointPrecision(QDataStream::FloatingPointPrecision::SinglePrecision);

    const uint8_t nchannels = 4 + !std::isnan(aux1) + !std::isnan(aux2) + !std::isnan(aux3) + !std::isnan(aux4);
    stream << nchannels << throttle << roll << pitch << yaw;
//    stream << uint8_t(1) << aux1;
//    stream << uint8_t(1);
//    stream << 1.0f;
    if (!std::isnan(aux1))
        stream << aux1;
    if (!std::isnan(aux2))
        stream << aux2;
    if (!std::isnan(aux3))
        stream << aux3;
    if (!std::isnan(aux4))
        stream << aux4;

//    QByteArray data;
//    data.append(nchannels);
//    data.append(reinterpret_cast<const char *>(&throttle), sizeof(float));
//    data.append(reinterpret_cast<const char *>(&roll), sizeof(float));
//    data.append(reinterpret_cast<const char *>(&pitch), sizeof(float));
//    data.append(reinterpret_cast<const char *>(&yaw), sizeof(float));
//    if (!std::isnan(aux1))
//        data.append(reinterpret_cast<const char *>(&aux1), sizeof(float));
//    if (!std::isnan(aux2))
//        data.append(reinterpret_cast<const char *>(&aux2), sizeof(float));
//    if (!std::isnan(aux3))
//        data.append(reinterpret_cast<const char *>(&aux3), sizeof(float));
//    if (!std::isnan(aux4))
//        data.append(reinterpret_cast<const char *>(&aux4), sizeof(float));

    qInfo() << "buf(" << buffer.data().size() << "): " << buffer.data();

    socket.writeDatagram(buffer.data(), this->server, port);
}
