#include "client-mavlink.hpp"
#include <iostream>
#include <QBuffer>
#include <zlib.h>


ClientMavLink::ClientMavLink(QObject *parent)
    : Client{parent}
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
ClientMavLink::start(const QString &server)
{
    // socket etc.
    qInfo() << "connect: " << server.toStdString();
    this->server = QHostAddress(server);
    socket = new QUdpSocket;
    socket->bind(this->server, port);
}

void
ClientMavLink::stop()
{
    qInfo() << "disconnecting ...";
    //        while (socket->hasPendingDatagrams());
    socket->close();
    qInfo() << "disconnected";
    delete socket;
    socket = nullptr;
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
ClientMavLink::send(float roll, float pitch, float yaw, float throttle,
                float aux1, float aux2, float aux3,
                float aux4, float aux5, float aux6)
{
    if (!socket) {
//        qInfo() << "not connected";
        return;
    }

//    qInfo() << "send " << throttle;
//    qFatal() << "send";

//    throttle = (-throttle + 1) / 2;
//    yaw = (1 + yaw ) / 2;
//    pitch = (-pitch + 1) / 2;
//    roll = (1 + roll ) / 2;

    if (throttle != 0)
        throttle *= -1;
    if (pitch != 0)
        pitch *= -1;

    qInfo() << "TAER: " << throttle << ", " << roll << ", " << pitch << ", " << yaw << ", " << aux1 << ", " << aux2;

//    QByteArray data;
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    // network byte order
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setFloatingPointPrecision(QDataStream::FloatingPointPrecision::SinglePrecision);

//    stream << uint8_t(0x0F) << nchannels << throttle << roll << pitch << yaw;
    stream << uint8_t(0x0F) << roll << pitch << yaw << throttle;
//    stream << uint8_t(1) << aux1;
//    stream << uint8_t(1);
//    stream << 1.0f;

    const uint8_t nauxchannels = !std::isnan(aux1) + !std::isnan(aux2) + !std::isnan(aux3) + !std::isnan(aux4);
    stream << nauxchannels;

    if (!std::isnan(aux1))
        stream << aux1;
    if (!std::isnan(aux2))
        stream << aux2;
    if (!std::isnan(aux3))
        stream << aux3;
    if (!std::isnan(aux4))
        stream << aux4;
    if (!std::isnan(aux5))
        stream << aux5;
    if (!std::isnan(aux6))
        stream << aux6;

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

    const uint32_t crc = crc32(0L, (const Bytef *)buffer.data().data(), buffer.data().size());

    // https://stackoverflow.com/questions/40416995/difference-between-crc32-implementations-of-linux-crc32-h-and-zlib-h-in-c
//     crc_final = crc32(crc_initial ^ 0xffffffff, buf, len) ^ 0xffffffff;

//    const uint32_t crc = crc32(0L^ 0xffffffff, (const Bytef *)buffer.data().data(), buffer.data().size())^ 0xffffffff;


    stream << crc;

    qInfo() << "crc: " << crc;

    qInfo() << "buf(" << buffer.data().size() << "): " << buffer.data();

//    for (const uint8_t &b : buffer.data())
    for (int i = 0; i < buffer.data().size(); i++)
        qInfo() << i << ":" << uint8_t(buffer.data()[i]);

    socket->writeDatagram(buffer.data(), this->server, port);
}
