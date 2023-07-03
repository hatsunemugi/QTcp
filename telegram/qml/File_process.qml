import QtQuick 2.15
import QtQuick.Controls 6.4
import telegram 1.0

Rectangle {
    id: root
    color: "transparent"
    width: say_.contentWidth
    height: say_.contentHeight
    property alias value: control.value
    property alias text: say_.text
    LineEdit {
        id: say_
        readOnly: true
        anchors.fill: parent
        text: chat.get_c(index)
    }
    MouseArea_{
        base_color: "transparent"
    }
    ProgressBar{
        id:control
        width: say_.contentWidth
        height: 4
        anchors.top: parent.bottom
        background: Rectangle {   //背景项
            implicitWidth: control.width
            implicitHeight: control.height
            color: "orange"
            radius: 3 //圆滑度
        }

        contentItem: Item {   //内容项
            Rectangle {
                width: control.visualPosition * control.width
                height: control.height
                radius: 2
                color: "green"
            }
        }
    }
}
