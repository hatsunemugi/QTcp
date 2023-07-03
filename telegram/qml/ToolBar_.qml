import QtQuick 2.15
import QtQuick.Controls 6.4
import telegram 1.0

Item {
    id: root
    width: parent.width
    height: contentHeight<mode.height?mode.height:contentHeight+16
    property Chat chat
    property alias contentHeight: edit.height
    state: "fold"
    FileDialog_ {
        id:file
    }
    Rectangle {
        id: bar
        radius: 8
        anchors.top: parent.top
        height: edit.height<mode.height?mode.height:edit.height+16
        width: parent.width
        color: "#f9f9f9"
        Icon_ {
            id: mode
            size: 32
            radius: 16
            base_color: parent.color
            hover_color: "#07000000"
            anchors.left: parent.left
            anchors.leftMargin: 8
            source: 'img/keyboard.svg'
        }
        Rectangle {
            color: "#99ffffff"
            anchors.left: mode.right
            anchors.right: plus.left
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            height: edit.height<mode.height?mode.height:edit.height
            TextEdit {
                id: edit
                anchors.verticalCenter: parent.verticalCenter
                height: contentHeight>mode.height?contentHeight:mode.height
                width: parent.width
                selectByMouse: true
                wrapMode: Text.Wrap
                font.pointSize: 16
                onTextChanged: {
                    if(text==="")
                        plus.state="thin"
                    else
                        plus.state="fat"
                }
            }
        }
        Icon_ {
            id: plus
            size: 28
            radius: 16
            base_color: parent.color
            hover_color: "#07000000"
            anchors.right: parent.right
            anchors.rightMargin: 8
            source: 'img/plus.svg'
            state: "thin"
            onClick:  {
                if(state==="fat")
                    chat.send_msg(edit.text)
                if(state==="thin")
                {
                    if(root.state==="fold"){
                        root.state="full"
                        _顺时针.running=true
                    }
                    else{
                        root.state="fold"
                        _逆时针.running=true

                    }
                }
                edit.text=""
            }
            Text_ {
                id: send
                anchors.fill: parent
                text: "发送"
            }

            states: [
                State {
                    name: 'thin'

                    PropertyChanges {
                        target: plus
                        width: 32
                        radius: 16
                        base_color: parent.color
                        img_opacity:1
                    }
                    PropertyChanges {
                        target: send
                        opacity: 0
                    }
                },
                State {
                    name: 'fat'

                    PropertyChanges {
                        target: plus
                        width: 64
                        radius: 8
                        base_color: "lightgreen"
                        img_opacity:0
                    }
                    PropertyChanges {
                        target: send
                        opacity: 1
                    }
                }
            ]

            transitions: [
                Transition {
                    from: 'fat'
                    to: 'thin'

                    NumberAnimation {
                        properties: 'width, radius'
                        duration: 300
                        easing.type: Easing.InOutSine
                    }
                },
                Transition {
                    from: 'thin'
                    to: 'fat'

                    NumberAnimation {
                        properties: 'width, radius'
                        duration: 300
                        easing.type: Easing.InOutSine
                    }
                }
            ]
            RotationAnimation {
                id: _顺时针
                target: plus
                from: 0
                to: 45
                duration: 300
            }
            RotationAnimation {
                id: _逆时针
                target: plus
                from: 90
                to: 0
                duration: 300
            }
        }
    }
    Rectangle_color {
        id: tool
        anchors.top: bar.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        radius: 8
        GridView {
            id: grid
            cellHeight: height/2
            cellWidth: width/4
            anchors.margins: 4
            anchors.fill: parent
            clip: true
            model: ToolBar_model{}
            flow: GridView.FlowTopToBottom
//            layoutDirection: "LeftToRight"
//            verticalLayoutDirection: "TopToBottom"
            delegate: Component {
                Rectangle_color {
                    radius: 8
                    Text_ {
                        anchors.fill: parent
                        text:name_
                    }

                    height: grid.cellHeight
                    width: grid.cellWidth
                    onClick: {
                        file.visible=true
                        file.open()
                    }
                }
            }

            ScrollBar.horizontal: ScrollBar {
                hoverEnabled: true
                active: hovered || pressed
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }
    }

    states: [
        State {
            name: 'fold'
            PropertyChanges {
                target: root
                height: contentHeight+16
            }
            PropertyChanges {
                target: tool
                opacity: 0
            }
        },
        State {
            name: 'full'
            PropertyChanges {
                target: root
                height: 256
            }
            PropertyChanges {
                target: tool
                opacity: 1
            }
        }
    ]

    transitions: [
        Transition {
            from: 'fold'
            to: 'full'

            NumberAnimation {
                properties: 'height,opacity'
                duration: 300
                easing.type: Easing.InOutSine
            }
        },
        Transition {
            from: 'full'
            to: 'fold'

            NumberAnimation {
                properties: 'height,opacity'
                duration: 300
                easing.type: Easing.InOutSine
            }
        }
    ]

}
