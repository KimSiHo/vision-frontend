import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: musicPage

    // 🎨 배경 이미지 (곡 커버)
    Image {
        id: backgroundImage
        anchors.fill: parent
        source: coverImage.source   // 동일 커버를 배경으로 사용
        fillMode: Image.PreserveAspectCrop
        opacity: 0.4                // 살짝 투명하게 (텍스트 잘 보이게)
    }

    Column {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 100   // 상단 제목 위치 조정

        Text {
            text: "🎵 음악 재생"
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            color: "white"
        }

        // 현재 곡 제목만 표시
        Text {
            id: trackTitle
            text: ""
            font.pixelSize: 20
            color: "white"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // 🎵 버튼들 → 하단 80% 위치
    Row {
        spacing: 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.8   // 화면 높이의 80% 지점
        Button { text: "▶️ 재생"; onClicked: commandController.sendCommand("MUSIC_PLAY") }
        Button { text: "⏸️ 일시정지"; onClicked: commandController.sendCommand("MUSIC_PAUSE") }
        Button { text: "⏹️ 정지"; onClicked: commandController.sendCommand("MUSIC_STOP") }
        Button { text: "⏭️ 다음 곡"; onClicked: commandController.sendCommand("MUSIC_NEXT") }
        Button { text: "⏮️ 이전 곡"; onClicked: commandController.sendCommand("MUSIC_PREV") }
    }

    // 🔊 오른쪽 세로 볼륨 슬라이더
    Column {
        spacing: 8
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 24
        height: parent.height * 0.8

        Text {
            text: "🔊"
            font.pixelSize: 20
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }

        Slider {
            id: volumeSlider
            orientation: Qt.Vertical
            from: 0; to: 100; value: 50; stepSize: 1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 30
            onValueChanged: commandController.sendCommand("AUDIO_VOL_SET:" + value)
        }
    }

    // 🚦 MusicController 시그널 연결
    Connections {
        target: musicController
        onTrackChanged: function(title, coverUrl) {
            console.log("QML: Track changed:", title, coverUrl)
            trackTitle.text = title
            backgroundImage.source = coverUrl
        }
    }
}
