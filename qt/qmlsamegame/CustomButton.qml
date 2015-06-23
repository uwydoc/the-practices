// CustomButton.qml
//
import QtQuick 2.0

Rectangle {
    property alias text: label.text
    property alias font: label.font
    signal clicked

    width: label.width + 20; height: label.height + 5;
    border { width: 1; color: Qt.darker(activePalette.button) }
    antialiasing: true
    radius: 4
    /// Color the button with a gradient
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: {
                if (mouseArea.pressed)
                    return activePalette.dark
                else
                    return activePalette.light
            }
        }
        GradientStop {
            position: 1.0
            color: activePalette.button
        }
    }

    Text {
        id: label
        anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
        text: "Button"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
