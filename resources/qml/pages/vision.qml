import QtQuick 2.15
import QtQuick.Controls 2.15
import org.freedesktop.gstreamer.GLVideoItem 1.0

Page {
    id: visionPage

    Component.onCompleted: {
        console.info("VisionPage loaded")
    }

    Component.onDestruction: {
        console.info("VisionPage destroyed")
    }

    // 원본 프레임 해상도 (백엔드에서 알 수 있다면 동적으로 세팅해도 됨)
    property int srcWidth: 960
    property int srcHeight: 544

    GstGLVideoItem {
        id: video
        objectName: "video"
        anchors.centerIn: parent

        property real aspect: srcWidth / srcHeight

        width: parent.width
        height: width / aspect

        onWidthChanged: {
            if (height > parent.height) {
                height = parent.height
                width = height * aspect
            }
        }
    }

    Repeater {
        model: detectionModel

        delegate: Rectangle {
            x: video.x + (box.x * video.width / visionPage.srcWidth)
            y: video.y + (box.y * video.height / visionPage.srcHeight)
            width: box.width * video.width / visionPage.srcWidth
            height: box.height * video.height / visionPage.srcHeight

            color: "transparent"
            border.color: "yellow"
            border.width: 2

            Text {
                anchors.bottom: parent.top
                anchors.left: parent.left
                text: "(" + confidence.toFixed(2) + ")"
                color: "yellow"
                font.pixelSize: 16
            }
        }
    }

    Row {
        id: controls
        spacing: 12
        anchors {
            top: parent.top
            right: parent.right
            topMargin: 16
            rightMargin: 16
        }

        Button {
            text: "시작"
            onClicked: {
                const reply = commandController.sendCommand("CAMERA_START")
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
            text: "테스트 전환(원격 비디오)"
            onClicked: {
                const reply = commandController.sendCommand("SWITCH_TO_TEST")
                console.log("Reply from backend:", reply)
            }
        }

        Button {
            text: "캡쳐"
            onClicked: {
                video.grabToImage(function(result) {
                    if (result) {
                        captureController.saveFrame(result.image)
                    }
                })
            }
        }

        Button {
            text: "종료"
            onClicked: {
                const reply = commandController.sendCommand("CAMERA_STOP")
                console.log("Reply from backend:", reply)
                visionController.stop()
            }
        }
    }
}
