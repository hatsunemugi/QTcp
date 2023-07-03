import QtQuick 2.15

MouseArea {
    id: root
    property color base_color: '#ffffff'
    property color base_color_light: '#ffffff'
    property color base_color_dark: '#f5f5f5'
    property color hover_color: '#f0f0f0'
    property color press_color: '#d0d0d0'
    property bool bOpen
    anchors.fill: parent
    hoverEnabled: true
    onHoveredChanged: {
        parent.color= containsMouse ?  hover_color :base_color
    }
    onPressed: {
        parent.color= press_color
    }
    onReleased: {
        parent.color= containsMouse ? hover_color :base_color
        bOpen=!bOpen
        console.log("clicked")
    }

    onBase_colorChanged: color=base_color
}
