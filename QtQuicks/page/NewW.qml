import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Window {
    width: 320
    height: 240
    title: qsTr("New Window")

    Image {
        id: img
        anchors.fill: parent
        source: "http://giarld.github.io/img/avatar.png"
    }

    Button {
        text: "close"
        anchors.centerIn: parent
        onClicked: {
            close()
        }
    }

    Component.onCompleted: {
        console.log("NewW Create")
    }

    Component.onDestruction: {
        console.log("NewW Destroy")
    }
}
