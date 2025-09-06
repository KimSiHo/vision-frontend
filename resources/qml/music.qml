import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: musicPage

    Column {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height * 0.3

        Text {
            text: "🎵 음악 재생"
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: buttonRow
            spacing: 16
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                text: "▶️ 재생"
                onClicked: {
                    console.log("Music: PLAY")
                    const reply = commandController.sendCommand("MUSIC_PLAY")
                    console.log("Reply from backend:", reply)
                }
            }

            Button {
                text: "⏸️ 일시정지"
                onClicked: {
                    console.log("Music: PAUSE")
                    const reply = commandController.sendCommand("MUSIC_PAUSE")
                    console.log("Reply from backend:", reply)
                }
            }

            Button {
                text: "⏹️ 정지"
                onClicked: {
                    console.log("Music: STOP")
                    const reply = commandController.sendCommand("MUSIC_STOP")
                    console.log("Reply from backend:", reply)
                }
            }

            Button {
                text: "⏭️ 다음 곡"
                onClicked: {
                    console.log("Music: NEXT")
                    const reply = commandController.sendCommand("MUSIC_NEXT")
                    console.log("Reply from backend:", reply)
                }
            }

            Button {
                text: "⏮️ 이전 곡"
                onClicked: {
                    console.log("Music: PREV")
                    const reply = commandController.sendCommand("MUSIC_PREV")
                    console.log("Reply from backend:", reply)
                }
            }
        }
    }
}
