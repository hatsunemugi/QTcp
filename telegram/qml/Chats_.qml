import QtQuick.Controls 6.4
import QtQuick 2.15
import telegram 1.0

Item {
    id: root
//    property BackEnd backend
//    property Peer peer
    property Chat chat
    property int count
    property int peer_index
    signal open
    Connections {
        target: backend
        function onNew_peer(){
            list.model=backend.get_peer_n()
        }
    }
    Rectangle {
        anchors.fill: parent
        color: "#05000000"
        ListView {
            id: list
            anchors.fill: parent
            anchors.margins: 20
            clip: true
            delegate: peer_
            contentWidth: root.width
            ScrollBar.vertical: ScrollBar {
                hoverEnabled: true
                active: hovered || pressed
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }
    }

    Component {
        id: peer_
            Peer_{
                id: p
                peer: backend.get_peer_by(index)
                onOpen: {
                    console.log(peer.get_ip())
                    root.chat=peer.toChat()
                    backend.setPeer(peer)
                    root.open()
                }
            }

    }
}
