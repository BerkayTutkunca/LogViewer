import QtQuick
import QtQuick.Dialogs

Rectangle {
    id: root

    signal fileSelected(url fileUrl)

    color: "#0f172a"

    Column {
        anchors.centerIn: parent
        spacing: 24

        Text {
            anchors.horizontalCenter: parent.horizontalCenter

            color: "#f8fafc"

            font.family: "Inter"
            font.pixelSize: 48
            font.weight: Font.Bold

            text: qsTr("LogViewer")
        }

        FileSelectCard {
            anchors.horizontalCenter: parent.horizontalCenter

            showFileType: false

            onSelectFileClicked: fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog

        title: qsTr("TLOG dosyası seç")
        fileMode: FileDialog.OpenFile

        nameFilters: [
            qsTr("TLOG dosyaları (*.tlog)")
        ]

        onAccepted: {
            root.fileSelected(selectedFile)
        }
    }
}