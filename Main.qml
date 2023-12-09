import QtCharts
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Window

ApplicationWindow {
    color: "#282828"
    height: 400
    id: root
    title: qsTr("WordRate")
    visible: true
    width: 300

    onClosing: {
        cancelButton.cancelReading()
    }

    ChartView {
        backgroundColor: "transparent"
        height: parent.height - openButton.height - pauseButton.height - control.height - openButton.anchors.margins
        id: chart
        legend.labelColor: "white"
        visible: false
        width: root.width
        ValuesAxis {
            id: valueAxisX
            labelsColor: "white"
        }
        ValuesAxis {
            id: valueAxisY
            labelsColor: "white"
        }
        BarSeries {
            axisX: BarCategoryAxis {
                categories: ["top"]
                id: category
                labelsColor: "white"
            }
            axisY: valueAxisY
            id: series
        }
    }
    Connections {
        objectName: "metaReceiver"
        target: cppBackend

        function drawChart(msg : list) : string {
            chart.visible = true
            series.clear()
            for (var i = 0; i < msg.length; ++i) {
                var jsonData = JSON.parse(JSON.stringify(msg[i]))
                series.append(jsonData.word, [jsonData.count])
                valueAxisY.max = Math.max(jsonData.count, valueAxisY.max)
                control.value = jsonData.percent
            }
        }

        function showError(msg : string) {
            messageBox.informativeText = msg
            messageBox.open()
        }
    }
    FileDialog {
        id: fileDialog
        objectName: "metaFileDialog"
        title: "Open File"

        signal fileDialogAccepted(path: string)

        onAccepted: {
            chart.visible = false
            control.value = 0
            series.clear();
        }
    }
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.margins: 20
        anchors.right: parent.right
        color: startButtonMouse.hovered ? "#464646" : startButtonStylus.hovered ? "#464646" : "#1d1d1d"
        height: 40
        id: startButton
        radius: 10
        width: parent.width / 2.5
        Text {
            anchors.centerIn: parent
            font.pixelSize: 20
            color: "#ffffff"
            text: "Старт"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                fileDialog.fileDialogAccepted(Qt.resolvedUrl(fileDialog.selectedFile.toString().replace(/^(file:\/{3})/,"")))
                series.clear()
                valueAxisY.max = 0
            }
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
            id: startButtonStylus
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
            id: startButtonMouse
        }
    }
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 20
        color: openButtonMouse.hovered ? "#464646" : openButtonStylus.hovered ? "#464646" : "#1d1d1d"
        height: 40
        id: openButton
        radius: 10
        width: parent.width / 2.5
        Text {
            anchors.centerIn: parent
            color: "#ffffff"
            font.pixelSize: 20
            text: "Открыть"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                fileDialog.open()
            }
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
            id: openButtonStylus
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
            id: openButtonMouse
        }
    }
    Rectangle {
        anchors.margins: 20
        color: pauseButtonMouse.hovered ? "#464646" : pauseButtonStylus.hovered ? "#464646" : "#1d1d1d"
        height: 40
        id: pauseButton
        objectName: "pauseButtonMeta"
        property bool isChecked: false
        radius: 10
        width: parent.width / 2.5
        x: openButton.x
        y: openButton.y - openButton.height - openButton.anchors.margins / 2

        signal pauseRequired(flag: bool)

        Text {
            anchors.centerIn: parent
            color: "#ffffff"
            font.pixelSize: 20
            text: pauseButton.isChecked ? "Пуск" : "Пауза"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                pauseButton.isChecked = !pauseButton.isChecked
                pauseButton.pauseRequired(pauseButton.isChecked)
            }
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
            id: pauseButtonStylus
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
            id: pauseButtonMouse
        }
    }
    Rectangle {
        anchors.margins: 20
        color: cancelButtonMouse.hovered ? "#464646" : cancelButtonStylus.hovered ? "#464646" : "#1d1d1d"
        height: 40
        id: cancelButton
        objectName: "cancelButtonMeta"
        radius: 10
        width: parent.width / 2.5
        x: startButton.x
        y: startButton.y - startButton.height - startButton.anchors.margins / 2

        signal cancelReading()

        Text {
            anchors.centerIn: parent
            color: "#ffffff"
            font.pixelSize: 20
            text: "Отмена"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                chart.visible = false
                control.value = 0
                series.clear();
                cancelButton.cancelReading()
            }
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
            id: cancelButtonStylus
        }
        HoverHandler {
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
            id: cancelButtonMouse
        }
    }
    ProgressBar {
        height: 12
        id: control
        padding: 2
        value: 0
        width: root.width
        y: root.height - height
    }
    MessageDialog {
        buttons: MessageDialog.Ok
        id: messageBox
        text: "Ошибка!"
        onAccepted: {
            Qt.quit()
        }
    }
}
