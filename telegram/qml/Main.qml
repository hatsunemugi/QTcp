import QtQuick 2.15
import QtQuick.Controls 6.4
import telegram 1.0
import QtQuick.Window 2.15

ApplicationWindow {
    x: 320
    y: 28
    width: 960
    height: 540
    visible: true
    color: "#ffffff"
    title: qsTr("telegram")
    SideBar {
        anchors.bottom: parent.bottom
        text: backend.local_ip()
        id: sidebar
        onBOpenChanged: config.open()
    }
    Config_ {
        id: config
        onConnect_: backend.connect_to(config.ip)
    }
    Chats_ {
        anchors.top: parent.top
        anchors.bottom: sidebar.top
        anchors.left: parent.left
        anchors.right: parent.right
//        backend: backend
        id:chats
        onChatChanged: chat_view.chat=chats.chat
        onOpen: {
            if(chat_view.chat!=null)
                chat_view.open()
        }
    }

    Log {
        id: log
    }

    Refresh{
        id:refresh
        onClick: {
            if(log.closed)
                log.open()
        }

//        backend: backend
    }
    Chat_View {
        id: chat_view
    }
}
