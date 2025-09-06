import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: app
    visible: true
    width: 1800
    height: 1000
    flags: Qt.FramelessWindowHint

    MouseArea {
        anchors.fill: parent
        onPressed: app.startSystemMove(mouse)
    }

    // 공통 타이틀바
    Rectangle {
        id: titleBar
        height: 40
        width: parent.width
        color: "#222"

        Row {
            anchors.fill: parent
            spacing: 12

            Text {
                text: "IVI Demo"
                color: "white"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            // 오른쪽 버튼들
            Row {
                spacing: 4

                Rectangle {
                    width: 40; height: 30; color: "transparent"
                    Text {
                        anchors.centerIn: parent
                        text: networkController.online ? "🛜" : "🚫"
                        font.pixelSize: 26
                    }
                }

                Rectangle {
                    width: 40; height: 30; color: "#555"
                    Text { anchors.centerIn: parent; text: "—"; color: "white" }
                    MouseArea { anchors.fill: parent; onClicked: app.showMinimized() }
                }

                Rectangle {
                    width: 40; height: 30; color: "#555"
                    Text { anchors.centerIn: parent; text: "⬜"; color: "white" }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (app.visibility === Window.Maximized)
                                app.showNormal()
                            else
                                app.showMaximized()
                        }
                    }
                }

                Rectangle {
                    width: 40; height: 30; color: "red"
                    Text { anchors.centerIn: parent; text: "✕"; color: "white" }
                    MouseArea { anchors.fill: parent; onClicked: Qt.quit() }
                }
            }
        }
    }

    // ✅ Loader는 ApplicationWindow 안에 넣는다
    Loader {
        id: appPageLoader
        anchors {
            top: titleBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        source: "qrc:/qml/home.qml"
    }
}
