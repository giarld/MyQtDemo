import QtQuick 2.0

Rectangle {
    anchors.fill: parent
    color: "#ff0000"
    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: parent.clicked()
    }

    Text {
        text: qsTr("点击关闭")
        anchors.centerIn: parent
    }

    // 构造函数
    Component.onCompleted: {
        console.log("Create")
    }

    // 析构函数
    Component.onDestruction: {
        console.log("Destroy")
    }
}
