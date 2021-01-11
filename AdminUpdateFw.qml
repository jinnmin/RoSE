import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.controls 1.0
import QtQuick.Layouts 1.3



Rectangle {
    id: adminFwUpdatePopup
    visible: false
    color: "transparent"

    MouseArea {
        anchors.fill: parent
    }

    readonly property int initializte: -1
    readonly property int fail: 0
    readonly property int success: 1
    readonly property int err_dir_not_found: 2
    readonly property int err_file_not_found: 3

    readonly property int waiting_for_mcu: 4
    property int updateStatus: initializte

    property int selectedItem: -1
    readonly property int updateFirmwareMain: 0
    readonly property int updateFirmwareSono: 1
    readonly property string fwMainVer: "FW MAIN Ver. v"
    readonly property string fwSonoVer: "FW RF Multi Needle Ver. v"    //SKW 2019-03-11 표기 수정 (FW SONO Ver. v > FW RF Multi Needle Ver. v)
    readonly property string swVer:     "SW Ver. v1.0.0.7"
    property var versionOfFiles: undefined

    signal close(int row)

    function f_open() {
        visible = true
        sendPageInfo()
    }

    function f_close() {
        visible = false
    }

    function sendPageInfo() {
        if (adminFwUpdatePopup.visible) {
            UserIface.setPageNo(pageAdminFwUpdate)
        }
    }

    function setImageSource(objId, _source) {
        objId.source = _source
    }

    Connections {
        target: UserIface
        onFwUpdateStatus: {
            print("onFwUpdateStatus", status)
            if (status)
                updateStatus = waiting_for_mcu
            else {
                updateStatus = fail
                if (err_dir_not_found == result)
                    updateText.text = "hironic dir is not found.\nPlease make sure hironic dir\nor USB Re-Insert."
                else if (err_file_not_found == result)
                    updateText.text = "FwUpdate file is not found.\nPlease make sure FwUpdate file."
                else
                    updateText.text = "FwUpdate has failed ..."
            }
        }
        onFwProgress: {
            updateText.text = "Firmware updating...("+ progress + "/100)"
        }
        onFwFullVersion: {
            print("onFwFullVersion", main, sono)
            var mainMajor = (main >> 12)
            var mainMiddle = ((main >> 8) & 0x0F)
            var mainDebug = ((main >> 4) & 0x00F)
            var mainTest = (main & 0x00F)
            var sonoMajor = (sono >> 12)
            var sonoMiddle = ((sono >> 8) & 0x0F)
            var sonoDebug = ((sono >> 4) & 0x00F)
            var sonoTest = (sono & 0x00F)

            switch(selectedItem) {
//                case dfMainBD:
                case updateFirmwareMain:
                    // versionOfFiles이 있을 경우.
                    if (typeof versionOfFiles !== "undefined") {
                        // versionOfFiles과 main fw 버전과 같을 경우.
                        if (main.toString(16) === versionOfFiles) {
                            fwMainVersion.text = fwMainVer + mainMajor + "." + mainMiddle + "." + mainDebug + "." + mainTest
                            fwSonoVersion.text = fwSonoVer + sonoMajor + "." + sonoMiddle + "." + sonoDebug + "." + sonoTest
                        }
                        // versionOfFiles과 main fw 버전이 다를 경우.
                        else {
                            fwMainVersion.text = fwMainVer + mainMajor + "." + mainMiddle + "." + mainDebug + "." + mainTest + " / " + "update file ver : " + versionOfFiles
                            fwSonoVersion.text = fwSonoVer + sonoMajor + "." + sonoMiddle + "." + sonoDebug + "." + sonoTest
                        }
                        // initialize
                        versionOfFiles = undefined
                        selectedItem = -1
                    }
                    // versionOfFiles이 없을 경우.
                    else {
                            fwMainVersion.text = fwMainVer + mainMajor + "." + mainMiddle + "." + mainDebug + "." + mainTest
                            fwSonoVersion.text = fwSonoVer + sonoMajor + "." + sonoMiddle + "." + sonoDebug + "." + sonoTest
                    }
                    break;
                case updateFirmwareSono:
                    if (typeof versionOfFiles !== "undefined") {
                        if (sono.toString(16) === versionOfFiles) {
                            fwMainVersion.text = fwMainVer + mainMajor + "." + mainMiddle + "." + mainDebug + "." + mainTest
                            fwSonoVersion.text = fwSonoVer + sonoMajor + "." + sonoMiddle + "." + sonoDebug + "." + sonoTest
                        }
                        else {
                            fwMainVersion.text = fwMainVer + mainMajor + "." + mainMiddle + "." + mainDebug + "." + mainTest
                            fwSonoVersion.text = fwSonoVer + sonoMajor + "." + sonoMiddle + "." + sonoDebug + "." + sonoTest + " / " + "update file ver : " + versionOfFiles
                        }
                        // initialize
                        versionOfFiles = undefined
                        selectedItem = -1
                    }
                    else {
                            fwMainVersion.text = fwMainVer + mainMajor + "." + mainMiddle + "." + mainDebug + "." + mainTest
                            fwSonoVersion.text = fwSonoVer + sonoMajor + "." + sonoMiddle + "." + sonoDebug + "." + sonoTest
                    }
                    break;
                default:
                    fwMainVersion.text = fwMainVer + mainMajor + "." + mainMiddle + "." + mainDebug + "." + mainTest
                    fwSonoVersion.text = fwSonoVer + sonoMajor + "." + sonoMiddle + "." + sonoDebug + "." + sonoTest
                    break;
            }
        }

        onFwCheckedFile: {
            print("onFwCheckedFile", fileName)
            versionOfFiles = fileName
        }
    }


    Image {
        id: popupFwUpdate
        clip: true; cache: false
        x:(800-762)/2; y: (1280-500)/2
        width: 762; height: 500
        source: pngDir + "bg_popup.png"

        Popup {
            id: popup
            implicitWidth: 762
            implicitHeight: 100
            x: 10
            y: 120
            modal: true
            padding: 0.0

            Text {
                id: updateText
                text: qsTr("Firmware updating...")
                x: 50
                width: 352; height: 60
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 25
            }

            onAboutToHide: {
                stopTimer()
            }
            onAboutToShow: {
                closeTimer.start()
            }

            Timer {
                id: closeTimer
                interval: 2000
                repeat: true
                onTriggered: {
                    if (closeTimer.repeat) {
                        switch(updateStatus) {
                            case waiting_for_mcu:
                                // 하이로닉 요청사항으로 FW 업데이트 완료 후 5초대기 후 버전정보 확인 필요. 버전정보 확인 후 업데이트 성공 출력.
                                closeTimer.interval = 5000
                                closeTimer.restart()/*
                                if(selectedItem != dfMainBD)
                                    UserIface.updateFw(selectedItem, false)*/

                                updateStatus = success
                                break;
                            case success:
                                updateText.text = "FwUpdate has succeeded."

                                // serial port re-open
                                UserIface.connectDev()

                                closeTimer.repeat = false
                                closeTimer.restart()
                                break;
                            case fail:
                                // serial port re-open
                                UserIface.connectDev()

                                // FW 업데이트 시 LCD를 껐다 켜므로 팝업 시간이 짧을 경우 팝업 메시지를 볼수 없으므로 인터벌 시간을 임시로 늘린다.
                                closeTimer.interval = 6000
                                closeTimer.repeat = false
                                closeTimer.restart()
                                break;
                        }
                    }
                    else {
                        popup.close()
                        // initialize
                        updateStatus = initializte
                        closeTimer.interval = 2000
                        closeTimer.repeat = true
                        updateText.text = "Firmware updating ..."
                    }
                }
            }
            function stopTimer() {
                closeTimer.stop()
            }
        } // popup



            TextButton
            {
                x: (800-400)/2; y: 97
                width: 400; height: 65;
                text: "MAIN BD"
                textsize: 34
                img_p: pngDir + "btn_s.png";
                img_n: pngDir + "btn_n.png"
                text_p: textColor
                text_n: textColor

                onClicked: {
                    popup.open()
//                    selectedItem = dfMainBD
//                    UserIface.updateFw(selectedItem, false)
                }

            }

            Row {
                spacing : 15
                //x: 50; y: 171 //SKW 2019-03-11 RV, RC, RN 버튼 사용 시 위치
                x: (800-400)/2; y: 171//SKW 2019-03-11 불필요 버튼 제거에 따른 화면 수정
                TextButton
                {
                    //width: 150; height: 65;//SKW 2019-03-11 RV, RC, RN 사용 시 크기
                    width: 400; height: 65;//SKW 2019-03-11 불필요 버튼 제거에 따른 화면 수정
                    text: "RM BD";
                    textsize: 34
                    img_p: pngDir + "btn_popup_s.png";
                    img_n: pngDir + "btn_popup_n.png"
                    text_p: textColor
                    text_n: textColor
                    onClicked: {
                        popup.open()
                        selectedItem = rmBD
                        UserIface.updateFw(selectedItem, false)
                    }
                }

                //SKW 2019-03-11 RV, RC, RN 미사용으로 인하여 주석
//                TextButton
//                {
//                    width: 150; height: 65;
//                    text: "RV BD";
//                    textsize: 34
//                    img_p: pngDir + "btn_popup_s.png";
//                    img_n: pngDir + "btn_popup_n.png"
//                    text_p: textColor
//                    text_n: textColor
//                    onClicked: {
//                        selectedItem = rvBD
//                        UserIface.updateFw(selectedItem, false)
//                    }
//                }

//                TextButton
//                {
//                    width: 150; height: 65;
//                    text: "RN BD";
//                    textsize: 34
//                    img_p: pngDir + "btn_popup_s.png";
//                    img_n: pngDir + "btn_popup_n.png"
//                    text_p: textColor
//                    text_n: textColor
//                    onClicked: {
//                        selectedItem = rnBD
//                        UserIface.updateFw(selectedItem, false)
//                    }
//                }

//                TextButton
//                {
//                    width: 150; height: 65;
//                    text: "RC BD";
//                    textsize: 34
//                    img_p: pngDir + "btn_popup_s.png";
//                    img_n: pngDir + "btn_popup_n.png"
//                    text_p: textColor
//                    text_n: textColor
//                    onClicked: {
//                        selectedItem = rcBD
//                        UserIface.updateFw(selectedItem, false)
//                    }
//                }
            }

            Text {
                id: fwMainVersion
                x: 57; y: 280
                anchors.margins: 15
                font.pixelSize: 25
                font.bold: false
                font.family: "Myriad Pro"
                color: "white"
                text: qsTr(fwMainVer)
            }

            Text {
                id: fwSonoVersion
                x: 57; y: 310
                anchors.margins: 15
                font.pixelSize: 25
                font.bold: false
                font.family: "Myriad Pro"
                color: "white"
                text: qsTr(fwSonoVer)
            }

            Text {
                id: swVersion
                x: 57; y: 340
                anchors.margins: 15
                font.pixelSize: 25
                font.bold: false
                font.family: "Myriad Pro"
                color: "white"
                text: qsTr(swVer)
            }


            TextButton
            {
                id: btnQuit
                width: 290; height: 80;
                x: ((800 - 290) / 2)
                y: 400
                text: "QUIT";
                textsize: 34
                img_p: pngDir + "btn_s.png";
                img_n: pngDir + "btn_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    f_close()
                }
            }

    }


}
