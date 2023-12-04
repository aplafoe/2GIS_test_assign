import QtQuick
import QtQuick.Window
import QtCharts
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic

Window {
    id: root
    minimumWidth: 300
    minimumHeight: 400
    visible: true
    title: qsTr("WordRate")
    color: "#282828"
    Rectangle {
        id: startButton
        width: parent.width / 2.5
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 20
        height: 40
        color: "#1d1d1d"
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
                console.log("Button clicked!")
            }
        }
    }
    Rectangle {
        id: cancelButton
        width: parent.width / 2.5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        height: 40
        color: "#1d1d1d"
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
    }
    Rectangle {
        id: openButton
        width: parent.width / 2.5
        anchors.margins: 20
        y: startButton.y - height * 1.25
        x: startButton.x
        height: 40
        color: "#1d1d1d"
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
                console.log("Button clicked!")
            }
        }
    }
    Rectangle {
        id: pauseButton
        width: parent.width / 2.5
        anchors.margins: 20
        y: cancelButton.y - height * 1.25
        x: cancelButton.x
        height: 40
        color: "#1d1d1d"
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
    }
    Text {
        id: firstLegendText
        x: (root.width - width) / 2
        y: pauseButton.y - 130
        text: "FirstWord"
        font.pixelSize: 20
        color: "#ffffff"
    }
    Rectangle {
        id: firstLegendIcon
        width: 30
        y: firstLegendText.y
        x: firstLegendText.x - width * 1.2
        height: 30
        color: "#1d1d1d"
        radius: 10
    }
    Text {
        id: secondLegendText
        x: firstLegendText.x
        y: firstLegendText.y + 40
        text: "SecondWord"
        font.pixelSize: 20
        color: "#ffffff"
    }
    Rectangle {
        id: secondLegendIcon
        width: 30
        y: secondLegendText.y
        x: secondLegendText.x - width * 1.2
        height: 30
        color: "#123456"
        radius: 10
    }
    Text {
        id: thirdLegendText
        x: firstLegendText.x
        y: secondLegendText.y + 40
        text: "ThirdWord"
        font.pixelSize: 20
        color: "#ffffff"
    }
    Rectangle {
        id: thirdLegendIcon
        width: 30
        y: thirdLegendText.y
        x: thirdLegendText.x - width * 1.2
        height: 30
        color: "#654321"
        radius: 10
    }
    Rectangle {
        id: previousPage
        width: 20;
        height: 20;
        color: "#777777"
        radius: 10
        x: openButton.x
        y: secondLegendIcon.y
    }
    Rectangle {
        id: nextPage
        width: 20;
        height: 20;
        color: "#777777"
        radius: 10
        x: pauseButton.x + pauseButton.width - width
        y: secondLegendIcon.y
    }
    ProgressBar {
        id: control
        value: 0.5
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
    Rectangle {
        width: 100;
        height: 100;
        color: "red"
    }
}
