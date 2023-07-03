import QtQuick 2.15

Rectangle_color {
    id: root
    height: 32
    width: 32
    property int size: 24
    property alias source: img.source
    property alias img_opacity: img.opacity
    anchors.verticalCenter: parent.verticalCenter
    Image {
        id: img
        sourceSize: Qt.size(size, size)
        anchors { centerIn: parent }
    }
}
