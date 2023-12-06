import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import QtCharts

Window {
    id: root
    minimumWidth: 300
    minimumHeight: 300
    visible: true
    title: qsTr("WordRate")
    color: "#282828"
    ChartView {
        visible: false
        height: parent.height - openButton.height - startButton.height - control.height - openButton.anchors.margins;
        width: root.width
        backgroundColor: "transparent"
        id: view
        legend.labelColor: "white"
        ValuesAxis {
            labelsColor: "white"
            id: valueAxis
                    min: 0
        }
        ValuesAxis {
            labelsColor: "white"
            id: valueAxisY
            min: 0
            max: 0
        }
            BarSeries {
                axisX: BarCategoryAxis {
                    id: cate
                    categories: ["top15"]
                    labelsColor: "white"
                }
                axisY: valueAxisY
                   id: mySeries
               }
        }
    Connections {
        objectName: "metaReceiver"
        target: cppBackend
        function myQmlFunction(msg : list) : string {
            view.visible = true;
            mySeries.clear();
            var categories = ["top"];
            for (var i = 0; i < msg.length; ++i) {
                var jsonData = JSON.parse(JSON.stringify(msg[i]));
                mySeries.append(jsonData.word, [jsonData.count]);
                valueAxisY.max = Math.max(jsonData.count, valueAxisY.max)
                control.value = jsonData.percent
            }
            cate.categories = categories;
        }
    }
    FileDialog {
            objectName: "metaFileDialog"
            id: fileDialog
            title: "Open File"
            signal fileDialogAccepted(path: string)
            onAccepted: {
                mySeries.clear()
                fileDialog.fileDialogAccepted(Qt.resolvedUrl(fileDialog.selectedFile.toString().replace(/^(file:\/{3})/,"")))
            }
        }
    Rectangle {
        id: startButton
        width: parent.width / 2.5
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 20
        height: 40
        color: mouse.hovered ? "#464646" : "#1d1d1d"
        radius: 10
        Text {
            anchors.centerIn: parent
            text: "Старт"
            font.pixelSize: 20
            color: "#ffffff"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                cppBackend.clicked()
            }
        }
        HoverHandler {
            id: stylus
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
        }
        HoverHandler {
            id: mouse
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
        }
    }
    Rectangle {
        id: cancelButton
        width: parent.width / 2.5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        height: 40
        color: mouse1.hovered ? "#464646" : "#1d1d1d"
        radius: 10
        Text {
            anchors.centerIn: parent
            text: "Отмена"
            font.pixelSize: 20
            color: "#ffffff"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Button clicked!")
            }
        }
        HoverHandler {
            id: stylus1
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
        }
        HoverHandler {
            id: mouse1
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
        }
    }
    Rectangle {
        id: openButton
        width: parent.width / 2.5
        anchors.margins: 20
        y: startButton.y - height * 1.25
        x: startButton.x
        height: 40
        color: mouse2.hovered ? "#464646" : "#1d1d1d"
        radius: 10
        Text {
            anchors.centerIn: parent
            text: "Открыть"
            font.pixelSize: 20
            color: "#ffffff"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                fileDialog.open()
            }
        }
        HoverHandler {
            id: stylus2
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
        }
        HoverHandler {
            id: mouse2
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
        }
    }
    Rectangle {
        id: pauseButton
        width: parent.width / 2.5
        anchors.margins: 20
        y: cancelButton.y - height * 1.25
        x: cancelButton.x
        height: 40
        color: mouse4.hovered ? "#464646" : "#1d1d1d"
        radius: 10
        Text {
            anchors.centerIn: parent
            text: "Пауза"
            font.pixelSize: 20
            color: "#ffffff"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Button clicked!")
            }
        }
        HoverHandler {
            id: stylus4
            acceptedDevices: PointerDevice.Stylus
            cursorShape: Qt.CrossCursor
        }
        HoverHandler {
            id: mouse4
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
        }
    }
    ProgressBar {
        id: control
        value: 0
        padding: 2
        height: 12
        width: root.width
        y: root.height - height
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 6
            color: "transparent"
            radius: 3
        }
        contentItem: Item {
                implicitWidth: 200
                implicitHeight: 4

                Rectangle {
                    width: control.visualPosition * parent.width
                    height: parent.height / 2
                    radius: 2
                    color: "#61FF3A"
                }
        }
    }
}
