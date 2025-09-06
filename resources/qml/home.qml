import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    Column {
        anchors.centerIn: parent
        spacing: 20

        Text { text: "메인 메뉴"; font.pixelSize: 30 }

        Button {
            text: "지도 화면으로 이동"
            onClicked: appPageLoader.source = "qrc:/qml/map.qml"
        }

        Button {
            text: "비전 페이지로 이동"

            onClicked: {
                // ✅ StackView 대신 Loader 강제 교체
                console.log("hi")

                appPageLoader.source = "qrc:/qml/main.qml"
            }
        }

        Button {
            text: "음악 재생 페이지로 이동"
            onClicked: appPageLoader.source = "qrc:/qml/music.qml"
        }
    }
}
