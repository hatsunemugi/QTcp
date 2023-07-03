import QtQuick 2.15
import QtQuick.Controls 2.15
import telegram 1.0

Popup_ {
    id: root

    Connections {
        target: backend
        function onNew_log() {
            list.model= backend.get_log_n()
            list.positionViewAtEnd();
        }
    }
    Rectangle {
        id: rect
        width: parent.width
        height: parent.height
//        anchors.centerIn: parent
        color: "transparent"
        border.width: 1
        border.color: "black"
        radius: 8
        Rectangle {
            id: topbar
            color: "#f8f8f8"
//            border.width: 1
//            border.color: "black"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            radius: 8
            anchors.margins: 4
            height: 36
            Icon_ {
                id: detail
                size: 24
                radius: 16
                base_color: parent.color
                hover_color: "#07000000"
                anchors.right: return_.left
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                source: 'img/detail.svg'
                onClick: {
                    var component = Qt.createComponent("Console.qml")
                    var console = component.createObject()
                    console.show()
                }
            }
            Icon_ {
                id: return_
                size: 28
                radius: 8
                base_color: parent.color
                hover_color: "#07000000"
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                source: 'img/return.svg'
//                onBOpenChanged: root.close()
                onClick: root.close()
            }
            MouseArea {
                anchors.left: parent.left
                anchors.right: detail.left
                height: parent.height
                drag.target: rect
            }
        }
        Rectangle {
            anchors.top: topbar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 4
//            antialiasing: true
//            border.width: 1
//            border.color: "black"
            radius: 8
            ListView {
                id: list
                spacing: 8
                clip: true
                model: 0
                anchors{
                    fill: parent
                    topMargin: 1
                    rightMargin: 1
                    bottomMargin: 1
                }

                contentWidth: width
                delegate: Component {
                    Rectangle_color {
                        base_color: "transparent"
                        x: 16
                        width: list.contentWidth
                        height:  log_.contentHeight
                        LineEdit {
                            id: log_
                            readOnly: true
                            selectByMouse: true
                            font.pointSize: 16
                            anchors.fill: parent
                            wrapMode: Text.Wrap
                            text: backend.get_log_by(index)
                        }
                    }
                }
                ScrollBar.vertical: ScrollBar {
                    hoverEnabled: true
                    active: hovered || pressed
//                    anchors.top: parent.top
//                    anchors.right: parent.right
//                    anchors.bottom: parent.bottom
                }
            }
        }

        MouseArea_resize {
            z: -1
        }
    }
}
