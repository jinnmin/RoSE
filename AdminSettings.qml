import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: adminSettingContainer
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true

    readonly property int informationMode: 0
    readonly property int settingsMode: 1
///////////////////////////////////
    property bool saveFlag: false

    property bool swUpdateResult: false

    property string voltcheck : "0" //SKW 2019-01-30
    property int voltcheckFlag : 0
    //SKW 2019-01-31
    property string vcheck_core;
    property string vcheck_sdram_c;
    property string vcheck_sdram_i;
    property string vcheck_sdram_p;
    property string vcheck_temperature;
    property double vtotal_volt;
    property int operatTime: 0


    signal mainSettingExit();

    function setImageSource(objId, _source) {
        objId.source = _source
    }

    function sendPageInfo() {
        if( adminSettingContainer.visible) {
            print("AdminSettings before setPageNo : " + pageAdminSetting);
            UserIface.setPageNo(pageSettings);
        }

        print("2 adminPasswordInput. settinsPermission: ", settinsPermission)
        if (settinsPermission == 1) {
            agencyRow.visible = false
            masterRow.visible = false
        }
        else if (settinsPermission == 2) {
            agencyRow.visible = true
            masterRow.visible = false
        }
        else if (settinsPermission == 3) {
            agencyRow.visible = true
            masterRow.visible = true
        }
    }

    function f_update_Fw()
    {
        updateFw.f_open()
    }

    function f_Rms_update_Fw()
    {
        rmsUpdateFw.f_open()
    }

    function f_RM_HP_setting()
    {

    }

    function f_PS_HP_setting()
    {

    }

    Connections {
        target: UserIface
        onSwUpdateStatus: {
            print("onSwUpdateStatus", status)
            swUpdateResult = status
            if (swUpdateResult)
            {
                updateText.text = "SW Update Complete .. \n System will reboot"
            }
            else
            {
                updateText.text = "SW Update has failed ..."
                btnUpdatePopupQuit.text = "QUIT"
            }

            btnUpdatePopupQuit.visible = true

        }

        //SKW 2019-01-31 volt checker
        onVoltcheck: {
            vcheck_temperature = check_temperature
            vcheck_core = check_core
            vcheck_sdram_c = check_sdram_c
            vcheck_sdram_i = check_sdram_i
            vcheck_sdram_p = check_sdram_p
            vtotal_volt = total_volt.toFixed(2)

            operatTime ++
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
            img_org: pngDir + "btn_home_n.png";
            img_p: pngDir + "btn_home_s.png";
            img_n: pngDir + "btn_home_n.png"

            onClicked: {
                    print("adminSettingContainer btnHomeImage")
                    adminSettingContainer.mainSettingExit()
            }
        }

        Text {
            id: patientInfoLogoTxt
            x: defaultWidth - width
            width: 150; height: 98
            text: "Admin "
            color: logoTextColor
            font.pixelSize:  34
            horizontalAlignment: TextInput.AlignRight
            verticalAlignment: TextInput.AlignVCenter
            font.family: "Myriad Pro"
        }
    }

    Column {
        y: 120
        spacing: 50

        Row {
            id: userRow
            spacing : 30
            x: 100 ; y: 100
            TextButton
            {
                width: 250; height: 84;
                text: "UPGRADE SW";
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    popupRect.visible = true
//                    UserIface.updateSw(dfMainBD)
                }
            }

            TextButton
            {
                width: 250; height: 84;
                text: "UPGRADE FW";
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    f_update_Fw()
                }
            }

            TextButton
            {
                width: 250; height: 84;
                text: "RMS SW";
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    popupRect.visible = true
//                    UserIface.rmsUpdateSw(dfMainBD)
                }
            }

            TextButton
            {
                width: 250; height: 84;
                text: "RMS FW";
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    f_Rms_update_Fw()
                }
            }
        }

        Row {
            id: agencyRow
            visible: false
            spacing : 30
            x: 100

            TextButton
            {
                width: 250; height: 84;
                text: "GET LOG";
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    popupRect.visible = true
                    updateText.text = "Getting Log datas ..."
                    UserIface.getLogger();
                }
            }

            TextButton
            {
                visible: false
                width: 250; height: 84;
                text: "RMS FW";
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    f_Rms_update_Fw()
                }
            }
        }

        Row {
            id: masterRow
            visible: false
            spacing : 30
            x: 100

            TextButton
            {
                width: 250; height: 84;
                text: "SET RM"
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    f_RM_HP_setting()
                }
            }

            TextButton
            {
                visible: true
                width: 250; height: 84;
                text: "SET PS"
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    f_PS_HP_setting()
                }
            }

            TextButton
            {
                width: 250; height: 84;
                text: "START RMS"
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    UserIface.startRMS(mCurrDate)
                }
            }

            TextButton
            {
                visible: true
                width: 250; height: 84;
                text: "STOP RMS"
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    print("before stopRMS")
                    UserIface.stopRMS()
                }
            }
        }
    }

    Row {
        spacing : 30
        x: ((defaultWidth+30)/2) - 330 ; y: 700
        TextButton
        {
            width: 330; height: 84;
            text: "BACK";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                adminSettingContainer.mainSettingExit()
            }
        }

        TextButton
        {
            width: 330; height: 84;
            text: "QUIT Qt";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                print(" Bye for now!")
                Qt.quit()
            }
        }
    }

