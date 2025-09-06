import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: bluetoothPage

    Component.onCompleted: {
        console.info("BluetoothPage loaded")
    }

    Component.onDestruction: {
        console.info("BluetoothPage destroyed")
        bluetoothModel.clear()
    }

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
                console.debug("Bluetooth:", checked ? "ON" : "OFF", "Reply:", reply)
            }
        }

        Button {
            text: "주변 기기 검색"
            onClicked: {
                const reply = commandController.sendCommand("BT_SCAN")
                console.debug("Scanning for devices… Reply:", reply)
            }
        }

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
                    console.info("Connecting to", model.addr, "Reply:", reply)
                }
            }
        }
    }
}
