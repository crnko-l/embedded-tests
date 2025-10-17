import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 400
    height: 300
    visible: true
    title: "STM32 Serial Control"

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10

        ComboBox {
            id: portSelector
            model: SerialManager.availablePorts()
            Layout.fillWidth: true
        }

        RowLayout {
            Button {
                text: SerialManager.connected ? "Disconnect" : "Connect"
                onClicked: {
                    if (SerialManager.connected)
                        SerialManager.disconnectPort()
                    else
                        SerialManager.connectPort(portSelector.currentText)
                }
            }
        }

        RowLayout {
            Button { text: "LED ON";  onClicked: SerialManager.sendCommand("LED ON") }
            Button { text: "LED OFF"; onClicked: SerialManager.sendCommand("LED OFF") }
            Button { text: "STATUS";  onClicked: SerialManager.sendCommand("STATUS") }
        }

        TextArea {
            id: logArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            text: "Waiting for data..."
        }
    }

    Connections {
        target: SerialManager
        function onMessageReceived(msg) {
            logArea.append(msg)
        }
    }
}
