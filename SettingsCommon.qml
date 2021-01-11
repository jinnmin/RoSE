import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.controls 1.0

Item {
    id: settingCommonContainer
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight

///////////////////// 2018-09-06
    property int audioMute: 1
    property int soundVol: 0
    property int itemChecked: 0

    readonly property int informationMode: 0
    readonly property int settingsMode: 1

    property var systemLangType: "0"
    property var audioLangType: "0"
    readonly property var langEn: "EN"           // English
    readonly property var langCn: "CN"           // Chinese
    readonly property var langKr: "KR"           // Korean
    readonly property var langEs: "ES"           // Spanish
    readonly property var langAr: "AR"           // Arabic

    property bool saveFlag: false

    property int mYear
    property int mMonth
    property int mDay
    property int mHour
    property int mMinute

    property string mEditorFirst: "0" //SKW 2019-05-02 누락된 변수 추가, 사용하지 않는 PATIENT INFORMATION부분 (id: patientInfoRect)이지만, 에러메시지가 출력되어 추가함

    signal mainSettingExit();

    function setImageSource(objId, _source) {
        objId.source = _source
    }

    function sendPageInfo() {
        if( settingCommonContainer.visible) {
            print("SettingsCommon  12. SettingCommon fwVersionString : " , fwVersionString);
            currentPage = pageSettings
            firmwartVersionTxt.text = fwVersionString
            UserIface.setPageNo(pageSettings);
        }
    }

 ////////////////////////////////////////////////// Move from MainSettings.qml

    function setInfoSetting(curMode) {
        print("SettingsCommon   curMode: " + curMode)
        setImageSource(btnInformationImg, pngDir + "settings_tab_1_n.png")
        setImageSource(btnSettingsImg, pngDir + "settings_tab_2_n.png")

        settingSettingsRect.visible = false
        settingInfoRect.visible = false

        if(curMode === informationMode) {
            setImageSource(btnInformationImg, pngDir + "settings_tab_1_s.png")
            settingInfoRect.visible = true

        }
        else {
            setImageSource(btnSettingsImg, pngDir + "settings_tab_2_s.png")   //settingmode
            settingSettingsRect.visible = true
            UserIface.getTime()
        }
    }

    function setSoundLevel(level) {
        print(" setSoundLevel: ", level)
        setImageSource(radioSoundMute, pngDir + "radiobtn_off.png")
        setImageSource(radioSoundLow, pngDir + "radiobtn_off.png")
        setImageSource(radioSoundHigh, pngDir + "radiobtn_off.png")

        switch (level) {
            case 3:
                setImageSource(radioSoundMute, pngDir + "radiobtn_on.png")
                break;
            case 2:
                setImageSource(radioSoundLow, pngDir + "radiobtn_on.png")
                break;
            case 0:
                setImageSource(radioSoundHigh, pngDir + "radiobtn_on.png")
                break;

        }
    }

    function setCurrentDateTime(year, month, day, hour, minute) {
        print(year, ".", month, ".", day)

        if(year < 2000)
            year = 2019

        if(minute < 10)
            minute = "0"+minute

        if(hour < 10)
            hour = "0"+hour

        currTimeTxt.text = year + "." + month + "." + day + "   " + hour + ":" + minute

        return ; // KZ_UI_DEBUG
    }

    Connections {
        target: UserIface

        onReqGetInfoCurTime: {
            print("currYear", currYear)
            print("currMon", currMon)
            print("currDay", currDay)
            print("currHour", currHour)
            print("currMinute", currMinute)

            mYear = "20" + currYear
            mMonth = currMon
            mDay = currDay
            mHour = currHour
            mMinute = currMinute

            setCurrentDateTime(mYear, mMonth, mDay, mHour, mMinute)
        }

     }
//////////////////////////////////////////////////  Move from MainSettings.qml

    Loader {
        id: layerLoader
        property bool valid: item !== null
    }

    Connections {
        onMainSettingExit: {
            print(layerLoader.item + " is deactived")
            layerLoader.active = false
            homeContainer.targetExit();
        }
    }

///////////////////////////////////////////////////////////////////////////////////////

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
                print("HP Settings btnHomeImage")
                settingCommonContainer.mainSettingExit()
            }
        }

        Image {
            id: topLogoIcon
            visible: false
            x: 110; y: 0
            width: 172; height: 98
            clip: true; cache: false
            source: pngDir + "top_logo.png"
        }

        Text {
            id: dfitSettingsTxt
            x: defaultWidth - width
            width: 478; height: 70
            text: "Information"
            color: textColor
            font.pixelSize:  32
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.family: "Myriad Pro"
        }

        Image {
            id: btnInformationImg
            visible: false
            x: 50; y: 126
            width: 340; height: 70
            clip: true; cache: false
            source: pngDir + "settings_tab_1_s.png"

            MouseArea {
                anchors.fill: btnInformationImg
                onClicked: {
                    setInfoSetting(informationMode)
                    UserIface.stopAudioGuide()// SKW  stop 'intro.mp3' 2019-01-28
                }
                onPressed: {
                    setImageSource(btnInformationImg, pngDir + "settings_tab_1_s.png")
                }
                onReleased: {
                    setImageSource(btnInformationImg, pngDir + "settings_tab_1_n.png")
                }
            }
        }

        Image {
            id: btnSettingsImg
            visible: false
            x: 50+340+20; y: 126
            width: 340; height: 70
            clip: true; cache: false
            source: pngDir + "settings_tab_2_n.png"

            MouseArea {
                anchors.fill: btnSettingsImg

                onClicked: {
                    setInfoSetting(settingsMode)
                    UserIface.stopAudioGuide()// SKW  stop 'intro.mp3' 2019-01-28
                }
               onPressed: {
                    setImageSource(btnSettingsImg, pngDir + "settings_tab_2_s.png")
               }
                onReleased: {
                    setImageSource(btnSettingsImg, pngDir + "settings_tab_2_n.png")
                }
            }
        }

