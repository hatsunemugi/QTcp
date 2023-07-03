import QtQuick 2.15
import QtQuick.Controls 2.15
import telegram 1.0

Popup {
    id: root
//    height: bar.currentIndex===0? 96: 128
    width: parent.width*0.75
    anchors.centerIn: parent
    padding: 0
    margins: 0
    modal: true
    property color color_: "#f0f0f0"
    property bool bConnect
    property string ip
    signal connect_
    Component.onCompleted: height= bar.currentIndex===0? 96: 128
    NumberAnimation {
        id: full
        target: root
        properties: "height"
        from: 96
        to: 128
    }
    NumberAnimation {
        id: fold
        target: root
        properties: "height"
        from: 128
        to: 96
    }
//    background: Rectangle {
//        color: "transparent"
//    }
    Rectangle {
        radius: 16
        anchors.fill: parent
        TabBar{
            id: bar
            width: parent.width
            TabButton {
                text: qsTr("单播")
                background: Rectangle_color {
                    id: left_
                    Connections {
                        target: bar
                        function onCurrentIndexChanged() {
                            left_.base_color=bar.currentIndex ===0 ? left_.base_color_dark : left_.base_color_light
                        }
                    }
                }
            }
            TabButton {
                text: qsTr("组播")
                background: Rectangle_color {
                    id: right_
                    Connections {
                        target: bar
                        function onCurrentIndexChanged() {
                            right_.base_color=bar.currentIndex ===1 ? right_.base_color_dark : right_.base_color_light
                        }
                    }
                }
            }
            onCurrentIndexChanged: {
                if(currentIndex===1)
                    full.running=true
                else
                    fold.running=true
            }
        }
        SwipeView {
            anchors.top: bar.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            currentIndex: bar.currentIndex
            clip: true
            Rectangle {
                color: root.color_
                Ipv4_input {
                    id: peer_ipv4
                    color: root.color_
                    anchors.left: parent.left
                    anchors.leftMargin: root.width*0.1
                    anchors.verticalCenter: parent.verticalCenter
                }
                Rectangle_color {
                    id: connect
                    width: 64
                    height: 32
                    radius: 16
                    anchors.right: parent.right
                    anchors.rightMargin: root.width*0.1
                    anchors.verticalCenter: parent.verticalCenter
                    base_color: "#2ef548"
                    hover_color: "#4df361"
                    press_color: "#3cc04c"
                    Text_ {
                        anchors.fill: parent
                        text: "连接"
                    }
                    onClick: {
                        root.ip=peer_ipv4.ip
                        bConnect=!bConnect
                        root.connect_()
                    }
                }
            }
            Rectangle_color {
                id: peers
            }
            onCurrentIndexChanged: {
                bar.currentIndex=currentIndex
            }
        }
    }
}
