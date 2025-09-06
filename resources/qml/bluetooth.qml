import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: bluetoothPage

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Label {
            text: "🔵 블루투스 설정"
            font.pixelSize: 28
            Layout.alignment: Qt.AlignHCenter
        }

        Switch {
            id: btSwitch
            text: btSwitch.checked ? "Bluetooth 켜짐" : "Bluetooth 꺼짐"
            checked: false
            onToggled: {
                const reply = commandController.sendCommand(checked ? "BT_ON" : "BT_OFF")
                console.log("Bluetooth:", checked ? "ON" : "OFF", "Reply:", reply)
            }
        }

        Button {
            text: "주변 기기 검색"
            onClicked: {
                const reply = commandController.sendCommand("BT_SCAN")
                console.log("Scanning for devices… Reply:", reply)
                const reply2 = bluetoothController.start()
                console.log("bluetoothController for devices… Reply:", reply2)
            }
        }

        // 🔹 실제 모델 바인딩
        ListView {
            id: deviceList
            width: 400
            height: 200
            model: bluetoothModel
            delegate: ItemDelegate {
                width: ListView.view.width
                text: model.name + " (" + model.addr + ")"
                onClicked: {
                    const reply = commandController.sendCommand("BT_CONNECT:" + model.addr)
                    console.log("Connecting to", model.addr, "Reply:", reply)
                }
            }
        }
    }
}
