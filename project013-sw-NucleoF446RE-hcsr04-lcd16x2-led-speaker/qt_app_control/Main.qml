import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 420
    height: 560
    visible: true
    title: "STM32 Serial Control"

    ColumnLayout {
        anchors.fill: parent
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
                text: "Set Lower"
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
                text: "Set Upper"
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
        }

        Label { text: "Lower Limit: " + serialManager.lower_limit.toFixed(2) }
        Label { text: "Upper Limit: " + serialManager.upper_limit.toFixed(2) }
        Label { text: "Scaling Factor: " + serialManager.scaling_factor.toFixed(4) }
        Label { text: "Distance: " + serialManager.distance.toFixed(1) }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "black"
            ListView {
                id: sampleList
                anchors.fill: parent
                clip: true
                model: ListModel { id: sampleModel }

                delegate: Text {
                    text: value.toFixed(2)
                    color: "black"
                    font.pixelSize: 16
                }
            }
        }

    }

    Connections {
            target: serialManager

            // *** Correct camelCase signal name ***
            function onHistory_sample_received(sample) {
                console.log("[QML] historySampleReceived:", sample)
                sampleModel.append({ value: sample })
                if (sampleModel.count > 10)
                    sampleModel.remove(0)
            }
    }
}
