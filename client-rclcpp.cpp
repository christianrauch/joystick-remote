#include "client-rclcpp.hpp"
#include <QCoreApplication>
#include <iostream>

ClientROS::ClientROS(QObject *parent) : Client{parent} {
  const QStringList args = QCoreApplication::arguments();
  argv_vec.resize(args.size());
  char const **argv = new char const *[args.size() + 1];
  for (int i = 0; i < args.size(); ++i) {
    argv_vec[i] = args.at(i).toStdString();
    argv[i] = argv_vec[i].c_str();
  }
  argv[args.size()] = nullptr;

  rclcpp::init(args.size(), argv);

  start();
}

bool ClientROS::start(const QString &server) {
  node = rclcpp::Node::make_shared("joystick_remote_client");
  const bool wrench = node->declare_parameter("use_wrench", true);
  if (wrench)
    pub_wrench =
        node->create_publisher<geometry_msgs::msg::Wrench>("wrench", 1);
  else
    pub_twist = node->create_publisher<geometry_msgs::msg::Twist>("twist", 1);
  qInfo() << "manual_control publisher created";
  return true;
}

bool ClientROS::stop() {
  qInfo() << "disconnecting ...";
  pub_wrench.reset();
  pub_wrench = nullptr;
  pub_twist.reset();
  pub_twist = nullptr;
  node.reset();
  node = nullptr;
  return true;
}

bool ClientROS::send(float roll, float pitch, float yaw, float throttle,
                     float aux1, float aux2, float aux3, float aux4, float aux5,
                     float aux6) {
  // origin of the joystick widget is in the top left
  // invert y axes
  throttle *= -1;
  pitch *= -1;

  // qInfo() << "TAER: " << throttle << ", " << roll << ", " << pitch << ", " <<
  // yaw << ", " << aux1 << ", " << aux2;

  if (pub_wrench) {
    geometry_msgs::msg::Wrench msg;

    msg.force.z = (throttle + 1) / 2; // map from [-1,1] to [0,1];
    msg.torque.x = pitch;
    msg.torque.y = roll;
    msg.torque.z = yaw;

    pub_wrench->publish(msg);
  } else if (pub_twist) {
    geometry_msgs::msg::Twist msg;

    msg.linear.z = (throttle + 1) / 2; // map from [-1,1] to [0,1];
    msg.angular.x = pitch;
    msg.angular.y = roll;
    msg.angular.z = yaw;

    pub_twist->publish(msg);
  }

  return true;
}
