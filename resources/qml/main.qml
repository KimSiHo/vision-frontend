import QtQuick 2.15
import QtQuick.Controls 2.15
import org.freedesktop.gstreamer.GLVideoItem 1.0

ApplicationWindow {
    id: app
    visible: true
    width: 1800
    height: 1000
    title: "Qt Frontend Example hi2"

    GstGLVideoItem {
        id: video
        objectName: "video"
        width: 960
        height: 544
        anchors.centerIn: parent
    }

    Repeater {
        model: detectionModel

        delegate: Rectangle {
            // 3. C++ 모델의 role 이름을 속성처럼 직접 사용합니다.
            x: video.x + box.x
            y: video.y + box.y
            width: box.width
            height: box.height

            color: "transparent"
            border.color: "yellow"
            border.width: 2

            // 라벨과 신뢰도를 표시할 텍스트
            Text {
                id: labelText
                anchors.bottom: parent.top // 박스 위에 위치
                anchors.left: parent.left

                text:  " (" + confidence.toFixed(2) + ")" // toFixed(2)는 소수점 2자리까지 표시
                color: "yellow"
                font.pixelSize: 16
            }
        }
    }

    Column {
        id: controls
        spacing: 12
        anchors {
            right: parent.right
            top: parent.top
            rightMargin: 16
            topMargin: 16
        }

        Button {
            text: "시작"
            onClicked: {
                const reply = commandController.sendCommand("START")
                console.log("Reply from backend:", reply)
                visionController.start()
                cameraController.startPipeline()
            }
        }

        Button {
            text: "카메라 전환"
            onClicked: {
                const reply = commandController.sendCommand("SWITCH_TO_CAMERA")
                console.log("Reply from backend:", reply)
            }
        }

        Button {
            text: "동영상 전환"
            onClicked: {
                const reply = commandController.sendCommand("SWITCH_TO_VIDEO")
                console.log("Reply from backend:", reply)
            }
        }

        Button {
            text: "종료"
            onClicked: {
                const reply = commandController.sendCommand("STOP")
                console.log("Reply from backend:", reply)
                visionController.stop()
            }
        }
    }
}
