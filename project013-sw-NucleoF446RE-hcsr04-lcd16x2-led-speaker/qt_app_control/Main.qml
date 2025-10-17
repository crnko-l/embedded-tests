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
            Layout.fillWidth: true
            model: SerialManager.availablePorts()
        }

        Button {
            Layout.fillWidth: true
            text: SerialManager.connected ? "Disconnect" : "Connect"
            onClicked: SerialManager.connected
                       ? SerialManager.disconnectPort()
                       : SerialManager.connectPort(portSelector.currentText)
        }

        Button {
            Layout.fillWidth: true
            text: "Request Limits"
            enabled: SerialManager.connected
            onClicked: SerialManager.requestLimits()
        }

        Label { text: "Lower Limit: " + SerialManager.lowerLimit.toFixed(2) + " cm" }
        Label { text: "Upper Limit: " + SerialManager.upperLimit.toFixed(2) + " cm" }
    }
}
