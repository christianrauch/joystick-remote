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
  pub_quat = node->create_publisher<geometry_msgs::msg::Quaternion>("orientation", 1);
  pub_att = node->create_publisher<geometry_msgs::msg::Vector3>("attitude", 1);
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

  geometry_msgs::msg::Vector3 msg_att;

  msg_att.x = roll * 4;
  msg_att.y = pitch * 4;
  msg_att.z = yaw * -1;

  pub_att->publish(msg_att);

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

  geometry_msgs::msg::Quaternion msg_quat;

  // roll *= 4;
  // pitch *= 4;
  // yaw *= 4;

  // Convert roll, pitch, yaw (in radians) to quaternion
  double cy = cos(yaw * 0.5);
  double sy = sin(yaw * 0.5);
  double cp = cos(pitch * 0.5);
  double sp = sin(pitch * 0.5);
  double cr = cos(roll * 0.5);
  double sr = sin(roll * 0.5);
  msg_quat.w = cr * cp * cy + sr * sp * sy;
  msg_quat.x = sr * cp * cy - cr * sp * sy;
  msg_quat.y = cr * sp * cy + sr * cp * sy;
  msg_quat.z = cr * cp * sy - sr * sp * cy;

  pub_quat->publish(msg_quat);

  return true;
}
