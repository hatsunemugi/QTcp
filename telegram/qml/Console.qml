import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
Window {
    id: root
    x:320
    y:640
    width: 320
    height: 32
    visible: true
    color: "#ffffff"
    title: qsTr("console")
    Rectangle {
        anchors.fill: parent
        TextEdit {
            id: edit
            anchors.horizontalCenter: parent.horizontalCenter
//            canPaste: true
//            canRedo: true
//            canUndo: true

            wrapMode: Text.Wrap
            width: parent.width
            height: parent.height>contentHeight?parent.height:contentHeight
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
                    }break;
                case Qt.Key_Enter:{
                    backend.exec(edit.text)
                    }break;
                }

            }

        }
        Icon_ {
            x:root.width-width
            source: "img/return.svg"
            onClick: {
                backend.exec(edit.text)
            }
        }
    }

}
