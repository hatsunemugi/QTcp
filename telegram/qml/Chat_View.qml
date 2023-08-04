import QtQuick.Controls 6.4
import QtQuick 2.15
import telegram 1.0

Popup {
    id: root
    property Chat chat
    property int chat_count
    property string name
    height: parent.height
    width: parent.width
    anchors.centerIn: parent
    padding: 0
    margins: 0
    Connections {
        target: chat
        function onNew_msg() {
            list.model=0
            list.model=chat.get_n();
            list.positionViewAtEnd();
        }
        function onNew_file() {
            list.model=0
            list.model=chat.get_n();
            list.positionViewAtEnd();
        }
    }
    onChatChanged: {
        open()
        list.model=chat.get_n()
        var peer = chat.get_remote()
        name=peer.get_ip()
        toolbar.chat=root.chat
        backend.set_ip(name)
    }
    Rectangle {
        anchors.fill: parent
        color: "#f8f8f8"
        Rectangle {
            id: topbar
            color: "#f8f8f8"
            width: parent.width
            height: 64
            Icon_ {
                id: return_
                size: 32
                radius: 4
                base_color: parent.color
                hover_color: "#07000000"
                anchors.left: parent.left
                anchors.leftMargin: 8
                source: 'img/return.svg'
//                onBOpenChanged: root.close()
                onClick: root.close()
            }
            Text_ {
                anchors.left: return_.right
                anchors.right: detail.left
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                height: 32
                anchors.fill: parent
                text: name
            }
            Icon_ {
                id: detail
                size: 28
                radius: 4
                base_color: parent.color
                hover_color: "#07000000"
                anchors.right: parent.right
                anchors.rightMargin: 8
                source: 'img/detail.svg'
            }
        }

        ListView {
            id: list
            anchors.top: topbar.bottom
            anchors.bottom:toolbar.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 0
            clip: true
            delegate: peer
            contentWidth: root.width
            ScrollBar.vertical: ScrollBar {
                hoverEnabled: true
                active: hovered || pressed
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }
        ToolBar_ {
            id: toolbar
            anchors.bottom:parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }


    Component {
        id: peer
        Rectangle {
            width: root.width
            height: 96
            color: "transparent"
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
                    readOnly: true
                    anchors.fill: parent
                    text: chat.get_name(index)
                }
            }
            Rectangle {
                id: say
                enabled: !chat.bIsFile(index)
                visible: !chat.bIsFile(index)
//                color: "transparent"
                anchors.left: icon.right
                anchors.top: name.bottom
//                anchors.bottom: parent.bottom
                anchors.topMargin: 16
                anchors.leftMargin: 16
                width: say_.contentWidth+8
                height: say_.contentHeight+4
                radius: 4
                TextEdit {
                    id: say_
                    readOnly: true
                    x:4
                    y:2
                    font.pointSize: 12
                    height: contentHeight
                    width: root.width-icon.width-48
                    wrapMode: Text.Wrap
                    text: chat.get_c(index)
                }
                MouseArea_{
//                    base_color: "transparent"
                    Component.onCompleted: parent.color=base_color
                }
            }
            File_process {
                id: file
                enabled: chat.bIsFile(index)
                visible: chat.bIsFile(index)
                anchors.left: icon.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16
                anchors.leftMargin: 16
                text: chat.get_c(index)
                Component.onCompleted:  value=chat.get_file_process_by(index)
                Connections {
                    target: chat
                    function onProcess(value) {
                        if(chat.bIsFile(index)){
                            if(chat.get_file_process_by(index)!==1){
                                chat.set_file_process(index,value)
                                file.value=value
                            }
                        }
                    }
                }
            }
        }
    }
}
