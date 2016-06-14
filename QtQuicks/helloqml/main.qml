import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    width: 500; height: 300
    visible: true
    color: "lightgray"

    Rectangle {
        id: page
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent

        Text {
            id: helloText
            text: qsTr("Hello World")
            y: 30
            anchors.horizontalCenter: page.horizontalCenter
            font.pointSize: 24
            font.bold: true

            MouseArea {
                id: mouseArea; anchors.fill: parent
            }

            states: State {
                name: "down"; when: mouseArea.pressed == true
                PropertyChanges {
                    target: helloText
                    y: page.height-50; rotation: 360; color: "red"
                }
            }

            transitions: Transition {
                from: ""
                to: "down"
                reversible: true        //从down恢复到默认状态时使用反向动画
                ParallelAnimation {
                    NumberAnimation {
                        properties: "y,rotation"
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }

                    ColorAnimation {
                        duration: 500
                    }
                }
            }
        }

        Grid {
            id: colorPicker
            x: 20
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            rows: 2; columns: 3; spacing: 5

            Cell { cellColor: "red"; onClicked: helloText.color = cellColor}
            Cell { cellColor: "green"; onClicked: helloText.color = cellColor}
            Cell { cellColor: "blue"; onClicked: helloText.color = cellColor}
            Cell { cellColor: "yellow"; onClicked: helloText.color = cellColor}
            Cell { cellColor: "steelblue"; onClicked: helloText.color = cellColor}
            Cell { cellColor: "black"; onClicked: helloText.color = cellColor}
        }
    }
}
