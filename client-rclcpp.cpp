#include "client-rclcpp.hpp"
#include <iostream>
#include <QCoreApplication>


ClientROS::ClientROS(QObject *parent)
    : Client{parent}
{
    const QStringList args = QCoreApplication::arguments();
    argv_vec.resize(args.size());
    char const * *argv = new char const *[args.size()+1];
    for (int i = 0; i < args.size(); ++i) {
        argv_vec[i] = args.at(i).toStdString();
        argv[i] = argv_vec[i].c_str();
    }
    argv[args.size()] = nullptr;

    rclcpp::init(args.size(), argv);
}

bool
ClientROS::start(const QString &server)
{
    node = rclcpp::Node::make_shared("joystick_remote_client");
    manual_control_pub = node->create_publisher<mavros_msgs::msg::ManualControl>("manual_control", 1);
    qInfo() << "manual_control publisher created";
    return true;
}

bool
ClientROS::stop()
{
    qInfo() << "disconnecting ...";
    manual_control_pub.reset();
    manual_control_pub = nullptr;
    node.reset();
    node = nullptr;
    return true;
}

bool
ClientROS::send(float roll, float pitch, float yaw, float throttle,
                float aux1, float aux2, float aux3,
                float aux4, float aux5, float aux6)
{
    // origin of the joystick widget is in the top left
    // invert y axes
    throttle *= -1;
    pitch *= -1;

    // qInfo() << "TAER: " << throttle << ", " << roll << ", " << pitch << ", " << yaw << ", " << aux1 << ", " << aux2;

    if (manual_control_pub) {
        // https://mavlink.io/en/services/manual_control.html
        mavros_msgs::msg::ManualControl msg;

        msg.header.stamp = node->now();
        msg.header.frame_id = "manual_control";
        msg.x = pitch;
        msg.y = roll;
        msg.z = (throttle+1)/2;  // map from [-1,1] to [0,1];
        msg.r = yaw;
        msg.aux1 = aux1;
        msg.aux2 = aux2;
        msg.aux3 = aux3;
        msg.aux4 = aux4;

        manual_control_pub->publish(msg);
    }

    return true;
}
