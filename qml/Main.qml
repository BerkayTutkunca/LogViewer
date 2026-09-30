import QtQuick
import QtQuick.Window

Window {
    id: root

    width: 1280
    height: 800

    minimumWidth: 1000
    minimumHeight: 650

    visible: true

    title: qsTr("LogViewer")
    color: "#0f172a"

    readonly property int fileOpenPage: 0
    readonly property int mapPage: 1
    readonly property int rawDataPage: 2

    property int currentPage: fileOpenPage

    FileOpenPage {
        id: fileOpenPageItem

        anchors.fill: parent

        visible:
            root.currentPage === root.fileOpenPage

        onFileSelected: function(fileUrl) {
            errorMessage = ""
            appController.loadLog(fileUrl)
        }
    }

    Rectangle {
        id: appHeader

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 44

        visible:
            root.currentPage !== root.fileOpenPage

        color: "#111827"

        Text {
            anchors.centerIn: parent

            text: qsTr("LogViewer")

            color: "#f8fafc"

            font.family: "Inter"
            font.pixelSize: 20
            font.weight: Font.DemiBold
        }
    }

    Rectangle {
        id: tabBar

        anchors.top: appHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        height: 44

        visible:
            root.currentPage !== root.fileOpenPage

        color: "#111827"

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 24

            height: parent.height

            TabButton {
                text: qsTr("Harita")

                active:
                    root.currentPage === root.mapPage

                onClicked: {
                    root.currentPage = root.mapPage
                }
            }

            TabButton {
                text: qsTr("Ham Veri")

                active:
                    root.currentPage === root.rawDataPage

                onClicked: {
                    root.currentPage = root.rawDataPage
                }
            }
        }
    }

    Item {
        id: viewerContent

        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: timelineControl.top

        visible:
            root.currentPage !== root.fileOpenPage

        MapPage {
            id: mapPageItem

            anchors.fill: parent

            visible:
                root.currentPage === root.mapPage
        }

        RawDataPage {
            id: rawDataPageItem

            anchors.fill: parent

            visible:
                root.currentPage === root.rawDataPage
        }
    }

    TimelineControl {
        id: timelineControl

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 110

        visible:
            root.currentPage !== root.fileOpenPage

        currentTime:
            appController.playbackController.currentTime

        duration:
            appController.playbackController.duration

        activePacket:
            appController.playbackController.activePacket

        playbackPosition:
            appController.playbackController.position

        onPositionChangedByUser: function(position) {
            appController.playbackController.seek(position)
        }
    }

    Connections {
        target: appController

        function onLoadStarted() {
            fileOpenPageItem.errorMessage = ""
        }

        function onLoadSucceeded() {
            fileOpenPageItem.errorMessage = ""
            root.currentPage = root.mapPage
        }

        function onLoadFailed(errorMessage) {
            fileOpenPageItem.errorMessage = errorMessage
        }
    }
}
