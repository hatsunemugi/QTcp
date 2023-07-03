import QtQuick 2.15

Rectangle_color {
    width: 32
    height: 24
    property alias text: edit.text
    property alias bFocus: edit.focus
    property string ip_prefix
    property string prev_
    signal next()
    signal prev()
    signal done()
    signal move()
    signal paste_()
    signal copy_()
    signal new_ip_prefix()
    onIp_prefixChanged: {
        ip_prefix_vector=ip_prefix.match("(\\d+).(\\d+).(\\d+).(\\d+)")
        for(var i=0;i<ip_prefix_vector.length;i++)
            console.log(ip_prefix_vector[i])
    }
    onMove: {
        if(text==="")
            text="*"
    }
    TextInput {
        id: edit
        selectByMouse: true
        font.pointSize: 16
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        validator:  RegularExpressionValidator{ regularExpression: /^[0-9/]*[*]?$/}
        Component.onCompleted: prev_=text
        onTextChanged: {
            if(text.length>=3)
            {
                if(text>255)
                    text=255
                else
                    next()
            }
            if(text.length>1)
            {
                if(text.charAt(text.length-1)!=='*')
                {
                    text=Number(text)
                    prev_=text
                }
                else
                    text="*"
                if(text.charAt(0)>2)
                    next()
            }
        }
        onFocusChanged: {
            if(focus){
                if(text==="*"||text==="0"){
                    text=""
                }
            }
        }
        Keys.onPressed: (event) => {
            switch (event.key)
            {
            case Qt.Key_Left:if(cursorPosition==0)
                {
                    console.log("prev")
                    prev()
                }break;
            case Qt.Key_Right:if(cursorPosition==length)
                {
                    console.log("next")
                    next()
                }break;
            case Qt.Key_Return:{
                console.log("done");
                focus=false;
                done();
                }break;
            case Qt.Key_Enter:{
                console.log("done");
                focus=false;
                done();
                }break;
            case Qt.Key_V:if(event.modifiers===Qt.ControlModifier){
                paste_()
                }break;
            case Qt.Key_C:if(event.modifiers===Qt.ControlModifier){
                copy_()
                }break;
            }

        }
    }
}
