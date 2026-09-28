import QtQuick

Rectangle {
    id: root

    color: "#0f172a"

    Rectangle {
        id: rawDataContent

        anchors.fill: parent

        color: "#172033"

        border.color: "#334155"
        border.width: 1

        Rectangle {
            id: rawDataHeader

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            anchors.topMargin: 20
            anchors.leftMargin: 25
            anchors.rightMargin: 25

            height: 42
            color: "transparent"

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                color: "#f8fafc"

                font.family: "Inter"
                font.pixelSize: 18
                font.weight: Font.DemiBold

                text: qsTr("Ham MAVLink verisi")
            }

            Text {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                color: "#64748b"

                font.family: "Inter"
                font.pixelSize: 12

                text: qsTr("%1 paket").arg(rawDataListView.count)
            }
        }

        Rectangle {
            id: rawDataList

            anchors.top: rawDataHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            anchors.topMargin: 12
            anchors.leftMargin: 25
            anchors.rightMargin: 25
            anchors.bottomMargin: 21

            color: "#111827"
            radius: 8
            clip: true

            ListView {
                id: rawDataListView

                anchors.fill: parent
                anchors.rightMargin: 8

                clip: true

                model: appController.rawDataModel

                delegate: RawDataRow {
                    width: rawDataListView.width

                    timestamp: model.timestamp
                    messageName: model.messageName
                    payload: model.payload

                    active:
                        index ===
                        appController.playbackController.currentIndex
                }
            }
        }
    }

    Connections {
        target: appController.playbackController

        function onCurrentIndexChanged() {
            const currentIndex =
                appController.playbackController.currentIndex

            if (currentIndex < 0 ||
                currentIndex >= rawDataListView.count) {
                return
            }

            rawDataListView.positionViewAtIndex(
                currentIndex,
                ListView.Center
            )
        }
    }
}