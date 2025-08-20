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
        height: 540
        anchors.centerIn: parent
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


  // AI 영상이 표시될 배경이라고 가정
  //  Rectangle {
//       id: videoSurface
//       anchors.fill: parent
//       color: "black" // 실제로는 비디오가 출력될 영역
//   }
//
//   // Repeater가 C++ 모델의 데이터를 기반으로 UI 요소를 생성
//   Repeater {
//       // 1. main.cpp에서 등록한 C++ 모델을 지정합니다.
//       model: detectionModel
//
//       // 2. 모델의 각 아이템(DetectionItem)에 대해 아래 delegate가 하나씩 생성됩니다.
//       delegate: Rectangle {
//           // 3. C++ 모델의 role 이름을 속성처럼 직접 사용합니다.
//           x: box.x
//           y: box.y
//           width: box.width
//           height: box.height
//
//           color: "transparent"
//           border.color: "yellow"
//           border.width: 2
//
//           // 라벨과 신뢰도를 표시할 텍스트
//           Text {
//               id: labelText
//               anchors.bottom: parent.top // 박스 위에 위치
//               anchors.left: parent.left
//
//               text: label + " (" + confidence.toFixed(2) + ")" // toFixed(2)는 소수점 2자리까지 표시
//               color: "yellow"
//               font.pixelSize: 16
//
//               // 텍스트 배경
//               background: Rectangle {
//                   color: "black"
//                   opacity: 0.7
//               }
//           }
//       }
//