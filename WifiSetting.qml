import useriface 1.0
import QtQuick 2.0
import QtQuick 2.5
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import "."

Item {
    id: wifiSettingContainer
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true

    property string imgSrc : ""
    property variant ssidName: ["SEARCHING", "", "", "", "", "", ""]
    property variant freq: ["", "", "", "", "", ""]
    property variant flags: ["", "", "", "", "", ""]
    property variant rssiLevel: ["", "", "", "", "", ""]
    MouseArea { anchors.fill: parent; }

    property int retryConnection: 3

///////////////////////////////////

    /*
     * z-order of mainContainer is 2
     * main(z-order(1)) - mainContainer(z-order(2))
     */

    signal mainSettingExit();
    signal targetExit();

    function sendPageInfo() {
        if (wifiSettingContainer.visible) {

            UserIface.setPageNo(pageWifiSetting)
        }
    }

    function setImageSource(objId, _source) {
        objId.source = _source
    }


    function setWifiLevel(objId, list_num) {

        if(rssiLevel[list_num] > -70)
            objId.source = pngGT + "ic_wifi_3.png"
        else if(rssiLevel[list_num] > -85)
            objId.source = pngGT + "ic_wifi_2.png"
        else if(rssiLevel[list_num] > -95)
            objId.source = pngGT + "ic_wifi_1.png"
        else
            objId.source = pngGT + "ic_wifi_0.png"
    }

     function setWifiPassword(num) {

        m_ssid = ssidName[num]

        if(flags[num].search("PSK") >= 0) {
            f_open_keyboard();
        }
        else {
            m_text = "";
            f_connect_empty(m_ssid, m_text)  // 2018-03-28 JJHWANG no Password
        }

    }

/////// GENTLO //////////////////////

    Connections {
        target: UserIface

        onResScanResult: {

            wifiListModel.clear() //20200616 NKM

            var displayAp = 0

            if(wifi_host_count > 0)
            {

                for(var i=0; i<wifi_host_count ; i++)
                {
                    var params = UserIface.get_scan_list_at(i).split("^")

                    freq[i] = params[1]
                    rssiLevel[i] = params[2]

                    flags[i] = params[3]
                    ssidName[i] = params[4]

                    if(ssidName[i] !== "") {
                        if(displayAp <= 18){
                            if(rssiLevel[i] < 0) { //20200616 NKM
                                if(rssiLevel[i] > -50)
                                    imgSrc = pngGT + "ic_wifi_3.png"
                                else if(rssiLevel[i] > -55)
                                    imgSrc = pngGT + "ic_wifi_2.png"
                                else if(rssiLevel[i] > -60)
                                    imgSrc = pngGT + "ic_wifi_1.png"
                                else
                                    imgSrc = ""

//                                if(rssiLevel[i] > -60)
                                    wifiListModel.append({'rssiLevelImg':imgSrc, 'wifi_host_idx': i,'ssid': ssidName[i].substring(0, 25), 'frequecy': rssiLevel[i]})
                            }
                            else {
                                if(rssiLevel[i] > 60)
                                    imgSrc = pngGT + "ic_wifi_3.png"
                                else if(rssiLevel[i] > 55)
                                    imgSrc = pngGT + "ic_wifi_2.png"
                                else if(rssiLevel[i] > 50)
                                    imgSrc = pngGT + "ic_wifi_1.png"
                                else
                                    imgSrc = ""

//                                if(rssiLevel[i] > 50)
                                    wifiListModel.append({'rssiLevelImg':imgSrc, 'wifi_host_idx': i,'ssid': ssidName[i].substring(0, 25), 'frequecy': rssiLevel[i]})

                            }
                        }
                        displayAp ++
                    }
                }
            }

        }

    }

    Timer // SKW 2020-01-09 describe wifi info at settings page
    {
        id: wifiScanningTimer
        repeat: true; interval: 3000; running: parent.visible
        onTriggered:
        {
            var netinfo = UserIface.get_address().split("^")

             if (netinfo[1] == " (CONNECTED)") {
                 m_addrinfo = netinfo[0] + "  ("+ m_ssid +")"
             }
            else {
                if(retryConnection < 3) {
                    retryConnection++
                    wifiScanningTimer.restart()
                    searchingTxt.text = "CONNECTING..." + retryConnection
                    i_keyboard.visible = false;
                    f_connect(m_ssid, m_text)
                }
                else {
                    searchingTxt.text = " "
                    m_addrinfo == "NOT CONNECTED"
                    wifiScanningTimer.stop()
                }
            }

            if(m_addrinfo == "NOT CONNECTED" || (netinfo[1] == " (DISCONNECTED)")) {
                connectedIpTxt.text = m_addrinfo
                searchingAniRect.visible = false
                searchingTxt.text = " "
                f_clear_list()

                UserIface.conn(m_ssid, m_text)

            }
            else {
                if(netinfo[0].length < 5 ){
                    m_addrinfo == "NOT CONNECTED"
                    connectedIpTxt.text = m_addrinfo
                }
                else if( (netinfo[1] == " (CONNECTED)") && (netinfo[0].length > 6)){
                    m_addrinfo = netinfo[0]
                    connectedIpTxt.text = m_addrinfo
                    searchingAniRect.visible = false
                    wifiScanningTimer.stop()
                }
            }

            btnWifiScan.visible = true
        }
    }

    Loader {
        id: layerLoader
        property bool valid: item !== null
    }

    Connections {
        onMainSettingExit: {
            layerLoader.active = false
            homeContainer.targetExit();
        }
    }


//    ListModel { id: i_scan_list_model }
    property int bssidw: 150
    property int freqw: 70
    property int levelw: 50
    property int flagsw: 300 // 480
    property int ssidw: 390
    property int connw: 100
    property int rcount: 5
    property int listh: 40
    property int listw: listspacingr * rcount + freqw + levelw + ssidw + connw
//    property int listw: listspacingr * rcount + bssidw + freqw + levelw + flagsw + ssidw + connw
    property int listspacingr: 30
    property int viewcount: 10

    function f_open()
    {
        visible = true
        wifiScanningTimer.start()
        retryConnection = 3
    }
    function f_close()
    {
        visible = false
        wifiScanningTimer.stop()
    }

    function f_clear_list() {

        for(var i=0; i<=18; i++ ){
            freq[i] = ""
            rssiLevel[i] = ""
            flags[i] = ""
            ssidName[i] = ""
        }
        wifiListModel.clear()
    }

    function f_connect_empty(ssid, passwd)  // 2018-03-28 JJHWANG no Password
    {
        searchingAniRect.visible = true
        searchingTxt.text = "CONNECTING ..."

        UserIface.conn(ssid, passwd)
        f_clear_list()
    }

    function f_connect(ssid, passwd)
    {
        searchingAniRect.visible = true

        searchingTxt.text = "CONNECTING ..."

        f_clear_list()

        if(passwd !== "")
            UserIface.conn(ssid, passwd)
    }


    Rectangle {
        id: mainContainerBgRect
        visible: true
        anchors.fill: parent
        color:bgColor

        ImgButton {
            id: btnHomeImage
            width: 76; height: 70;
            img_org: pngDir + "btn_home_n.png"
            img_p: pngDir + "btn_home_s.png";
            img_n: pngDir + "btn_home_n.png"
            onClicked: {
                f_clear_list()
                wifiSettingContainer.mainSettingExit()
            }
        }

        Text {
            id: infoLogoTxt
            x: 20+80+10+172
            width: 478; height: 98
            text: "WiFi Settings"
            color: logoTextColor
            font.pixelSize:  34
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.family: "Myriad Pro"
        }

    }

    Text {
        id: connectedIpTxt
        x: 591; y: 84
        width: 436; height: 42
        text: "SEARCHING"
        color: "#aaaaaa"
        font.pixelSize:  30
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom
        font.family: "Myriad Pro"
    }

    TextButton
    {
        id: btnWifiScan
        visible: true
        x: 35; y: 84
        width: 150
        height: 64
        text: ""
        textsize: 34
        img_p: pngGT + "btn_scan_s.png";
        img_n: pngGT + "btn_scan_n.png"
        text_p: textColor
        text_n: textColor

        onClicked: {
            //NKM 20200401 wifi clear logic
            f_clear_list()

            //scan wifi
            wifi_host_count = UserIface.scan()
            searchingTxt.text = "SEARCHING ..."
            wifiScanningTimer.restart()

        }
    }

    //------------------------------------Make wifi list start------------------------------------
    ListModel { id: wifiListModel }

    Rectangle
    {
        x: 80; y:200
        width: 1120; height: 72*6
        visible: true
        color:"transparent"

        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height

            spacing: 20

            ListView{
                width: 1050
                height: 72*6
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10

                model: wifiListModel

                delegate:
                    Rectangle{
                        id :test_id
                        width: parent.width
                        height: 70
                        color:"transparent"

                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            width: 44; height: 42
                            clip: true; cache: false
                            source: rssiLevelImg
                        }

                        Text{
                            x:test_id.x + 60
                            width: ssidw; height: 70
                            text: ssid
                            color: inputTextColor
                            font.pixelSize:  28
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.family: "Myriad Pro"
                        }

                        TextButton
                        {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right : parent.right
                            width: 113; height: 42
                            text: ""
                            textsize: 34
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            img_p: pngGT + "btn_connect_s.png";
                            img_n: pngGT + "btn_connect_n.png"
                            text_p: inputTextColor
                            text_n: inputTextColor

                            onClicked: {
                                currListNum = 0
                                setWifiPassword(wifi_host_idx)

                            }
                        }

                    }
            }
        }
    }

    Rectangle {
        id: searchingAniRect
        visible: true
        anchors.fill: parent
        color: "transparent"

        Text {
            id: searchingTxt
            x: (defaultWidth-200)/2; y: (defaultHeight-200)/2 - 100
            width: 200; height: 30
            text: "Searching..."
            color: "#aaaaaa"
            font.pixelSize:  30
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignHCenter
            font.family: "Myriad Pro"
        }

      }

    Image {
        id: pressScanImg
        visible: false
        x: (defaultWidth - 416)/2; y: (defaultHeight-204)/2
        width: 416; height: 204
        clip: true; cache: false
        source: pngGT + "wifi_connected.png"
    }


