import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
//import QtQuick.Controls.Basic


Window {
//    width: 640
//    height: 480
    width: 1000
    height: 600
    visible: true
    title: qsTr("Remote Control")

    Client {
        id: client
    }

    Timer {
        interval: 100; running: true; repeat: true
//        onTriggered: client.send(joypad1.xAxis, joypad1.yAxis, joypad2.xAxis, joypad2.yAxis)
        // MODE 2 TAER
        onTriggered: client.send_channels(joypad1.yAxis, joypad2.xAxis, joypad2.yAxis, joypad1.xAxis,
                                          aux1.value, aux2.value)
    }

    ColumnLayout {
//        anchors.fill: parent
//        width: 1500
        width: parent.width
//        Layout.preferredwidth: 500
        height: parent.height

        RowLayout {
            id: row_connect
//            anchors.fill: parent
//            width: parent.width
//            Layout.alignment: Qt.AlignCenter
            width: parent.width

//            Item { Layout.fillHeight: true }
//            Item { Layout.fillWidth: true }

//            Item{
                Text {
                    id: text1
//                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    text: qsTr("Connect")
                }
//            }

    //        Item{
                Frame {
                    id: frame
                    Layout.alignment: Qt.AlignCenter
                    TextInput {
                        id: server
//                        Layout.fillWidth: true
//                        Layout.fillHeight: true
                        inputMask: "999.999.999.999"
                        height: 20
                        text: qsTr("127.0.0.1")
                    }
                }
    //        }

    //        Item{
                Switch {
                    id: switch1
//                    Layout.alignment: Qt.AlignRight
//                    anchors.right: parent.right
//                    Layout.alignment: Qt.AlignRight
//                    Layout.alignment: Qt.AlignRight
//                    text: qsTr("connect")
                    onToggled: client.connect(server.text, switch1.checked)
                }
    //        }
        }

        RowLayout {
            id: row_joypads
//            anchors.fill: parent
            Layout.fillWidth: true
            Layout.fillHeight: true
//            width: parent.width
//            height: parent.height

            Joypad {
                id: joypad1
                verticalAnimation: false
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

//            Item {
            Slider {
                id: aux1
                Layout.fillHeight: true
//                from: 0
//                value: 0
//                to: 1000
                orientation: Qt.Vertical
            }
            Slider {
                id: aux2
                Layout.fillHeight: true
//                from: 0
//                value: 0
//                to: 1000
                orientation: Qt.Vertical
            }
//            }

            Joypad {
                id: joypad2
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

        }

        RowLayout {
            id: row_status
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            anchors.fill: parent
            width: parent.width
//            width: parent.width
//            height: parent.height

//            Grid{
                TextField {
                    id: textX1
//                    Layout.fillWidth: true
//                    Layout.alignment: Qt.AlignLeft
                    readOnly: true
                    text: joypad1.xAxis
                    maximumLength: 4
                }
                TextField {
                    id: textY1
//                    Layout.fillWidth: true
//                    Layout.alignment: Qt.AlignRight
                    readOnly: true
                    text: joypad1.yAxis
                    maximumLength: 4
                }
//            }

            Item { Layout.fillWidth: true }

            Text {
                id: textasas
                text: qsTr("values")
            }

            Item { Layout.fillWidth: true }

//            Grid{
//                Layout.alignment: Qt.AlignRight

                TextField {
                    id: textY2
//                    Layout.fillWidth: true
//                    Layout.alignment: Qt.AlignLeft
                    readOnly: true
                    text: joypad2.yAxis
                    maximumLength: 4
                }
                TextField {
                    id: textX2
//                    Layout.fillWidth: true
//                    Layout.alignment: Qt.AlignRight
                    readOnly: true
                    text: joypad2.xAxis
                    maximumLength: 4
                }
//            }
        }
    }
}
