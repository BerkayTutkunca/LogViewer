import QtQuick

Rectangle {
    id: root

    property string text: qsTr("Harita")
    property bool active: false

    signal clicked()

    implicitWidth: 140
    implicitHeight: 44

    width: implicitWidth
    height: implicitHeight

    color: root.active ? "#1e293b" : "#111827"

    Text {
        id: label

        anchors.centerIn: parent

        color: root.active ? "#f8fafc" : "#94a3b8"

        font.family: "Inter"
        font.pixelSize: 14
        font.weight: Font.DemiBold

        text: root.text
    }

    Rectangle {
        id: activeIndicator

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 3

        color: "#3a6ea5"
        visible: root.active
    }

    TapHandler {
        onTapped: root.clicked()
    }
}