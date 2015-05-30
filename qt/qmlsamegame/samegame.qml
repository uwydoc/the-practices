// samegame.qml
//
import QtQuick 2.0

import "samegame.js" as SameGame

Rectangle {
    id: screen
    width: 490; height: 720

    SystemPalette { id: activePalette }

    Item {
        width: parent.width
        anchors { top: parent.top; bottom: toolBar.top }

        Image {
            id: background
            anchors.fill: parent
            source: "pics/background.jpg"
            fillMode: Image.PreserveAspectCrop
        }

        Item {
            id: gameCanvas

            property int blockSize: 40
            property int score: 0

            width: parent.width - parent.width % blockSize
            height: parent.height - parent.height % blockSize
            anchors.centerIn: parent

            MouseArea {
                anchors.fill: parent
                onClicked: SameGame.handleClick(mouse.x, mouse.y)
            }
        }
    }

    Rectangle {
        id: toolBar
        width: parent.width; height: 40
        color: activePalette.window
        anchors.bottom: parent.bottom

        CustomButton {
            anchors { left: parent.left; leftMargin: 4; verticalCenter: parent.verticalCenter }
            text: "New Game"
            font { pointSize: 14; family: "Cambria" }
            onClicked: SameGame.startNewGame()
        }

        Text {
            id: score
            anchors { right: parent.right; rightMargin: 4; verticalCenter: parent.verticalCenter }
            text: "Score: Who knows"
            font { pointSize: 14; family: "Cambria" }
        }
    }
}
