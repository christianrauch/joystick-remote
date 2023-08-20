import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Joypad {
        id: joypad1

        anchors.bottom: textX.top
        anchors.bottomMargin: 20

        anchors.right: parent.right
        anchors.rightMargin: 10

        anchors.left: parent.left
        anchors.leftMargin: 10

        anchors.top: parent.top
        anchors.topMargin: 10

        alignment: Qt.AlignCenter
    }

    TextField {
        id: textX
        text: joypad1.xAxis

        anchors.bottom: parent.bottom
    }

    TextField {
        id: textY
        text: joypad1.yAxis

        anchors.left: textX.right
        anchors.bottom: parent.bottom
    }
}
