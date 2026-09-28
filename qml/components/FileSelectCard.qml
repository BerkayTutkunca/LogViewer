import QtQuick

Rectangle {
    id: root

    property alias titleText: fileType.text
    property alias descriptionText: description.text
    property alias supportedFormatText: supportedFormat.text
    property alias buttonText: primaryButton.text

    property bool showFileType: true

    signal selectFileClicked()

    implicitWidth: 360
    implicitHeight: 193

    width: implicitWidth
    height: implicitHeight

    color: "#111827"
    radius: 12

    border.color: "#334155"
    border.width: 1

    Column {
        anchors.centerIn: parent
        spacing: 12

        Text {
            id: fileType

            anchors.horizontalCenter: parent.horizontalCenter

            visible: root.showFileType

            color: "#94a3b8"
            font.family: "Inter"
            font.pixelSize: 28
            font.weight: Font.Bold

            text: qsTr("TLOG")
        }

        Text {
            id: description

            anchors.horizontalCenter: parent.horizontalCenter

            color: "#cbd5e1"
            font.family: "Inter"
            font.pixelSize: 14
            font.weight: Font.Normal

            text: qsTr("Bir .tlog uçuş kaydı seçin")
        }

        PrimaryButton {
            id: primaryButton

            anchors.horizontalCenter: parent.horizontalCenter

            text: qsTr("Dosya seç")

            onClicked: root.selectFileClicked()
        }

        Text {
            id: supportedFormat

            anchors.horizontalCenter: parent.horizontalCenter

            color: "#64748b"
            font.family: "Inter"
            font.pixelSize: 12
            font.weight: Font.Normal

            text: qsTr("Desteklenen format: .tlog")
        }
    }
}