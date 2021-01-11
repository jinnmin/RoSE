import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

//Image
Item
{
    x: 0; y: defaultHeight-520
    width: defaultWidth; height: 520
    clip: true
    visible: false
    property string m_type
    property bool m_enable_change_padType: true
    property string text: ""
    property bool m_exmode: false
    property var m_obj
    property string m_name
    property int m_max_char  // 2018-03-19 JJHWANG Num Editor change
    function f_open_ex(name, type, obj, num)
    {
print(" ****** Click f_open_ex")
        m_max_char = num

        m_name = name
        m_type = type
        m_exmode = true
        m_obj = obj
        text = obj.text
        f_setup()
    }

    function f_open(name, type, data)
    {
print(" ****** Click f_open")
        m_name = name
        m_exmode = false
        m_type = type
        text = data
        f_setup()
    }

    function f_setup()
    {
print(" ****** Click f_setup")
        visible = true

        if(m_type === "num")
        {
            bgKeyPadRtl.visible = false
            bgNumPadRtl.visible = true
        }
        if(m_type === "char")
        {
            bgKeyPadRtl.visible = true
            bgNumPadRtl.visible = false
        }
        if(m_type === "numonly")
        {
            bgKeyPadRtl.visible = false
            bgNumPadRtl.visible = true
        }

    }
    function f_push_key(key)
    {
print(" ****** Click f_push_key : " + key)
print(" ****** Click text.length : " + text.length)
        if((key === "enter") || (key === "OK"))
        {
            bgKeyPadRtl.visible = false
            bgNumPadRtl.visible = false
            visible = false
            close(m_name, text)
        }
        else if(key === "123") {
            bgKeyPadRtl.visible = false
            bgNumPadRtl.visible = true
        }
        else if(key === "ABC") {
            if(m_type != "numonly") {
                bgKeyPadRtl.visible = true
                bgNumPadRtl.visible = false
            }
        }
        else if(text.length < m_max_char) {  // 2018-03-19 JJHWANG
            if(key === "space") {
                if(m_type != "numonly")
                    text += " "
            }
            else if(key === "back") {
                text = text.substring(0, text.length-1)
            }
            else {
                text += key
            }
            if(m_exmode) {
                m_obj.text = text
print("m_obj.text: " + m_obj.text + "text: "+ text)
            } else {
                input_text(text)
            }
            push_key(key)
        }
        else if(key === "back") {  // 2018-03-19 JJHWANG
            text = text.substring(0, text.length-1)

            if(m_exmode) {
                m_obj.text = text
            } else {
                input_text(text)
            }
            push_key(key)
        }
    }

    signal push_key(string key)
    signal close(string name, string text)
    signal input_text(string text)

    MouseArea { anchors.fill: parent }

/////////////////////// ABC Key Pad /////////////////////////
    Rectangle {
        id: bgKeyPadRtl
        anchors.fill: parent
        width: parent.width; height: parent.height
        color:bgColor
        MouseArea { anchors.fill: parent }

        Image {
            id: imgKeyPadBg
            visible: true
            x: 0; y: defaultHeight-520
            width: defaultWidth; height: 520
            clip: true; cache: false
            source: pngDir + "bg_keypad.png"
        }

        Column {
            x: 28; y: 49

            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                spacing: 6
                Repeater {
                    model: ["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"]
                    TextButton {  // SU_text_button
                        width: 69; height: 97;
                        text_p: inputTextColor
                        text_n: inputTextColor
                        textsize: 38
                        img_n: pngDir + "btn_keypad_n.png"
                        img_p: pngDir + "btn_keypad_s.png"
                        text: modelData
                        onClicked: f_push_key(text)
                    }
                }
            }
        }

        Column {
            x: 66; y: 167

            anchors.horizontalCenter: parent.horizontalCenter
            Row {
                spacing: 6
                Repeater {
                    model: ["A", "S", "D", "F", "G", "H", "J", "K", "L"]
                    TextButton {  // SU_text_button
                        width: 69; height: 97;
                        text_p: inputTextColor
                        text_n: inputTextColor
                        textsize: 38
                        img_n: pngDir + "btn_keypad_n.png"
                        img_p: pngDir + "btn_keypad_s.png"
                        text: modelData
                        onClicked: f_push_key(text)
                    }
                }
            }
        }

        Column {
            x: 28; y: 167+97+21

            anchors.horizontalCenter: parent.horizontalCenter
            Row {
                spacing: 6
                TextButton {  // SU_text_button
                    width: 107; height: 97;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_option1_n.png"
                    img_p: pngDir + "btn_keypad_option1_s.png"
                    text: "."
                    onClicked: f_push_key(text)
                }

                Repeater {
                    model: ["Z", "X", "C", "V", "B", "N", "M"]
                    TextButton {  // SU_text_button
                        width: 69; height: 97;
                        text_p: inputTextColor
                        text_n: inputTextColor
                        textsize: 38
                        img_n: pngDir + "btn_keypad_n.png"
                        img_p: pngDir + "btn_keypad_s.png"
                        text: modelData
                        onClicked: f_push_key(text)
                    }
                }

                ImgButton { // SU_img_button
                    width: 107; height: 97;
                    img_n: pngDir + "btn_keypad_option1_n.png"
                    img_p: pngDir + "btn_keypad_option1_s.png"
                    Image {
                        clip: true; cache: false
                        anchors.centerIn: parent; width: 47; height: 45
                        source: pngDir + "ic_backspace.png"
                    }
                    onClicked: f_push_key("back")
                }

            }
        }

        Column {
            x: 28; y: 403
            anchors.horizontalCenter: parent.horizontalCenter
            Row {
                spacing: 6
                TextButton {  // SU_text_button
                    width: 143; height: 87;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_option2_n.png"
                    img_p: pngDir + "btn_keypad_option2_s.png"
                    text: "123"
                    onClicked: f_push_key(text)
                }

                TextButton { // SU_img_button
                    width: 446; height: 87;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_space_n.png"
                    img_p: pngDir + "btn_keypad_space_s.png"
                    text: "Space"
                    onClicked: f_push_key("space")
                }

                TextButton {  // SU_text_button
                    width: 143; height: 87;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_option2_n.png"
                    img_p: pngDir + "btn_keypad_option2_s.png"
                    text: "OK"
                    onClicked: f_push_key(text)
                }
            }
        }
    }

