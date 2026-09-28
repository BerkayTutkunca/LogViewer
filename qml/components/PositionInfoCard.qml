import QtQuick

Rectangle {
    id: root

    property string latitude: "39.797434"
    property string longitude: "32.458996"
    property string altitude: "486.1 m"

    implicitWidth: 260
    implicitHeight: 187

    width: implicitWidth
    height: implicitHeight

    color: "#111827"
    radius: 8

    Column {
        anchors.fill: parent
        anchors.margins: 16

        spacing: 10

        Text {
            text: qsTr("Konum")

            color: "#f8fafc"

            font.family: "Inter"
            font.pixelSize: 14
            font.weight: Font.Bold
        }

        InfoRow {
            label: qsTr("Lat")
            value: root.latitude
        }

        InfoRow {
            label: qsTr("Lon")
            value: root.longitude
        }

        InfoRow {
            label: qsTr("Alt")
            value: root.altitude
        }
    }

    component InfoRow: Item {
        property string label: ""
        property string value: ""

        width: 228
        height: 36

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            color: "#ffffff"

            font.family: "Inter"
            font.pixelSize: 13
            font.weight: Font.DemiBold

            text: parent.label
        }

        Text {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            color: "#ffffff"

            font.family: "Inter"
            font.pixelSize: 13
            font.weight: Font.DemiBold

            text: parent.value
        }
    }
}