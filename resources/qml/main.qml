import QtQuick 2.15
import QtQuick.Controls 2.15
//import org.freedesktop.gstreamer.GLVideoItem 1.0

ApplicationWindow {
    id: app
    visible: true
    width: 960
    height: 540
    title: "Qt Frontend Example"

    //GLVideoItem {
    //    id: video
    //    objectName: "video"
    //    anchors.fill: parent
    //}

    // 컨트롤 버튼들
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
            text: "START Backend"
            onClicked: {
                const reply = commandController.sendCommand("START")
                console.log("Reply from backend:", reply)
                visionController.start()
                // qmlglsink의 widget 속성에 이 GLVideoItem을 넘겨야 함
                // cameraController.start(video)
            }
        }

        Button {
            text: "STOP Backend"
            onClicked: {
                const reply = commandController.sendCommand("STOP")
                console.log("Reply from backend:", reply)
                visionController.stop()
            }
        }
    }
}
