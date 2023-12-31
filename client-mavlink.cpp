#include "client-mavlink.hpp"
#include <iostream>
// #include <QBuffer>
// #include <zlib.h>

#include <future>


using std::chrono::seconds;
using namespace mavsdk;

std::shared_ptr<System> get_system(Mavsdk& mavsdk)
{
    std::cout << "Waiting to discover system...\n";
    auto prom = std::promise<std::shared_ptr<System>>{};
    auto fut = prom.get_future();

    // We wait for new systems to be discovered, once we find one that has an
    // autopilot, we decide to use it.
    mavsdk.subscribe_on_new_system([&mavsdk, &prom]() {
        auto system = mavsdk.systems().back();

        if (system->has_autopilot()) {
            std::cout << "Discovered autopilot\n";

            // Unsubscribe again as we only want to find one system.
            mavsdk.subscribe_on_new_system(nullptr);
            prom.set_value(system);
        }
    });

    // We usually receive heartbeats at 1Hz, therefore we should find a
    // system after around 3 seconds max, surely.
    if (fut.wait_for(seconds(3)) == std::future_status::timeout) {
        std::cerr << "No autopilot found.\n";
        return {};
    }

    // Get discovered system now.
    return fut.get();
}


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

bool
ClientMavLink::start(const QString &server)
{
    // "udp://" + server + ":14540"

    // const mavsdk::ConnectionResult connection_result =
    //     mavsdk.add_any_connection(("udp://" + server + ":14540").toStdString());

    const mavsdk::ConnectionResult connection_result = mavsdk.add_udp_connection(server.toStdString());

    // const mavsdk::ConnectionResult connection_result = mavsdk.add_udp_connection();

    if (connection_result != mavsdk::ConnectionResult::Success)
        return false;

    std::cout << ", " << connection_result << std::endl;

    // const std::vector<std::shared_ptr<mavsdk::System>> sys = mavsdk.systems();

    // if (sys.empty())
    //     return false;

    std::shared_ptr<mavsdk::System> sys = get_system(mavsdk);

    if (!sys)
        return false;

    manual_control = std::make_shared<mavsdk::ManualControl>(sys);

    action = std::make_shared<mavsdk::Action>(sys);

    qInfo() << "connected: " << server;

    if (action->arm() != Action::Result::Success) {
        std::cerr << "arming failed: " << std::endl;
        return false;
    }

    if (action->takeoff() != Action::Result::Success) {
        std::cerr << "takeoff failed: " << std::endl;
        return false;
    }

    return true;

}

bool
ClientMavLink::stop()
{
    qInfo() << "disconnecting ...";
    action->land();
    action->disarm();
    //        while (socket->hasPendingDatagrams());
    // socket->close();
    qInfo() << "disconnected";
    // delete socket;
    // socket = nullptr;

    return true;
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

bool
ClientMavLink::send(float roll, float pitch, float yaw, float throttle,
                float aux1, float aux2, float aux3,
                float aux4, float aux5, float aux6)
{
//     if (!socket) {
// //        qInfo() << "not connected";
//         return;
//     }

    // if (connection_result != mavsdk::ConnectionResult::Success)
    //     return false;

    if (!manual_control)
        return false;

//    qInfo() << "send " << throttle;
//    qFatal() << "send";

   // throttle = (-throttle + 1) / 2;
   // yaw = (1 + yaw ) / 2;
   // pitch = (-pitch + 1) / 2;
   // roll = (1 + roll ) / 2;

    if (throttle != 0)
        throttle *= -1;
    if (pitch != 0)
        pitch *= -1;

    qInfo() << "TAER: " << throttle << ", " << roll << ", " << pitch << ", " << yaw << ", " << aux1 << ", " << aux2;

    if (manual_control)
        manual_control->set_manual_control_input(pitch, roll, throttle, yaw);

    return true;

// //    QByteArray data;
//     QBuffer buffer;
//     buffer.open(QIODevice::WriteOnly);
//     QDataStream stream(&buffer);
//     // network byte order
//     stream.setByteOrder(QDataStream::BigEndian);
//     stream.setFloatingPointPrecision(QDataStream::FloatingPointPrecision::SinglePrecision);

// //    stream << uint8_t(0x0F) << nchannels << throttle << roll << pitch << yaw;
//     stream << uint8_t(0x0F) << roll << pitch << yaw << throttle;
// //    stream << uint8_t(1) << aux1;
// //    stream << uint8_t(1);
// //    stream << 1.0f;

//     const uint8_t nauxchannels = !std::isnan(aux1) + !std::isnan(aux2) + !std::isnan(aux3) + !std::isnan(aux4);
//     stream << nauxchannels;

//     if (!std::isnan(aux1))
//         stream << aux1;
//     if (!std::isnan(aux2))
//         stream << aux2;
//     if (!std::isnan(aux3))
//         stream << aux3;
//     if (!std::isnan(aux4))
//         stream << aux4;
//     if (!std::isnan(aux5))
//         stream << aux5;
//     if (!std::isnan(aux6))
//         stream << aux6;

// //    QByteArray data;
// //    data.append(nchannels);
// //    data.append(reinterpret_cast<const char *>(&throttle), sizeof(float));
// //    data.append(reinterpret_cast<const char *>(&roll), sizeof(float));
// //    data.append(reinterpret_cast<const char *>(&pitch), sizeof(float));
// //    data.append(reinterpret_cast<const char *>(&yaw), sizeof(float));
// //    if (!std::isnan(aux1))
// //        data.append(reinterpret_cast<const char *>(&aux1), sizeof(float));
// //    if (!std::isnan(aux2))
// //        data.append(reinterpret_cast<const char *>(&aux2), sizeof(float));
// //    if (!std::isnan(aux3))
// //        data.append(reinterpret_cast<const char *>(&aux3), sizeof(float));
// //    if (!std::isnan(aux4))
// //        data.append(reinterpret_cast<const char *>(&aux4), sizeof(float));

//     const uint32_t crc = crc32(0L, (const Bytef *)buffer.data().data(), buffer.data().size());

//     // https://stackoverflow.com/questions/40416995/difference-between-crc32-implementations-of-linux-crc32-h-and-zlib-h-in-c
// //     crc_final = crc32(crc_initial ^ 0xffffffff, buf, len) ^ 0xffffffff;

// //    const uint32_t crc = crc32(0L^ 0xffffffff, (const Bytef *)buffer.data().data(), buffer.data().size())^ 0xffffffff;


//     stream << crc;

//     qInfo() << "crc: " << crc;

//     qInfo() << "buf(" << buffer.data().size() << "): " << buffer.data();

// //    for (const uint8_t &b : buffer.data())
//     for (int i = 0; i < buffer.data().size(); i++)
//         qInfo() << i << ":" << uint8_t(buffer.data()[i]);

//     // socket->writeDatagram(buffer.data(), this->server, port);
//     return true;
}
