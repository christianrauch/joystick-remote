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
  manual_control_pub =
      node->create_publisher<geometry_msgs::msg::Wrench>("wrench", 1);
  qInfo() << "manual_control publisher created";
  return true;
}

bool ClientROS::stop() {
  qInfo() << "disconnecting ...";
  manual_control_pub.reset();
  manual_control_pub = nullptr;
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

  if (manual_control_pub) {
    geometry_msgs::msg::Wrench msg;

    msg.force.z = (throttle + 1) / 2; // map from [-1,1] to [0,1];
    msg.torque.x = pitch;
    msg.torque.y = roll;
    msg.torque.z = yaw;

    manual_control_pub->publish(msg);
  }

  return true;
}
