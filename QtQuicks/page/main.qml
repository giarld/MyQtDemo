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
            width: appWin.width * 0.2
            height: width * 0.5
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
            width: appWin.width * 0.2
            height: width * 0.5
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

        Rectangle {
            width: appWin.width * 0.2
            height: width * 0.5
            color: "#ff0000"

            Text {
                text: qsTr("新窗口")
                anchors.centerIn: parent
            }

            NewW {
                id: newW
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    newW.show();
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
