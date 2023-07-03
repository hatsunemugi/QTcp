import QtQuick 2.15
import QtQuick.Controls 6.4
Item {
    id: root
    height: 32
    width:parent.width
    property alias text: content.text
    property bool bOpen
    Rectangle {
        color: "#06000000"
        border.color: "#50000000"
        anchors.fill: parent
        Text_ {
            id: content
            text: "test"
            anchors.fill: parent
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onHoveredChanged: {
                parent.color= containsMouse ?  '#f0f0f0' :'#06000000'
            }
            onPressed: {
                parent.color= '#d0d0d0'
            }
            onReleased: {
                parent.color= containsMouse ? '#f0f0f0' :'#06000000'
                bOpen=!bOpen
            }
        }
    }
}
