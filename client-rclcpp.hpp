#pragma once
#include "client.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/wrench.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <qqml.h>
#include <rclcpp/node.hpp>

class ClientROS : public Client {
  Q_OBJECT
  QML_ELEMENT
public:
  explicit ClientROS(QObject *parent = nullptr);

private:
  rclcpp::Node::SharedPtr node = nullptr;
  rclcpp::Publisher<geometry_msgs::msg::Wrench>::SharedPtr pub_wrench = nullptr;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_twist = nullptr;
  rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr pub_quat = nullptr;
  std::vector<std::string> argv_vec;

public slots:
  bool start(const QString &server = {}) override;

  bool stop() override;

  bool send(float roll, float pitch, float yaw, float throttle,
            float aux1 = NAN, float aux2 = NAN, float aux3 = NAN,
            float aux4 = NAN, float aux5 = NAN, float aux6 = NAN) override;

signals:
};
