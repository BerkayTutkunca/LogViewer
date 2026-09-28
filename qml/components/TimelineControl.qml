import QtQuick

Rectangle {
    id: root

    property string currentTime: "00:00.000"
    property string duration: "00:00.000"
    property int activePacket: 0

    property real playbackPosition: 0.0

    signal positionChangedByUser(real position)

    implicitHeight: 110
    height: implicitHeight

    color: "#111827"

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 1
        color: "#334155"
    }

    Text {
        id: currentTimeText

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 17

        color: "#94a3b8"

        font.family: "Inter"
        font.pixelSize: 12
        font.weight: Font.DemiBold

        text: root.currentTime
    }

    Text {
        id: durationText

        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 17

        color: "#94a3b8"

        font.family: "Inter"
        font.pixelSize: 12
        font.weight: Font.DemiBold

        text: root.duration
    }

    Item {
        id: sliderArea

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: 20
        anchors.rightMargin: 20

        anchors.top: currentTimeText.bottom
        anchors.topMargin: 8

        height: 28

        Rectangle {
            id: timelineTrack

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            height: 5
            radius: height / 2

            color: "#334155"
        }

        Rectangle {
            id: timelineProgress

            anchors.left: timelineTrack.left
            anchors.verticalCenter: timelineTrack.verticalCenter

            width: timelineTrack.width
                   * Math.max(
                       0.0,
                       Math.min(1.0, root.playbackPosition)
                   )

            height: timelineTrack.height
            radius: height / 2

            color: "#3a6ea5"
        }

        Rectangle {
            id: timelineHandle

            width: 15
            height: 15
            radius: width / 2

            x: timelineTrack.x
               + timelineTrack.width
               * Math.max(
                   0.0,
                   Math.min(1.0, root.playbackPosition)
               )
               - width / 2

            anchors.verticalCenter: timelineTrack.verticalCenter

            color: "#f8fafc"

            border.width: 2
            border.color: "#3a6ea5"

            scale: sliderMouseArea.pressed ? 1.15 : 1.0

            Behavior on scale {
                NumberAnimation {
                    duration: 80
                }
            }
        }

        MouseArea {
            id: sliderMouseArea

            anchors.fill: parent

            hoverEnabled: true

            cursorShape: Qt.PointingHandCursor

            function updatePosition(mouseX) {
                const position =
                    Math.max(
                        0.0,
                        Math.min(
                            1.0,
                            mouseX / timelineTrack.width
                        )
                    )

                root.positionChangedByUser(position)
            }

            onPressed: function(mouse) {
                updatePosition(mouse.x)
            }

            onPositionChanged: function(mouse) {
                if (pressed) {
                    updatePosition(mouse.x)
                }
            }
        }
    }

    Text {
        anchors.right: parent.right
        anchors.rightMargin: 20

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        color: "#64748b"

        font.family: "Inter"
        font.pixelSize: 12

        text: qsTr("Aktif paket: %1")
            .arg(root.activePacket)
    }
}