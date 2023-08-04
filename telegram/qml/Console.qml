import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
Popup_ {
    id: root
    x: parent.width/8
    height: 36
    width: parent.width/2
    Rectangle {
        color: "transparent"
        border.width: 1
        border.color: "black"
        radius: 8
        anchors.fill: parent
        LineEdit {
            id: edit
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: icon.width

            wrapMode: Text.Wrap
            onContentHeightChanged: {
                if(root!=null)
                    if(contentHeight>root.height)
                        root.height=contentHeight
            }
            Keys.onPressed: (event) => {
                switch (event.key)
                {
                case Qt.Key_Return:{
                    backend.exec(edit.text)
                    edit.text = ""
                    }break;
                case Qt.Key_Enter:{
                    backend.exec(edit.text)
                    edit.text = ""
                    }break;
                }

            }
            MouseArea {
                anchors.fill: parent
                onClicked: parent.focus = !parent.focus
            }

        }
        Icon_ {
            id: icon
            anchors.right: parent.right
            anchors.rightMargin: 2
            radius: 10
            source: "img/return.svg"
            onClick: {
                backend.exec(edit.text)
                edit.text = ""
                edit.focus = false
            }
        }
    }
}
