import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Window {
    width: 320
    height: 240
    title: qsTr("New Window")

    Button {
        text: "close"
        anchors.centerIn: parent
        onClicked: {
            close();
        }
    }
}