///////////////////////////////////////  Information ///////////////////////////////////////
        Rectangle {
            id: settingInfoRect
            visible: true
            x: 40; y: 126 //236 + 30
            width: defaultWidth; height: defaultHeight - y
            color:bgColor

            Column {
//                x: 0; y: 0;
                width: defaultWidth; height: 120

                Row {
                    spacing : 15
                    Text {
                        id: deviceSNText
                        text: qsTr("DEVICE S/N")
                        width: 410; height: 120
                        horizontalAlignment: Text.AlignLeft
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: inputTextColor
                    }

                    Text {
                        id: serialNumTxt
                        x: 20+410+15 +20
                        text: "HDFIT001"
                        width: 222; height: 120
                        horizontalAlignment: Text.AlignRight
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: hpSubTitle
                    }
                }

                Row {
                    spacing : 15
                    Text {
                        id: softwareTxt
                        text: qsTr("SOFTWARE VERSION")
                        width: 410; height: 120
                        horizontalAlignment: Text.AlignLeft
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: inputTextColor
                    }

                    Text {
                        id: softwartVersionTxt
                        x: serialNumTxt.x
                        text: "0.003"
                        width: 222; height: 120
                        horizontalAlignment: Text.AlignRight
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: hpSubTitle
                    }
                }

                Row {
                    spacing : 15
                    Text {
                        id: firmwareTxt
                        text: qsTr("FIRMWARE VERSION")
                        width: 410; height: 120
                        horizontalAlignment: Text.AlignLeft
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: inputTextColor
                    }

                    Text {
                        id: firmwartVersionTxt
                        x: serialNumTxt.x
                        text: "0.001"
                        width: 222; height: 120
                        horizontalAlignment: Text.AlignRight
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: hpSubTitle
                    }
                }

                Row {
                    spacing : 15
                    Text {
                        id: hardwareTxt
                        text: qsTr("HARDWARE VERSION")
                        width: 410; height: 120
                        horizontalAlignment: Text.AlignLeft
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: inputTextColor
                    }

                    Text {
                        id: hardwartVersionTxt
                        x: serialNumTxt.x
                        text: "0.001"
                        width: 222; height: 120
                        horizontalAlignment: Text.AlignRight
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: hpSubTitle
                    }
                }


                Row {
                    spacing : 15

                    Text {
                        text: qsTr("Wi-Fi SETTING")
                        width: 410; height: 120
                        horizontalAlignment: Text.AlignLeft
                        anchors.margins: 20
                        font.pixelSize:  28

                        font.family: "Myriad Pro"
                        color: inputTextColor
                    }

                    TextButton
                    {
                        id: btnWifiSet
                        width: 130; height: 64;
                        text: ""
                        textsize: 0
                        img_p: pngDir + "btn_set_s.png";
                        img_n: pngDir + "btn_set_n.png"
                        text_p: textColor
                        text_n: textColor

                        onClicked: {
                            setSelectPage(pageLoader, "qrc:/WifiSetting.qml")
                        }
                    }
                }

            }

            Column {
                y: 70
                width: defaultWidth; height: 120
                spacing: 119

                Repeater {
                    model : 6

                    Canvas {
                        width: defaultWidth; height: 1
                        onPaint: {
                            // get context to draw with
                            var ctx = getContext("2d")
                            // setup the stroke
                            ctx.lineWidth = 1
                            ctx.strokeStyle = listLineColor
                            // top-left start point
                            ctx.moveTo(0,0)
                            // upper line
                            ctx.lineTo(720,0)

                            ctx.stroke()
                        }
                    }
                }
            }
        }


    }

///////////////////////////////////////  Information ///////////////////////////////////////


///////////////////////////////////////  Settings ///////////////////////////////////////

    Image {
        id: btnSettingBg
        x:0; y: defaultHeight-height
        width: defaultWidth; height: 112
        clip: true; cache: false
        source: pngDir + "setting_bottom_bg.png"

        TextButton {
            id: btnSaveImage
            x: 352; y: 13
            width: 288; height: 86;
            text: "SAVE"
            textsize: 32
            img_p: pngDir + "btn_bottom_s.png";
            img_n: pngDir + "btn_bottom_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                print(" Settings btnSaveImage")
            }
        }

        TextButton {
            id: btnCancelImage
            x: 352 + 288; y: 13
            width: 288; height: 86;
            text: "CANCEL"
            textsize: 32
            img_p: pngDir + "btn_bottom_s.png";
            img_n: pngDir + "btn_bottom_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                print(" Settings btnCancelImage")
            }
        }

    }

    Component.onCompleted: {
        print(" Settings Common Component.onCompleted")
        sendPageInfo()
    }
}
