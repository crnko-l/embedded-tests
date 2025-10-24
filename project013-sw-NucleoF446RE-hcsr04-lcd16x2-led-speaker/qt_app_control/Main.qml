import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 420
    height: 360
    visible: true
    title: "STM32 Serial Control"

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10

        ComboBox {
            id: port_selector
            Layout.fillWidth: true
            model: serialManager.available_ports()
        }

        Button {
            Layout.fillWidth: true
            text: serialManager.connected ? "Disconnect" : "Connect"
            onClicked: serialManager.connected ? serialManager.disconnect_port() : serialManager.connect_port(port_selector.currentText)
        }

        RowLayout {
            Button { text: "Get Limits"; onClicked: serialManager.get_limits() }
            Button {
                text: "Set Lower";
                onClicked: serialManager.set_lower_limit(parseFloat(lowerField.text))
                }
            TextField {
                id: lowerField
                placeholderText: "Lower limit"
                width: 100
                inputMethodHints: Qt.ImhDigitsOnly
            }
        }

        RowLayout {
            Button { text: "Get Scale"; onClicked: serialManager.get_scaling_factor() }
            Button {
                text: "Set Upper";
                onClicked: serialManager.set_upper_limit(parseFloat(upperField.text))
            }
            TextField {
                id: upperField
                placeholderText: "Upper limit"
                width: 100
                inputMethodHints: Qt.ImhDigitsOnly
            }
        }

        RowLayout {
            Button { text: "Get Distance"; onClicked: serialManager.get_distance() }
            Item {}
            Item{}
        }

        Label { text: "Lower Limit: " + serialManager.lower_limit.toFixed(2) }
        Label { text: "Upper Limit: " + serialManager.upper_limit.toFixed(2) }
        Label { text: "Scaling Factor: " + serialManager.scaling_factor.toFixed(4) }
        Label { text: "Distance: " + serialManager.distance.toFixed(1) }
    }
}
