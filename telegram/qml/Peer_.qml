import QtQuick 2.15
import telegram 1.0

Item {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right
    height: 72
    property Peer peer
    property alias name: name_.text
    property alias say: say_.text
    property alias color: rect.color
    property bool bOnline
//    property bool bOpen: false
    signal open
    Timer {
        interval: 500
        repeat: true
        running: true
        onTriggered: {
            var bName=peer.get_name()==="";
            name_.text=bName?peer.get_ip():peer.get_name()
            wifi.source=peer.connected_()?'img/wifi.svg':'img/wifi-off-line.svg'

        }
    }
//    onPeerChanged: {

//        wifi.source=peer.connected_()?'img/wifi.svg':'img/wifi-off-line.svg'
//    }

    Rectangle {
        id:rect
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onHoveredChanged: {
                parent.color= containsMouse ?  '#f0f0f0' :'#ffffff'
                wifi.color= containsMouse ?  '#f0f0f0' :'#ffffff'
            }
            onPressed: {
                parent.color= '#d0d0d0'
                wifi.color= '#d0d0d0'
            }
            onReleased: {
                parent.color= containsMouse ? '#f0f0f0' :'#ffffff'
                wifi.color= containsMouse ? '#f0f0f0' :'#ffffff'
//                bOpen=!bOpen
                open()
            }
        }
    }

    Rectangle {
        id: icon
        anchors.left: parent.left
        anchors.leftMargin: 16
        color: "lightgreen"
        anchors.verticalCenter: parent.verticalCenter
        height: 32
        width: 32
    }
    Rectangle {
        id: name
        color: "transparent"
        anchors.left: icon.right
        anchors.top: parent.top
        anchors.leftMargin: 16
        anchors.topMargin: 16
        width: name_.contentWidth
        height: name_.contentHeight
        LineEdit {
            id: name_
            anchors.fill: parent
            readOnly: true
            text: "user"
        }
    }
    Rectangle {
        id: say
        color: "transparent"
        anchors.left: icon.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.leftMargin: 16
        width: say_.contentWidth
        height: say_.contentHeight
        LineEdit {
            id: say_
            anchors.fill: parent
            readOnly: true
            font.pointSize: 12
            text: "say"
        }
    }
    Icon_ {
        id: wifi
        radius: 8
        anchors.right: parent.right
        anchors.rightMargin: 48
        onClick: {
            if(!peer.connected_())
                peer.reconnect()
            else
                peer.disconnect_()
        }
    }

    Connections {
        target: peer
        function onNew_msg(value) {
            say_.text = value
        }
    }
}
