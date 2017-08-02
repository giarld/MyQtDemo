import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    id: appWin
    visible: true
    width: 360
    height: 640
    title: qsTr("页面跳转示例")
    property var exam: null

    Column {
        spacing: 30
        anchors.centerIn: parent
        Rectangle {
            width: 100
            height: 50
            color: "#00ff00"

            Text {
                text: qsTr("方式一")
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(appWin.exam == null) {
                        console.log("create exam");
                        appWin.exam = Qt.createComponent("apage.qml").createObject(appWin);
                        appWin.exam.clicked.connect(appWin.destroyExam);
                    }
                }
            }
        }

        Rectangle {
            width: 100
            height: 50
            color: "#0000ff"

            Text {
                text: qsTr("方式二")
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    loader.source = "apage.qml"
                    loader.visible = true
                    loader.item.clicked.connect(function c() {
                        console.log("type 2 close")
                        loader.source = ""
                        loader.visible = false
                    })
                }
            }
        }
    }

    function destroyExam() {
        appWin.exam.destroy();
        console.log("type 1 close")
    }

    Loader {
        id: loader
        anchors.fill: parent
        visible: false
    }
}
