import QtQuick 2.5
import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.controls 1.0
import QtQuick.Extras 1.4
import QtGraphicalEffects 1.0
import "."

Item {
    id: mainControlContainer
    visible: false
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true

    MouseArea {
        anchors.fill: parent
    }

    readonly property int defaultAngle:         360

    property string dotPng:                     ".png"

    signal close()

///////////// HP Info End //////////////////
    function f_open() {
        visible = true

        sendPageInfo()

        i_insert_cartridge.f_open()

    }


    function f_close() {
        visible = false

        close()
    }

    function sendPageInfo() {
        if (mainControlContainer.visible) {

            currentPage = pageAlex

print("mainControlContainer currentPage: ", currentPage);
            UserIface.setPageNo(currentPage)
        }
    }

    function setImageSource(objId, _source) {
        objId.source = _source
    }


    Connections {
        target: UserIface

        onConnectedHP: {
            print("MainSettings onConnectedHP check ", currentPage, pageAlex)
        }


        onResCmdFromPower: {
            print("1.Main onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)
            if((currentPage != pageAlex) && (currentPage != pageNdyag))
                return;

            print("2.Main onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)

            receivedCmdTxt.text = cmd
            receivedSetTxt.text = getSet
            receivedData1Txt.text = data1
            receivedData2Txt.text = data2
            receivedData3Txt.text = data3
            receivedData4Txt.text = data4
            receivedData5Txt.text = data5
            receivedData6Txt.text = data6
        }

        onAlramAlert: {
            print("MainSettings onAlram", alarmCode, "alarmHP ", alarmHP, "currentPage: ", currentPage);
            if((currentPage != pageAlex) && (currentPage != pageNdyag))
                return

//            popupRect.visible = true

            switch(alarmCode) {

                default:
                    updateTitleText.text = "Warning"
                    updateText.text = "HP : " + alarmHPString +  "\nUnregistered Alram Code(" + alarmCode + ")";
                    break;
            }
        }

    }

//////////////////////////////////////////////////////////////////////////////////////

    Rectangle {
        id: mainContainerBgRect
        visible: true
        anchors.fill: parent
        color:bgColor

        Image{
            id: topBtnHomeIcon
            x: 76
            width: 76; height: 70
            clip: true; cache: false
            source: pngDir + "btn_home_n.png"
            MouseArea {
                anchors.fill: topBtnHomeIcon
                onClicked: {
                    popupOperationEndRect.visible = true
                }
            }
        }

        Column {
            x: 100; y: 100
            spacing : 20

            Row {
                spacing: 20

                Text {
                    width: 250; height: 50
                    text: "Command: "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Image {
                    clip: true; cache: false
                    width: 250; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputCmdTxt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("Command")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputCmdTxt
                            onClicked: {
                                qmlf_keypad("inputCmdTxt", "numonly", inputCmdTxt, 3)
                            }
                        }
                    }
                }
            }

            Row {
                spacing: 20

                Text {
                    width: 250; height: 50
                    text: "Set/Get: "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Image {
                    clip: true; cache: false
                    width: 250; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputSetTxt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("1.Read 2.Write")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputSetTxt
                            onClicked: {
                                qmlf_keypad("inputSetTxt", "numonly", inputSetTxt, 1)
                            }
                        }
                    }
                }
            }

            Row {
                spacing: 20

                Text {
                    width: 250; height: 50
                    text: "Data: "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Image {
                    clip: true; cache: false
                    width: 100; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputData1Txt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("Data 1")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputData1Txt
                            onClicked: {
                                qmlf_keypad("inputData1Txt", "numonly", inputData1Txt, 3)
                            }
                        }
                    }
                }

                Image {
                    clip: true; cache: false
                    width: 100; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputData2Txt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("Data 2")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputData2Txt
                            onClicked: {
                                qmlf_keypad("inputData2Txt", "numonly", inputData2Txt, 3)
                            }
                        }
                    }
                }

                Image {
                    clip: true; cache: false
                    width: 100; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputData3Txt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("Data 3")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputData3Txt
                            onClicked: {
                                qmlf_keypad("inputData3Txt", "numonly", inputData3Txt, 3)
                            }
                        }
                    }
                }

                Image {
                    clip: true; cache: false
                    width: 100; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputData4Txt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("Data 4")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputData4Txt
                            onClicked: {
                                qmlf_keypad("inputData4Txt", "numonly", inputData4Txt, 3)
                            }
                        }
                    }
                }

                Image {
                    clip: true; cache: false
                    width: 100; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputData5Txt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("Data 5")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputData5Txt
                            onClicked: {
                                qmlf_keypad("inputData5Txt", "numonly", inputData5Txt, 3)
                            }
                        }
                    }
                }

                Image {
                    clip: true; cache: false
                    width: 100; height: 50
                    source: pngDir + "input_search.png"

                    TextInput {
                        id: inputData6Txt;
                        anchors.fill: parent
                        font.pixelSize: 34
                        font.bold: false
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignHCenter
                        text: qsTr("Data 6")
                        color: guideTextColor
                        font.family: "Myriad Pro"
                        cursorVisible: false

                        MouseArea {
                            anchors.fill: inputData6Txt
                            onClicked: {
                                qmlf_keypad("inputData6Txt", "numonly", inputData6Txt, 3)
                            }
                        }
                    }
                }

            }

            TextButton
            {
                width: 400; height: 65;
                text: "SEND"
                textsize: 34
                img_p: pngDir + "btn_s.png";
                img_n: pngDir + "btn_n.png"
                text_p: textColor
                text_n: textColor

                onClicked: {
                    sendCommand = parseInt(inputCmdTxt.text)
                    sendInstruction = parseInt(inputSetTxt.text)
                    sendData1 = parseInt(inputData1Txt.text)
                    sendData2 = parseInt(inputData2Txt.text)
                    sendData3 = parseInt(inputData3Txt.text)
                    sendData4 = parseInt(inputData4Txt.text)
                    sendData5 = parseInt(inputData5Txt.text)
                    sendData6 = parseInt(inputData6Txt.text)

                    sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )

                    sendData1 = sendData2 = sendData3 = sendData4 = sendData5 = sendData6 = 0
                }

            }



            Row {
                spacing: 20

                Text {
                    width: 250; height: 50
                    text: " Received Command: "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedCmdTxt
                    width: 250; height: 50
                    text: " CMD "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }
            }

            Row {
                spacing: 20

                Text {
                    width: 250; height: 50
                    text: " Received Get/Set: "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedSetTxt
                    width: 250; height: 50
                    text: " Get/Set "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }
            }

            Row {
                spacing: 20

                Text {
                    width: 250; height: 50
                    text: " Received Data: "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedData1Txt
                    width: 100; height: 50
                    text: "data 1"
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedData2Txt
                    width: 100; height: 50
                    text: " data 2 "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedData3Txt
                    width: 100; height: 50
                    text: "data 3"
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedData4Txt
                    width: 100; height: 50
                    text: "data 4"
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedData5Txt
                    width: 100; height: 50
                    text: " data 5 "
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }

                Text {
                    id: receivedData6Txt
                    width: 100; height: 50
                    text: "data 6"
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    color: inputTextColor
                }
            }
        }


    }

    Rectangle {
        id: popupOperationEndRect
        visible: false
        width: 722; height: 444
        x: ((defaultWidth - popupOperationEndRect.width) / 2)
        y: ((defaultHeight - popupOperationEndRect.height) / 2)
        color: "transparent"

        MouseArea {
            x: loadingContainer.x ; y: loadingContainer.y
            width: defaultWidth; height: defaultHeight
        }

        Rectangle {
            id:popupOperationEndOpacityRect
            x: -popupOperationEndRect.x ; y: -popupOperationEndRect.y
            width: defaultWidth; height: defaultHeight
            color: "#000000"
            opacity: 0.5
            MouseArea {
                anchors.fill: parent
            }
        }

        Image {
            id: popupOperationEndImgbg
            width: 722; height: 444
            clip: true; cache: false
            source: pngDir + "bg_popup.png"
        }

        Text {
            id: operationEndTitleText
            text: qsTr("Operation")
            x: 0
            y: 64
            width: 722; height: 46
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize:  34
            color: popupTitleColor
        }

        Text {
            id: operationEndText
            text: qsTr("Are you sure to finish the operation?")
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
                popupOperationEndRect.visible = false
                f_close()
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
                popupOperationEndRect.visible = false
            }
        }

    }

    InsertCartridge {
        id: i_insert_cartridge
        onClose: {
            print("i_insert_cartridge close mainContainerBgRect.visible: ", mainContainerBgRect.visible)
            i_inspection_stage.f_open()
        }
    }

    InspectionStage {
        id: i_inspection_stage
        onClose: {
            print("i_inspection_stage close mainContainerBgRect.visible: ", mainContainerBgRect.visible)
            i_mount_robot.f_open()
        }
    }

    MountRobot {
        id: i_mount_robot
        onClose: {
            print("i_mount_robot close mainContainerBgRect.visible: ", mainContainerBgRect.visible)
            i_mount_robot_end.f_open()
        }
    }

    MountRobotEnd {
        id: i_mount_robot_end
        onClose: {
            print("i_mount_robot_end close mainContainerBgRect.visible: ", mainContainerBgRect.visible)

        }
    }
    FunctionUtility {
        id: i_function
    }

}