////////////// Pop up ///////////////////
    Rectangle {
        id: popupRect
        visible: false
        width: 722; height: 444
        x: ((defaultWidth - width) / 2)
        y: ((defaultHeight - height) / 2)
        color: "transparent"

        MouseArea {
            anchors.fill: parent
        }

        Image {
            id: popupImgbg
            clip: true; cache: false
            width: 722; height: 444

            source: pngDir + "bg_popup.png"
        }


        Text {
            id: updateText
            text: qsTr(" SW upgrading .. \n Please wait")
            x: popupImgbg.x
            y:  100
            width: 722; height: 40
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize:  34
            color: textColor
            font.family: "Myriad Pro"
        }

        TextButton
        {
            id: btnUpdatePopupQuit
            x: ((popupRect.width - 330) / 2)
            y:  300
            visible: false
            width: 330; height: 84;
            text: "REBOOT";
            textsize: 34
            img_p: pngDir + "btn_popup_s.png";
            img_n: pngDir + "btn_popup_n.png"
            text_p: textColor
            text_n: textColor
            onClicked: {
                popupRect.visible = false
                if(swUpdateResult)
                    UserIface.setSystemReboot(true)
            }
        }
     }

////////////// Pop up ///////////////////
    Rectangle {
        id: popupvoltageWarningRect
        visible: false
        width: 722; height: 444
        x: ((defaultWidth - popupvoltageWarningRect.width) / 2)
        y: ((defaultHeight - popupvoltageWarningRect.height) / 2)

        Image {
            id: popupvoltageWarningRectImgbg
            clip: true; cache: false
            width: 722; height: 444
            source: pngDir + "bg_popup.png"
        }

        gradient: Gradient {
            GradientStop { position: 0.0; color: "Transparent" }
        }

        Text {
            id: popupvoltageWarningRectTitleTxt
            text: qsTr("Under Voltage Warning")
            x: 0
            y: 64
            width: 722; height: 46
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize:  34
            //color: textColor
            color:"#dfdfdf"
        }

        Text {
            id: popupvoltageWarningRectTxt
            text: qsTr("Total voltage under 4.3985v \n\n (Detecting : " + vtotal_volt + " v)")
            x: 0
            y: 64+46+25
            width: 722; height: 138
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 34
            //color: textColor
            color:"#aaaaaa"
        }

        Image {
            id: btnCloseImg
            clip: true; cache: false
            width: 290; height: 80
            x: 64 + 64 + 64
            y: 313
            source: pngDir + "btn_n.png"

            Text {
                id: btnCloseTxt
                text: qsTr("CLOSE")
                //y: 15
                width: 290; height: 80
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.margins: 0
                font.pixelSize:  34
                font.family: "Myriad Pro"
                color: textColor
            }

            MouseArea {
                anchors.fill: btnCloseImg
                onClicked: {
                popupvoltageWarningRect.visible = false
                }
                onPressed: {
                    setImageSource(btnCloseImg, pngDir + "btn_s.png")
                }
                onReleased: {
                    setImageSource(btnCloseImg, pngDir + "btn_n.png")
                }
            }//MouseArea
        }//Image(id: btnCloseImg)
    }//Rectangle(id: popupvoltageWarningRect)

//////////////////////////////////////////////////////////////
    Component.onCompleted: {
        print(" Admin Settings Component.onCompleted")

        sendPageInfo()
    }

    AdminUpdateFw
    {
        id: updateFw
        anchors.fill: parent
        onClose: {
            print("SEARCHBOX is closed. selected index: ", row)
            // TODO: Callup a person if selected index value more than 0.
/*
            if(row >= 0) {
                qmls_selected_patient(row)
            }
*/
        }
    }

    AdminRmsUpdateFw
    {
        id: rmsUpdateFw
        anchors.fill: parent
        onClose: {
            print("SEARCHBOX is closed. selected index: ", row)
            // TODO: Callup a person if selected index value more than 0.
        }
    }


}

