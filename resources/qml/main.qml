import QtQuick 2.15
import QtQuick.Controls 2.15
import org.freedesktop.gstreamer.GLVideoItem 1.0

Page {
    id: visionPage
    Component.onCompleted: {
        console.log("✅ main.qml really loaded", visionPage)
        console.log("main.qml loaded, waiting for window...")
        if (visionPage.window) {
            visionPage.window.sceneGraphInitialized.connect(function() {
                console.log("SceneGraph ready → startPipeline")
                cameraController.startPipeline()
            })
        }
    }

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
            x: video.x + box.x
            y: video.y + box.y
            width: box.width
            height: box.height

            color: "transparent"
            border.color: "yellow"
            border.width: 2

            Text {
                id: labelText
                anchors.bottom: parent.top
                anchors.left: parent.left
                text:  " (" + confidence.toFixed(2) + ")"
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
            text: "Check Now"
            onClicked: networkController.onlineChanged(networkController.online)
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
                const reply = commandController.sendCommand("STOP")
                console.log("Reply from backend:", reply)
                visionController.stop()
            }
        }
    }
}
