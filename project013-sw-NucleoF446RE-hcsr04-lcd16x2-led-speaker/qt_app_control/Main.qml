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
            model: serialManager.availablePorts()
        }

        Button {
            Layout.fillWidth: true
            text: serialManager.connected ? "Disconnect" : "Connect"
            onClicked: serialManager.connected
                       ? serialManager.disconnectPort()
                       : serialManager.connectPort(portSelector.currentText)
        }

        Button {
            Layout.fillWidth: true
            text: "Request Limits"
            enabled: serialManager.connected
            onClicked: serialManager.requestLimits()
        }

        Label { text: "Lower Limit: " + serialManager.lowerLimit.toFixed(2) + " cm" }
        Label { text: "Upper Limit: " + serialManager.upperLimit.toFixed(2) + " cm" }
    }
}
