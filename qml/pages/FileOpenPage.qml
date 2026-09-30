import QtQuick
import QtQuick.Dialogs

Rectangle {
    id: root

    property string errorMessage: ""

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

            onSelectFileClicked: {
                root.errorMessage = ""
                fileDialog.open()
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter

            width: 360

            visible: root.errorMessage.length > 0

            text: root.errorMessage

            color: "#ef4444"

            font.family: "Inter"
            font.pixelSize: 13
            font.weight: Font.Normal

            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
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