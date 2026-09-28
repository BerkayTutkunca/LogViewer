import QtQuick

Rectangle {
    id: root

    signal rawDataRequested()

    color: "#0f172a"

    Rectangle {
        id: appHeader

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 44
        color: "#111827"

        Text {
            anchors.centerIn: parent

            color: "#f8fafc"

            font.family: "Inter"
            font.pixelSize: 20
            font.weight: Font.DemiBold

            text: qsTr("LogViewer")
        }
    }

    Rectangle {
        id: tabBar

        anchors.top: appHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        height: 44
        color: "#111827"

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 24

            height: parent.height

            TabButton {
                text: qsTr("Harita")
                active: true
            }

            TabButton {
                text: qsTr("Ham Veri")
                active: false

                onClicked: root.rawDataRequested()
            }
        }
    }

    Rectangle {
        id: mapContent

        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        color: "#1e293b"

        Text {
            anchors.centerIn: parent

            color: "#ffffff"

            font.family: "Inter"
            font.pixelSize: 36
            font.weight: Font.DemiBold

            text: qsTr("Buraya 2D Harita gelecek")
        }

        PositionInfoCard {
            id: positionInfoCard

            anchors.top: parent.top
            anchors.right: parent.right

            anchors.topMargin: 24
            anchors.rightMargin: 24

            latitude:
                appController.playbackController.hasPosition
                ? appController.playbackController.latitude.toFixed(7)
                : "-"

            longitude:
                appController.playbackController.hasPosition
                ? appController.playbackController.longitude.toFixed(7)
                : "-"

            altitude:
                appController.playbackController.hasPosition
                ? appController.playbackController.altitude.toFixed(1) + " m"
                : "-"
        }
    }
}