import QtQuick 2.6
import QtQuick.Window 2.2
import "dpi.js" as DPI

Window {
    id: appWin
    visible: true
    width: 360
    height: 640
    title: qsTr("Screen fit")

    Rectangle {
        width: DPI.dpi(appWin, 360)
        height: DPI.dpi(appWin, 100)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: DPI.dpi(appWin, 30)
        color: "#2dff00"
        border.color: "black"
        border.width: DPI.dpi(appWin, 3)

        Text {
            text: "Hello"
            font.pointSize: DPI.dpi(appWin, 100)
            anchors.centerIn: parent
            color: "#ff0000"
            style: Text.Outline
            styleColor: "#0000ff"
        }
    }

    Grid {
        rows: 2; columns: 3; spacing: DPI.dpi(appWin, 5)
        anchors.centerIn: parent

        Rectangle { width: DPI.dpi(appWin, 200); height: DPI.dpi(appWin, 150); color: "red" }
        Rectangle { width: DPI.dpi(appWin, 200); height: DPI.dpi(appWin, 150); color: "blue" }
        Rectangle { width: DPI.dpi(appWin, 200); height: DPI.dpi(appWin, 150); color: "green" }
        Rectangle { width: DPI.dpi(appWin, 200); height: DPI.dpi(appWin, 150); color: "orange" }
        Rectangle { width: DPI.dpi(appWin, 200); height: DPI.dpi(appWin, 150); color: "pink" }
        Rectangle { width: DPI.dpi(appWin, 200); height: DPI.dpi(appWin, 150); color: "black" }
    }
}
