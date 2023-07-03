import QtQuick 2.15

Rectangle {
    id: root
    property alias text: head.text
    property string ip
    property var ip_: new Array(3)
    property string ip_prefix
    width: head.width+row.width
    height: row.height
    property int focus_index:-1
    Text_ {
        height: contentHeight
        width: contentWidth
        id: head
        text: "ip"
    }
    Text_ {
        id: point
        text: "."
        visible: false
    }
    Row {
        id: row
        height: 24
        width: 32*5
        anchors.leftMargin: 8
        anchors.left: head.right
        anchors.top: parent.top
        spacing: 2
        Repeater {
            id: repeater
            model: 7
            Ipv4_255 {
                id: per
                text: (index%2==1)?point.text:"*"
                width: (index%2==1)?point.width:32
                enabled: (index%2==0)
                base_color: root.color
                hover_color: "#e0e0e0"
                onPrev: {
                    if(index!==0){
                        focus_index=index-2;
                        move()
                    }
                }
                onNext: {
                    if(index!==6){
                        focus_index=index+2;
                    }
                    else
                    {
                        ip=ip_[0]+"."+ip_[1]+"."+ip_[2]+"."+ip_[3]
                        console.log(ip)
                        bFocus=false
                    }
                    move()
                }
                Keys.onPressed: (event) => {
                    event.accepted=true;
                }
                Connections {
                    target: root
                    function onFocus_indexChanged() {
                        per.bFocus=index===focus_index
                    }
                }
                onPaste_: {
                    var list=backend.paste_ip()
                    for(var i=0;i<list.length;i++)
                    {
                        repeater.itemAt(2*i).text=list[i];
                    }//192.168.31.132
                }
                onCopy_: {
                    backend.copy_ip(ip)
                }
                onMove: {
                    ip_[index/2]=text
                    ip=ip_[0]+"."+ip_[1]+"."+ip_[2]+"."+ip_[3]
                    console.log(ip)
                }
                onDone: {
                    ip_[index/2]=text
                    ip=ip_[0]+"."+ip_[1]+"."+ip_[2]+"."+ip_[3]
                    console.log(ip)
                }

                Component.onCompleted: {
                    var list=backend.get_prefix()
                    if(index/2<list.length&&index%2==0&&list[index/2]!=="")
                        text=list[index/2]
                }
            }
        }
    }
}
