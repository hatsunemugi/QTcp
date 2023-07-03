import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Popup_ {
    id: root
    width: 32
    height: 32
    x: parent.width-66
    y: parent.height-66
    property int duration: 2000
//    property BackEnd backend
//    property bool bRefresh
    signal click
    Rectangle {
        id: refresh_
        anchors.centerIn: parent
        width: 32
        height: 32
//        border.color: "#99000000"
//        border.width: 2
        color: "#eeffffff"
        radius: 16
        Image {
            id: refresh__
            source: 'img/refresh.svg'
            sourceSize: Qt.size(24, 24)
            anchors { centerIn: parent }
        }
        MouseArea {
            anchors.fill: parent
            onPressed: {
                parent.color= '#d0d0d0'
            }
            hoverEnabled: true
            onHoveredChanged: {
                parent.color= containsMouse ?  '#f0f0f0' :'#ffffff'
            }
            onReleased: {
                parent.color= containsMouse ? '#f0f0f0' :'#ffffff'
                _顺时针.running=true
//                bRefresh=!bRefresh
                click()
//                backend.scan()
            }
        }
        RotationAnimation {
            id: _顺时针
            target: refresh__
            from: 0
            to: 1080
            duration: root.duration
        }
        RotationAnimation {
            id: _逆时针
            target: refresh__
            from: 90
            to: 0
            duration: root.duration
        }
    }
    DropShadow {
        anchors.fill: refresh_
//        horizontalOffset: 3
//        verticalOffset: 3
        radius: 16
        samples: 32
        color: "#80000000"
        source: refresh_
    }
//    modal: false
//    focus: true
//    dim:true
//    Overlay.modeless: Rectangle {
//        color: "transparent"
//    }
//    background: Rectangle {
//        color: "transparent"
//    }
//    closePolicy:Popup.NoAutoClose
    Component.onCompleted: open()
}
