import QtQuick

Rectangle {
    id: root

    property alias text: label.text

    signal clicked()

    implicitWidth: 102
    implicitHeight: 41

    width: implicitWidth
    height: implicitHeight

    color: "#3a6ea5"

    Text {
        id: label

        anchors.centerIn: parent

        color: "#ffffff"

        font.family: "Inter"
        font.pixelSize: 14
        font.weight: Font.DemiBold

        text: qsTr("Dosya seç")
    }

    TapHandler {
        onTapped: root.clicked()
    }
}