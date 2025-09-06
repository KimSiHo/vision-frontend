import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    property real iconScale: 0.6
    property color borderColor: "gray"
    property int borderWidth: 2

    GridLayout {
        id: grid
        anchors.fill: parent
        rows: 2
        columns: 2
        rowSpacing: 0
        columnSpacing: 0

        ToolButton {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0
            background: Rectangle { color: "transparent" }
            contentItem: Item {
                anchors.fill: parent
                Image {
                    source: "qrc:/icons/home/map.png"
                    anchors.centerIn: parent
                    width: Math.min(parent.width, parent.height) * iconScale
                    height: width
                    fillMode: Image.PreserveAspectFit
                }
            }
            onClicked: stackView.push("qrc:/qml/pages/map.qml")
        }

        ToolButton {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0
            background: Rectangle { color: "transparent" }
            contentItem: Item {
                anchors.fill: parent
                Image {
                    source: "qrc:/icons/home/camera.png"
                    anchors.centerIn: parent
                    width: Math.min(parent.width, parent.height) * iconScale
                    height: width
                    fillMode: Image.PreserveAspectFit
                }
            }
            onClicked: stackView.push("qrc:/qml/pages/vision.qml")
        }

        ToolButton {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0
            background: Rectangle { color: "transparent" }
            contentItem: Item {
                anchors.fill: parent
                Image {
                    source: "qrc:/icons/home/music.png"
                    anchors.centerIn: parent
                    width: Math.min(parent.width, parent.height) * iconScale
                    height: width
                    fillMode: Image.PreserveAspectFit
                }
            }
            onClicked: stackView.push("qrc:/qml/pages/music.qml")
        }

        ToolButton {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0
            background: Rectangle { color: "transparent" }
            contentItem: Item {
                anchors.fill: parent
                Image {
                    source: "qrc:/icons/home/bluetooth.png"
                    anchors.centerIn: parent
                    width: Math.min(parent.width, parent.height) * iconScale
                    height: width
                    fillMode: Image.PreserveAspectFit
                }
            }
            onClicked: stackView.push("qrc:/qml/pages/bluetooth.qml")
        }
    }

    Rectangle {
        width: borderWidth
        color: borderColor
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        height: borderWidth
        color: borderColor
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
    }
}
