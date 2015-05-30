// Block.qml
//
import QtQuick 2.0

Item {
    id: block
    width: img.width; height: img.height

    property int type: 0
    property bool spawned: false

    Behavior on x {
        enabled: spawned
        SpringAnimation { spring: 2; damping: 0.2 }
    }
    Behavior on y {
        SpringAnimation { spring: 2; damping: 0.2 }
    }

    property bool dying: false
    states: [
        State {
            name: "AliveState"
            when: spawned == true && dying == false
            PropertyChanges { target: img; opacity: 1 }
        }
        , State {
            name: "DeathState"
            when: dying == true
            PropertyChanges { target: img; opacity: 0 }
        }
    ]

    Image {
        id: img
        anchors.fill: parent
        source: {
            if (type == 0)
                return "pics/redStone.png"
            else if (type == 1)
                return "pics/greenStone.png"
            else
                return "pics/blueStone.png"
        }
        opacity: 0

        Behavior on opacity {
            NumberAnimation { properties: "opacity"; duration: 200 }
        }
    }
}
