import QtQuick

Rectangle {
    id: root

    property string timestamp: "00:01:23.456"
    property string messageName: "GLOBAL_POSITION_INT"
    property string payload: "lat: 39797434, lon: 32458996, alt: 486120"

    property bool active: false

    signal clicked()

    implicitHeight: 56
    height: implicitHeight

    color: root.active ? "#1e293b" : "#111827"

    Rectangle {
        id: activeIndicator

        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: 3

        color: "#3a6ea5"
        visible: root.active
    }

    Row {
        anchors.left: parent.left
        anchors.leftMargin: 35

        anchors.right: parent.right
        anchors.rightMargin: 16

        anchors.verticalCenter: parent.verticalCenter

        height: parent.height

        Item {
            width: 140
            height: parent.height

            Text {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                anchors.rightMargin: 12

                color: "#94a3b8"

                font.family: "Inter"
                font.pixelSize: 12
                font.weight: Font.DemiBold

                text: root.timestamp

                elide: Text.ElideRight
            }
        }

        Item {
            width: 230
            height: parent.height

            Text {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                anchors.rightMargin: 16

                color: root.active ? "#f8fafc" : "#cbd5e1"

                font.family: "Inter"
                font.pixelSize: 13
                font.weight: Font.DemiBold

                text: root.messageName

                elide: Text.ElideRight
            }
        }

        Item {
            width: Math.max(
                0,
                root.width
                    - 35
                    - 16
                    - 140
                    - 230
            )

            height: parent.height

            Text {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                color: "#94a3b8"

                font.family: "Inter"
                font.pixelSize: 12
                font.weight: Font.DemiBold

                text: root.payload

                wrapMode: Text.Wrap
                maximumLineCount: 2
                elide: Text.ElideRight
            }
        }
    }

    TapHandler {
        onTapped: root.clicked()
    }
}