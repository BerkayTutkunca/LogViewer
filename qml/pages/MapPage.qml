import QtQuick
import QtLocation
import QtPositioning

Rectangle {
    id: root

    property bool followVehicle: true

    color: "#0f172a"

    Rectangle {
        id: mapContent

        anchors.fill: parent

        color: "#1e293b"
        clip: true

        Plugin {
            id: mapPlugin

            name: "osm"
        }

        Map {
            id: map

            anchors.fill: parent

            plugin: mapPlugin

            center: QtPositioning.coordinate(39.0, 35.0)
            zoomLevel: 5

            WheelHandler {
                id: wheelHandler

                acceptedDevices: PointerDevice.Mouse
                rotationScale: 1 / 120

                property: "zoomLevel"
            }

            DragHandler {
                id: dragHandler

                target: null

                onActiveChanged: {
                    if (active) {
                        root.followVehicle = false
                    }
                }

                onTranslationChanged: function(delta) {
                    map.pan(
                        -delta.x,
                        -delta.y
                    )
                }
            }

            MapPolyline {
                id: flightPath

                z: 1

                line.width: 4
                line.color: "#3a6ea5"

                path:
                    appController.playbackController.traveledPath
            }

            MapQuickItem {
                id: vehicleMarker

                z: 2

                visible:
                    appController.playbackController.hasPosition

                coordinate: QtPositioning.coordinate(
                    appController.playbackController.latitude,
                    appController.playbackController.longitude
                )

                anchorPoint.x: markerItem.width / 2
                anchorPoint.y: markerItem.height / 2

                sourceItem: Item {
                    id: markerItem

                    width: 48
                    height: 48

                    Rectangle {
                        anchors.centerIn: parent

                        width: 36
                        height: 36
                        radius: 18

                        color: "#ffffff"
                        opacity: 0.75
                    }

                    Image {
                        id: droneIcon

                        anchors.centerIn: parent

                        width: 42
                        height: 42

                        source: "qrc:/qt/qml/LogViewer/drone.svg"

                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        mipmap: true

                        rotation:
                            appController.playbackController.heading
                    }
                }
            }
        }

        PositionInfoCard {
            id: positionInfoCard

            z: 10

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

        Rectangle {
            id: recenterButton

            z: 10

            visible:
                !root.followVehicle &&
                appController.playbackController.hasPosition

            anchors.right: parent.right
            anchors.bottom: parent.bottom

            anchors.rightMargin: 24
            anchors.bottomMargin: 24

            width: 110
            height: 36
            radius: 6

            color: "#111827"

            border.width: 1
            border.color: "#334155"

            Text {
                anchors.centerIn: parent

                text: qsTr("Konuma dön")

                color: "#f8fafc"

                font.family: "Inter"
                font.pixelSize: 12
                font.weight: Font.DemiBold
            }

            TapHandler {
                onTapped: {
                    root.followVehicle = true

                    map.center = QtPositioning.coordinate(
                        appController.playbackController.latitude,
                        appController.playbackController.longitude
                    )
                }
            }
        }
    }

    Connections {
        target: appController.playbackController

        function onRouteChanged() {
            if (!appController.playbackController.hasRoute) {
                return
            }

            root.followVehicle = true

            map.center =
                appController.playbackController.initialCoordinate

            map.zoomLevel = 16
        }

        function onCurrentPositionChanged() {
            if (!root.followVehicle) {
                return
            }

            if (!appController.playbackController.hasPosition) {
                return
            }

            map.center = QtPositioning.coordinate(
                appController.playbackController.latitude,
                appController.playbackController.longitude
            )
        }
    }
}