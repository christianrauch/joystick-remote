#pragma once
#include "client.hpp"
#include <qqml.h>
#include <rclcpp/node.hpp>
#include <mavros_msgs/msg/manual_control.hpp>


class ClientROS : public Client
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ClientROS(QObject *parent = nullptr);

private:
    rclcpp::Node::SharedPtr node = nullptr;
    rclcpp::Publisher<mavros_msgs::msg::ManualControl>::SharedPtr manual_control_pub = nullptr;
    std::vector<std::string> argv_vec;

public slots:
    bool start(const QString &server) override;

    bool stop() override;

    bool
    send(float roll, float pitch, float yaw, float throttle,
         float aux1 = NAN, float aux2 = NAN, float aux3 = NAN,
         float aux4 = NAN, float aux5 = NAN, float aux6 = NAN)  override;

signals:

};