/////////////////////// ABC Key Pad /////////////////////////

//////////////////////  Numeric Key Pad /////////////////////
    Rectangle {
        id: bgNumPadRtl
//        visible: true
//        x: 0; y: defaultHeight-520
//        width: defaultWidth; height: 520
        anchors.fill: parent
        width: parent.width; height: parent.height
        color:bgColor
        MouseArea { anchors.fill: parent }

        Image {
            id: imgNumPadBg
            visible: true
            x: 0; y: defaultHeight-520
            width: defaultWidth; height: 520
            clip: true; cache: false
            source: pngDir + "bg_keypad.png"
        }

        Column {
            x: 29; y: 49
            spacing: 21
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                spacing: 6
                Repeater {
                    model: ["1", "2", "3", "4"]
                    TextButton {  // SU_text_button
                        width: 181; height: 97;
                        text_p: inputTextColor
                        text_n: inputTextColor
                        textsize: 38
                        img_n: pngDir + "btn_keypad_num_n.png"
                        img_p: pngDir + "btn_keypad_num_s.png"
                        text: modelData
                        onClicked: f_push_key(text)
                    }
                }
            }

            Row {
                spacing: 6
                Repeater {
                    model: ["5", "6", "7", "8"]
                    TextButton {  // SU_text_button
                        width: 181; height: 97;
                        text_p: inputTextColor
                        text_n: inputTextColor
                        textsize: 38
                        img_n: pngDir + "btn_keypad_num_n.png"
                        img_p: pngDir + "btn_keypad_num_s.png"
                        text: modelData
                        onClicked: f_push_key(text)
                    }
                }
            }
        }

        Column {
            x: 29; y: 49+97+21+97+21

            anchors.horizontalCenter: parent.horizontalCenter
            Row {
                spacing: 6
                TextButton {  // SU_text_button
                    width: 181; height: 97;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_num_option_n.png"
                    img_p: pngDir + "btn_keypad_num_s.png"
                    text: "."
                    onClicked: f_push_key(text)
                }

                Repeater {
                    model: ["9", "0"]
                    TextButton {  // SU_text_button
                    width: 181; height: 97;
                        text_p: inputTextColor
                        text_n: inputTextColor
                        textsize: 38
                        img_n: pngDir + "btn_keypad_n.png"
                        img_p: pngDir + "btn_keypad_s.png"
                        text: modelData
                        onClicked: f_push_key(text)
                    }
                }

                ImgButton { // SU_img_button
                    width: 181; height: 97;
                    img_n: pngDir + "btn_keypad_num_option_n.png"
                    img_p: pngDir + "btn_keypad_num_s.png"
                    Image {
                        clip: true; cache: false
                        anchors.centerIn: parent; width: 47; height: 45
                        source: pngDir + "ic_backspace.png"
                    }
                    onClicked: f_push_key("back")
                }

            }
        }

        Column {
            x: 29; y: 403
            anchors.horizontalCenter: parent.horizontalCenter
            Row {
                spacing: 6
                TextButton {  // SU_text_button
                    width: 143; height: 87;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_option2_n.png"
                    img_p: {
                        if(m_type != "numonly")
                            pngDir + "btn_keypad_option2_s.png"
                        else
                            pngDir + "btn_keypad_option2_n.png"
                    }
                    text: "ABC"
                    onClicked: {
                        if(m_type != "numonly")
                            f_push_key(text)
                    }
                }

                TextButton { // SU_img_button
                    width: 446; height: 87;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_space_n.png"
                    img_p: pngDir + "btn_keypad_space_s.png"
                    text: "Space"
                    onClicked: f_push_key("space")
                }

                TextButton {  // SU_text_button
                    width: 143; height: 87;
                    text_p: inputTextColor
                    text_n: inputTextColor
                    textsize: 38
                    img_n: pngDir + "btn_keypad_option2_n.png"
                    img_p: pngDir + "btn_keypad_option2_s.png"
                    text: "OK"
                    onClicked: f_push_key(text)
                }

            }
        }
    }

//////////////////////  Numeric Key Pad /////////////////////
}
