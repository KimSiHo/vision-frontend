import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: app
    visible: true
    width: 1800 // Screen.width
    height: 1000 // Screen.height
    flags: Qt.FramelessWindowHint

    MouseArea {
        anchors.fill: parent
        onPressed: app.startSystemMove(mouse)
    }

    Rectangle {
        id: titleBar

        property color titleColor: "#222"
        property color textColor: "white"

        height: 40
        width: parent.width
        color: titleColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 8

            ToolButton {
                text: "⬅"
                onClicked: if (stackView.depth > 1) stackView.pop()
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            }

            Label {
                text: "IVI Demo"
                color: titleBar.textColor
                font.pixelSize: 25
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            }

            Item { Layout.fillWidth: true }

            Label {
                text: networkController.online ? "🛜" : "🚫"
                font.pixelSize: 32
                Layout.alignment: Qt.AlignVCenter
            }

            ToolButton { text: "—"; onClicked: app.visibility = Window.Minimized }
            ToolButton {
                text: "⬜"
                onClicked: app.visibility =
                        app.visibility === Window.Maximized
                    ? Window.AutomaticVisibility
                    : Window.Maximized
            }
            ToolButton { text: "✕"; onClicked: Qt.quit() }
        }
    }

    // ✅ Loader는 ApplicationWindow 안에 넣는다
    StackView {
        id: stackView
        anchors {
            top: titleBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        initialItem: Qt.resolvedUrl("qrc:/qml/pages/home.qml")
    }
}