///// BACK & SAVE

    TextButton
    {
        id: btnBackImg
        x: 500; y: 700
        width: 280; height: 80
        text: "BACK"
        textsize: 34
        img_p: pngGT + "btn_s.png";
        img_n: pngGT + "btn_n.png"
        text_p: textColor
        text_n: textColor

        onClicked: {
            UserIface.clear_conf()
            setSelectPage(pageLoader, "qrc:/SettingsCommon.qml")
        }
    }

    TextButton
    {
        id: btnClearImg
        x: btnBackImg.x + 330 + 14; y: 700
        width: 330; height: 84
        text: "CLEAR"
        textsize: 34
        img_p: pngGT + "btn_s.png";
        img_n: pngGT + "btn_n.png"
        text_p: textColor
        text_n: textColor

        onClicked: {
            UserIface.clear_conf()
            pressScanImg.visible = true
            setRowVisibleFalse()
        }

    }


    property int keypadw: 59;
    property int keypadh: 97;
    property int sp: 6;
    property bool m_shift: false;
    property string m_text: "Press password"
    property string m_ssid: ""
    property int currListNum: 0

    function f_open_keyboard()
    {
        m_text = ""
        m_shift = false;
        i_keyboard.visible = true;
    }

    function f_pushkey(key)
    {
        switch(key){
            case "Shift":
                m_shift = !m_shift;
                break;
            case "OK":
                retryConnection = 0
                wifiScanningTimer.restart()
                searchingTxt.text = "CONNECTING..."
                i_keyboard.visible = false;
                f_connect(m_ssid, m_text)
                break;
            case "Back":
                if(m_text.length === 0)
                    i_keyboard.visible = false;
                else
                    m_text = m_text.substring(0, m_text.length-1);
                break;
            case "Space":
                m_text += " ";
                break;
            default:
                m_text += key;
                break;
        }
    }

    Rectangle
    {
        id: i_keyboard
        visible: false;
        anchors.bottom: parent.bottom;
        height: 744;
        width: defaultWidth;
//        color: "#eaeaea";

        Image {
            parent: i_keyboard
            x: 0; y: 0
            width: defaultWidth; height: 744
            clip: true; cache: false
            source: pngGT + "bg_keypad_pass.png"

        }

        Column
        {
            anchors.centerIn: parent
            spacing: 21
            TextField{
                x: 30; y: 50
                width: 740; height: 56;
                text: m_text;
                font.pixelSize:  34
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.family: "Myriad Pro"
            }


            Row {
                x:13
                spacing: sp;
                visible: !m_shift;
                Repeater {
                    model: ["`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-"];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }
                    }
                }
            }

            Row {
                x: 13
                visible: !m_shift;
                spacing: sp;
                Repeater {
                    model: ["q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]"];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }
                    }
                }
            }

            Row {
                x: 13
                visible: !m_shift;
                spacing: sp;
                Repeater {
                    model: ["a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "="];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }
                    }
                }
            }

            Row {
                visible: !m_shift;
                x: 13+59
                spacing: sp;
                Repeater {
                    model: ["z", "x", "c", "v", "b", "n", "m", ",", ".", "/"];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }

                    }
                }
            }
            Row {
                visible: m_shift;
                x: 5
                spacing: 3
                Repeater {
                    model: ["~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+"];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }

                    }
                }
            }
            Row {
                visible: m_shift;
                x: 13
                spacing: sp;
                Item {
                    clip: true
                    width: keypadw/2-5; height: keypadh;
                }
                Repeater {
                    model: ["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}"];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }

                    }
                }
            }
            Row {
                visible: m_shift;
                x: 13
                spacing: sp;
                Item {
                    clip: true
                    width: keypadw; height: keypadh;
                }

                Repeater {
                    model: ["A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\""];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }

                    }
                }
            }
            Row {
                visible: m_shift;
                x: 13+59
                spacing: sp;
                Repeater {
                    model: ["Z", "X", "C", "V", "B", "N", "M", "<", ">", "?"];
                    TextButton
                    {
                        width: keypadw; height: keypadh;
                        text: modelData;
                        textsize: 38
                        img_p: pngGT + "btn_pass_s.png";
                        img_n: pngGT + "btn_pass_n.png"
                        text_p: inputTextColor
                        text_n: inputTextColor

                        onClicked: {
                            f_pushkey(text)
                        }

                    }
                }
            }
            Row
            {
                x:13
                spacing: sp;
                TextButton {
                    width: 124; height: keypadh-10;
                    text: "Shift";
                    textsize: 38
                    img_p: pngGT + "btn_pass_option_s.png";
                    img_n: pngGT + "btn_pass_option_n.png"
                    text_p: inputTextColor
                    text_n: inputTextColor
                    onClicked: f_pushkey(text);
                }
                TextButton {
                    width: 384; height: keypadh-10;
                    text: "Space";
                    textsize: 38
                    img_p: pngGT + "btn_pass_space_s.png";
                    img_n: pngGT + "btn_pass_space_n.png"
                    text_p: inputTextColor
                    text_n: inputTextColor
                    onClicked: f_pushkey(text);
                }
                TextButton {
                    width: 124; height: keypadh-10;
                    text: "Back";
                    textsize: 38
                    img_p: pngGT + "btn_pass_option_s.png";
                    img_n: pngGT + "btn_pass_option_n.png"
                    text_p: inputTextColor
                    text_n: inputTextColor
                    onClicked: f_pushkey(text);
                }
                TextButton {
                    width: 124; height: keypadh-10;
                    text: "OK";
                    textsize: 38
                    img_p: pngGT + "btn_pass_option_s.png";
                    img_n: pngGT + "btn_pass_option_n.png"
                    text_p: inputTextColor
                    text_n: inputTextColor
                    onClicked: f_pushkey(text);
                }
            }
        }
    }

    Component.onCompleted: {
        f_open()
        sendPageInfo()
    }

}
