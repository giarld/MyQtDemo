import QtQuick 2.9
import QtQuick.Window 2.2

import QtMultimedia 5.9

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    VideoOutput {
        anchors.fill: parent
        source: camera
        rotation: 90
    }

    Camera {
        id: camera
    }
}
