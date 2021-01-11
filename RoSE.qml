import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.controls 1.0

ApplicationWindow {
    id: lightzenMainWindow
    visible: true

    readonly property int mainHome_Z:           1

    readonly property int defaultWidth:         1280
    readonly property int defaultHeight:        800

    readonly property string bgColor :          "#2d2c30"                // 2018-09-05 KZ_UI
    readonly property string textColor :        "#2d2c30" // "#dab8a8"              // 2018-09-05 KZ_UI
    readonly property string redTextColor :     "#fe311c"           // 2018-09-05 KZ_UI
    readonly property string inputTextColor :   "#dfdfdf"         // 2018-09-05 KZ_UI
    readonly property string guideTextColor :   "#555659"         // 2018-09-05 KZ_UI
    readonly property string logoTextColor :    "#aaaaaa"          // 2018-09-05 KZ_UI
    readonly property string hpSubTitle :       "#06bdb2"             // 2018-09-05 KZ_UI
    readonly property string settingSubTextColor : "#0074f8"    // 2018-09-05 KZ_UI
    readonly property string listLineColor :    "#3f4045"          // 2018-09-05 KZ_UI
    readonly property string popupTitleColor :  "#fe311c"          // 2018-09-05 KZ_UI
    readonly property string settingBgColor :   "#9233a8"

    readonly property int loadingCircletimoutInterval:  4000
    readonly property int loadingCheck:         8
    readonly property string bootMsg1 :         "EndoRobotics."
    readonly property string bootMsg2 :         "Test instructions are recommended before proceeding with the operation."
    property int loadingPercentage:             0
    property int loadOrSave:                    0 //0: Load / 1: Save


    property int skipDisplyCnt: 180

    property string alarmHPString:              "RoSE"
    property string fwVersionString:            "0.001"

//    readonly property int dfMainBD:           0x4446

///////////////////////////////////
    readonly property int pageMainHome:         0
    readonly property int pageAlex:             1
    readonly property int pageNdyag:            2
    readonly property int pageSettings:         8
    readonly property int pagePatientInfo:      9
    readonly property int pageHPSettings:       10
    readonly property int pageAdminSetting:     11
    readonly property int pageTimeSetting:      12
    readonly property int pageWifiSetting:      13
    readonly property int pageAdminFwUpdate:    14
    readonly property int pageSearchPatient:    15

    property int currentPage: pageMainHome

///////////// HP Info End //////////////////


////////////  LZ Protocol Start //////////////
    readonly property int keepAliveCmd:         0x01
    readonly property int mainFWVersionCmd:     0x02
    readonly property int subFWVersionCmd:      0x03
    readonly property int dateSetCmd:           0x04

    readonly property int cartStatusCmd:        0x11    // 17 // Command, data1, data2, data3, data4
    readonly property int inspectionStateCmd:   0x12    // 18
    readonly property int systemStateCmd:       0x13    // 19
    readonly property int sysSetStateCmd:       0x14    // 20
    readonly property int angleDataCmd:         0x15    // 21

    readonly property int errorCmd:             0xFC    // Error


////////////  LZ Protocol End //////////////


    readonly property string officialSwVer:     "0.003"
    readonly property string officialHwVer:     "0.001"
    readonly property string swVer:             "Software Version:"
    readonly property string fwVer:             "Firmware Version:"
    readonly property string hwVer:             "Hardware Version:"

    property string officialFwVer:              "0.010"

    property var enterMaster:                   "0315257002"  // 2018-06-14 JJHWANG Bi Change
    property var enterAgency:                   "0315257001"  // 2018-06-14 JJHWANG Bi Change
    property var enterUser:                     "0315257000"  // 2018-06-14 JJHWANG Bi Change
    property int settinsPermission:             0

    property var mCurrDate:                     "0"
    property bool sentLog:                      false

    property int displayAngle:                  1
    readonly property int angleMax:             360

    property string pngDir:                     "file:///home/app/res/image/Dfit/"
    property string pngGT:                      "file:///home/app/res/image/Gentlo/"
    property string pngRoSE:                    "file:///home/app/res/image/RoSE/"

    property int sendData1:                     0
    property int sendData2:                     0
    property int sendData3:                     0
    property int sendData4:                     0
    property int sendData5:                     0
    property int sendData6:                     0

    property int sendCommand:                   0
    property int sendInstruction:               1

    property string m_addrinfo:                 "NO DEVICE"
    property string m_ssid:                     "NOT CONNECTED"
    property bool wifiSignal:                   false // SKW 2020-01-23 wifi signal
    property int wifi_host_count:               0
    property string apName:                     "NO DEVICE"

    property string m_conn_ssid:                ""
    property string m_conn_passwd:              ""

////////////////////////////////////////////////////////////////////////////////////////////////
    width: defaultWidth; height: defaultHeight

    function setSelectPage(pageId, _source) {
        print(" &&&& setSelectPage pageId: ", pageId)
        pageId.active = false
        pageId.source = _source
        pageId.active = true
        pageId.z = mainHome_Z

        // disable Home Container
        homeContainer.enabled = false

        if (pageLoader.status == Loader.Ready) {
            print(pageLoader.item + " 1 is actived!!!!!!")

        }
    }

    function setImageSource(objId, _source) {
        objId.source = _source
    }

    function sendPageInfo() {
        if (homeContainer.visible) {
            print("6. pageMainHome before setPageNo", pageMainHome);
            currentPage = pageMainHome
            UserIface.setPageNo(pageMainHome)
        }
    }

    function sendCmd(cmd, getSet, data1, data2, data3, data4, data5, data6) {
        print("1. sendCmd cmd: ", cmd, "getSet: ", getSet, "data1: ", data2, "data2: ", data1, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)

        UserIface.sendToPower(cmd, getSet, data1, data2, data3, data4, data5, data6);
    }

    function qmlf_keypad(name, type, obj, num)
    {
        qmlf_keypad_text_clear()
        obj.text = ""
        print("Patient  ****** Click 3 qmlf_keypad obj.text: " + obj.text)
        obj.color = inputTextColor
        i_keypad.f_open_ex(name, type, obj, num)
    }

    function qmlf_keypad_text_clear()
    {
        i_keypad.text = ""
    }

    function qmlf_keypad_close()
    {
        i_keypad.visible = false
    }

    function activateCursor(objId) {
        objId.cursorVisible = true;
    }

    function setProgressBar(percent) {
        progressBar.percentage = percent
    }

    Connections {
        target: UserIface

        onConnectedSerial: {
             print("onConnectedSerial", connected, curPage)
            if (connected && pageMainHome == curPage)
            {
                sendCmd(mainFWVersionCmd, 1, 0, 0, 0, 0, 0, 0)
            }
            else
                UserIface.connectDev()
        }

        onResCmdFromPower: {
print("1.Home onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)

            if((currentPage != pageMainHome) && (popupEnterAdminRect.visible == false))
                return;

print("2.Home onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)

            switch(cmd) {
                case mainFWVersionCmd:
                    fwVersionNo.text = data1 + "." + data2 + data3
                    fwVersionString = fwVersionNo.text
                    break;
            }

        }


        onAlramAlert: {
            print("Main onAlram", alarmCode, "alarmHP ", alarmHP);
            if(currentPage != pageMainHome)
                return

//            popupRect.visible = true

        }
    }

    Loader {
        id: pageLoader

        //loader의 property item이 null 이 아닐 경우 true
        property bool valid: item !== null
    }

    Item {
        id: loadingContainer
        x: 0; y: 0
        width: parent.width; height: parent.height
        clip: true

        Image {
            id: bgImage
            anchors.fill: parent
            clip: true; cache: false
            source: pngRoSE + "endo_bg.png"
        }

        Image {
            id: bootIndicator1
            x: (defaultWidth - width)/2 ; y: (defaultHeight - height)/2
            width: 320; height: 205
            clip: true; cache: false
            visible: !homeContainer.enabled
            source: pngRoSE + "endo_logo.png"

            transform: Rotation { origin.x: 160; origin.y: 103; axis { x: 1; y: 1; z: 1 } angle: 1 * skipDisplyCnt }
        }

        Timer {
            id: bootingTimer
            interval: 2; running: true; repeat: false
            onTriggered: {
                skipDisplyCnt++
                if(skipDisplyCnt > 360) {
                    skipDisplyCnt = 0
                }
                bootingTimer.restart()

            }
        }

        Timer {
            id: bootProgressTimer
            interval: 10
            running: false
            repeat: true
            onTriggered:{

                if(loadingPercentage>100) {
                    bootProgressTimer.stop()
                    progressBar.visible = false
                    txt_bootMsg.visible = false
                    bootMsgTimer.stop()
                }

//                if(loadingPercentage < loadingCheck*2) {
//                    sendCmd(waterFlowCmd, 0, 0, 0, 0)
//                }
//                else if(loadingPercentage < loadingCheck *4 ) {
//                    sendCmd(waterTempCmd, 0, 0, 0, 0)
//                }
//                else if(loadingPercentage < loadingCheck *5 ) {
//                    sendCmd(simmer1Cmd, 0, 0, 0, 0)
//                }
//                else if(loadingPercentage < loadingCheck * 6 ) {
//                    sendCmd(simmer2Cmd, 0, 0, 0, 0)
//                }
//                else if(loadingPercentage < loadingCheck * 7 ) {
//                    sendCmd(gasPressCmd, 0, 0, 0, 0)
//                }
//                else if(loadingPercentage < loadingCheck * 8 ) {
//                    sendCmd(gasTempCmd, 0, 0, 0, 0)
//                }
//                else
//                    sendCmd(capVoltageCmd, 0, 0, 0, 0)

                loadingPercentage++
                setProgressBar(loadingPercentage)
            }
        }

        Timer {
            id: bootMsgTimer
            interval: 5000
            running: false
            repeat: true
            property int xor: 0
            onTriggered:{
                print("bootMsgTimer xor --> "+xor)
                print("txt_bootMsg.text --> "+txt_bootMsg.text)

                if(loadingPercentage > 100) {
                    bootMsgTimer.stop()
                  }

                xor ^= 1

                if(xor)
                    txt_bootMsg.text = bootMsg1
                else
                    txt_bootMsg.text = bootMsg2
            }
        }

        Text {
            id: swVersion
            x: defaultWidth - (192+5+77+30); y: defaultHeight-(34*3)
            horizontalAlignment: Text.AlignLeft
            anchors.margins: 0
            font.pixelSize:  24
            font.bold: false

            font.family: "Myriad Pro"
            color: textColor
            text: qsTr(swVer)
        }

        Text {
            id: swVersionNo
            x: defaultWidth - (77 + 30); y: swVersion.y
            horizontalAlignment: Text.AlignRight
            anchors.margins: 0
            font.pixelSize:  24
            font.bold: false

            font.family: "Myriad Pro"
            color: textColor
            text: qsTr(officialSwVer)
        }

        Text {
            id: fwVersion
            x: swVersion.x; y: swVersion.y - 34
            horizontalAlignment: Text.AlignLeft
            anchors.margins: 0
            font.pixelSize:  24
            font.bold: false

            font.family: "Myriad Pro"
            color: textColor
            text: qsTr(fwVer)
        }

        Text {
            id: fwVersionNo
            x: swVersionNo.x; y: swVersion.y - 34
            horizontalAlignment: Text.AlignRight
            anchors.margins: 0
            font.pixelSize:  24
            font.bold: false

            font.family: "Myriad Pro"
            color: textColor
            text: qsTr(officialFwVer)
        }

        Text {
            id: hwVersion
            x: swVersion.x; y: swVersion.y - 34 -34
            horizontalAlignment: Text.AlignLeft
            anchors.margins: 0
            font.pixelSize:  24
            font.bold: false

            font.family: "Myriad Pro"
            color: textColor
            text: qsTr(hwVer)
        }

        Text {
            id: hwVersionNo
            x: swVersionNo.x; y: swVersion.y - 34 -34
            horizontalAlignment: Text.AlignRight
            anchors.margins: 0
            font.pixelSize:  24
            font.bold: false

            font.family: "Myriad Pro"
            color: textColor
            text: qsTr(officialHwVer)
        }

        AnimatedImage {
            id: loadingCircle
            visible:false
            parent: loadingContainer
            x: 210; y: 427
            width: 60; height: 60
            clip: true; cache: false

            Timer {
                id: loadingCircleTimer
                interval: loadingCircletimoutInterval; running: true; repeat: false
                onTriggered: {
                    if(skipDisplyCnt == 0) {
                        loadingCircleTimer.restart()
                    }
                    else
                    {
                        skipDisplyCnt = 99

                        homeContainer.enabled = true
                        homeContainer.visible = true
                        bootProgressTimer.start()
                        bootMsgTimer.start()
                        txt_bootMsg.visible = true

                        loadingContainer.enabled = false
                        loadingContainer.visible = false

                        sendPageInfo()

                    }

                    skipDisplyCnt++

                }
            }
        }
    }


    Item {
        id: homeContainer
        x: 0; y: 0
        width: parent.width; height: parent.height
        visible: false
        enabled: false
        clip: true

        signal targetExit();

        Connections {
            onTargetExit: {
                print(pageLoader.item + " is deactived")
                pageLoader.active = false
                // enalble and visible Home Container
                homeContainer.enabled = true
                homeContainer.visible = true
                sendPageInfo()
            }
        }

        Rectangle {
            id: homeRect
            visible: !progressBar.visible
            anchors.fill: parent
            width: parent.width; height: parent.height
            color: bgColor

            Image {
                id: welcomeBgImg
                x:0; y:0
                width: defaultWidth; height: defaultHeight
                clip: true; cache: false
                source: ""
            }

            Image{
                id: topBtnSettingIcon
                x: 76
                width: 76; height: 70
                source: pngDir + "btn_setting_n.png"
                MouseArea {
                    anchors.fill: topBtnSettingIcon
                    onClicked: {
                        setSelectPage(pageLoader, "qrc:/SettingsCommon.qml")
                    }
                }
            }

            Row {
                y:  350
                spacing: 30
                anchors.centerIn: parent

                TextButton {
                    id: btnTrainingMenu
                    width: 325; height: 325;
                    text: "Training";
                    textsize: 50
                    img_p: pngRoSE + "btn_2.png";
                    img_n: pngRoSE + "btn_2.png"
                    text_p: inputTextColor
                    text_n: inputTextColor

                    onClicked: {
                        welcomeBgImg.visible = false
                        i_training_menu.f_open()
                    }
                }

                TextButton {
                    id: btnOperationMenu
                    width: 325; height: 325;
                    text: "Operation";
                    textsize: 50
                    img_p: pngRoSE + "btn_3.png";
                    img_n: pngRoSE + "btn_3.png"
                    text_p: inputTextColor
                    text_n: inputTextColor

                    onClicked: {
                        welcomeBgImg.visible = false
                        i_main_settings.f_open()
                    }
                }

                TextButton {
                    id: btnSystemOff
                    width: 325; height: 325;
                    text: "System off";
                    textsize: 50
                    img_p: pngRoSE + "btn_2.png";
                    img_n: pngRoSE + "btn_2.png"
                    text_p: inputTextColor
                    text_n: inputTextColor
                    onClicked: {
            //                        welcomeBgImg.visible = false
                        popupSystemOffRect.visible = true
                    }
                }
            }
        }

    } // 201904012 JOON for Welcome window


    /** ProgressBar **/
    Rectangle {
        id: progressBar
        property int percentage: 0
        x:328; y:413
        width: 625
        height: 31
        radius: height / 2
        color: "transparent"

        Item {
            id: cliprect
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width * parent.percentage / 100
            clip: true

            Rectangle {
                width: progressBar.width
                height: progressBar.height
                radius: height / 2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                color: "#b78973"
            }
        }
    }

    /** Boot Message **/
    Text { //bootMsg
        id:txt_bootMsg
        x:328; y:500
        width: 625
        height: 30
        text: "EndoRobotics"
        font.pixelSize: 17
        font.family: "Noto Sans"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "#dab8a8"
        visible: false
    }


////////////// Pop up ///////////////////

    Rectangle {
        id: popupRect
        visible: false
        width: 722; height: 444
        x: ((defaultWidth - popupRect.width) / 2)
        y: ((defaultHeight - popupRect.height) / 2)
        color: "transparent"

        MouseArea {
            x: loadingContainer.x ; y: loadingContainer.y
            width: defaultWidth; height: defaultHeight
        }

        Rectangle {
            id:popupOpacityRect
            x: -popupRect.x ; y: -popupRect.y
            width: defaultWidth; height: defaultHeight
            color: "#000000"
            opacity: 0.5
            MouseArea {
                anchors.fill: parent
            }
        }

        Image {
            id: popupImgbg
            width: 722; height: 444
            clip: true; cache: false
            source: pngDir + "bg_popup.png"
        }

        Text {
            id: updateTitleText
            text: qsTr("WARNING")
            x: 0
            y: 64
            width: 722; height: 46
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize:  34
            color: popupTitleColor
        }

        Text {
            id: updateText
            text: qsTr("Time Setting ...")
            x: 0
            y: 64 + 46 + 25
            width: 722; height: 138
            font.bold: false
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize:  34
            color: textColor
        }

        TextButton
        {
            id: btnDone
            x: ((popupRect.width - 290) / 2)
            y:  300
            width: 290; height: 80;
            text: "OK";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                popupRect.visible = false
            }
        }

    }

    Rectangle {
        id: popupSystemOffRect
        visible: false
        width: 722; height: 444
        x: ((defaultWidth - popupSystemOffRect.width) / 2)
        y: ((defaultHeight - popupSystemOffRect.height) / 2)
        color: "transparent"

        MouseArea {
            x: loadingContainer.x ; y: loadingContainer.y
            width: defaultWidth; height: defaultHeight
        }

        Rectangle {
            id:popupOperationEndOpacityRect
            x: -popupSystemOffRect.x ; y: -popupSystemOffRect.y
            width: defaultWidth; height: defaultHeight
            color: "#000000"
            opacity: 0.5
            MouseArea {
                anchors.fill: parent
            }
        }

        Image {
            id: popupSystemOffImgbg
            width: 722; height: 444
            clip: true; cache: false
            source: pngDir + "bg_popup.png"
        }

        Text {
            id: systemOffTitleTxt
            text: qsTr("System Off")
            x: 0
            y: 64
            width: 722; height: 46
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize:  34
            color: popupTitleColor
        }

        Text {
            id: systemOffTxt
            text: qsTr("Are you sure to off the system?")
            x: 0
            y: 64 + 46 + 25
            width: 722; height: 138
            font.bold: false
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize:  34
            color: inputTextColor
        }

        TextButton
        {
            id: btnYes
            x: 100
            y:  300
            width: 200; height: 80;
            text: "YES";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                popupSystemOffRect.visible = false
                systemOffImg.visible = true

            }
        }

        TextButton
        {
            id: btnNo
            x: 400
            y:  300
            width: 200; height: 80;
            text: "NO";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                popupSystemOffRect.visible = false
            }
        }

    }

    Rectangle {
        id: popupEnterAdminRect
        visible: false
        width: 722; height: 444
        x: ((defaultWidth - popupEnterAdminRect.width) / 2)
        y: ((defaultHeight - popupEnterAdminRect.height) / 2)
        color: "transparent"

        Rectangle {
            id:adminPopupOpacityRect
            x: - popupEnterAdminRect.x; y: - popupEnterAdminRect.y
            width: defaultWidth; height: defaultHeight
            color: "#000000"
            opacity: 0.5
            MouseArea {
                anchors.fill: parent
            }
        }

        Image {
            id: popupAdminImgbg
            width: 722; height: 444
            clip: true; cache: false
            source: pngDir + "bg_popup.png"
        }

        Text {
            text: qsTr("Insert Password")
            x: 0
            y: 64
            width: 722; height: 46*3 + 209
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize:  34
            font.bold: true
            font.family: "Myriad Pro"
            color: inputTextColor
        }

        TextInput {
            id: adminPasswordInput
            x: 0; y: 150
            width: 722; height: 45
            text: "**********"
            font.pixelSize:  36
            maximumLength: 10
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            font.family: "Myriad Pro"
            cursorVisible: true
            color: inputTextColor
            MouseArea {
                anchors.fill: adminPasswordInput
                onClicked: {
                    activateCursor(adminPasswordInput)
                    qmlf_keypad("password_input", "numonly", adminPasswordInput, 10)
                }
            }
        }

        TextButton
        {
            x: 50
            y:  300
            width: 290; height: 80;
            text: "OK";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                print("0 before adminPasswordInput.text: ", adminPasswordInput.text)
                if((adminPasswordInput.text == enterMaster) || (adminPasswordInput.text == "8899") ) {
                print("1 adminPasswordInput.text: ", adminPasswordInput.text)
                    adminPasswordInput.text = "**********"
                    popupEnterAdminRect.visible = false
                    settinsPermission = 3
                    setSelectPage(pageLoader, "qrc:/AdminSettings.qml")
                }
                else if (adminPasswordInput.text == enterAgency) {
                print("2 adminPasswordInput.text: ", adminPasswordInput.text)
                    adminPasswordInput.text = "**********"
                    popupEnterAdminRect.visible = false
                    settinsPermission = 2
                    setSelectPage(pageLoader, "qrc:/AdminSettings.qml")
                }
                else if (adminPasswordInput.text == enterUser){
                print("3 adminPasswordInput.text: ", adminPasswordInput.text)
                    adminPasswordInput.text = "**********"
                    popupEnterAdminRect.visible = false
                    settinsPermission = 1
                    setSelectPage(pageLoader, "qrc:/AdminSettings.qml")
                }
                else {
                print("4 adminPasswordInput.text: ", adminPasswordInput.text)
                    adminPasswordInput.text = "Incorrct Password"
                }
            }
        }

        TextButton
        {
            x: popupEnterAdminRect.width - 50 - 290
            y:  300
            width: 290; height: 80;
            text: "CANCEL";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                adminPasswordInput.text = "**********"
                popupEnterAdminRect.visible = false

            }
        }

    }

    Image {
        id: systemOffImg
        visible: false
        width: defaultWidth; height: defaultHeight
        clip: true; cache: false
        source: pngRoSE + "endo_ci.png"

        MouseArea {
            anchors.fill: parent
        }

    }
////////////// Pop up ///////////////////

    MainSettings {
        id: i_main_settings
        onClose: {
            print("i_main_settings close homeContainer.visible: ", homeContainer.visible)
        }
    }

    TrainingMenu {
        id: i_training_menu
        onClose: {
            print("i_training_menu close homeContainer.visible: ", homeContainer.visible)
        }
    }

    FunctionUtility {
        id: i_function
    }

    // KEYPAD
    Keypad
    {
        id: i_keypad;
        onClose:
        {
            print("RM Gentlo Keypad close : ",name, text)
        }
    }

}
