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
    id: trainingMenuContainer
    visible: false
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true

    MouseArea {
        anchors.fill: parent
    }

    readonly property int defaultAngle:                 360
    property int gripTopTrainingAngle:                  0
    property int gripBottomTrainingAngle:               0
    property int armTrainingAngle:                      0
    property int rollTrainingAngle:                     0

    property int joyGripAngle:                          0
    property int joyArmAngle:                           0
    property int joyRollAngle:                          0

    signal close()

///////////// HP Info End //////////////////
    function f_open() {
        visible = true

        sendPageInfo()
        rollTimer1.start()
        gripTopTrainingAngle = 0
        gripBottomTrainingAngle = 0
        rollTrainingAngle = 0

//        sendCommand = cartStatusCmd
//        sendInstruction = 1
//        sendData1 = sendData2 = sendData3 = sendData4 = sendData5 = sendData6 = 0
//        sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )

    }


    function f_close() {
        visible = false
        rollTimer1.stop()
        gripTopTrainingAngle = 0
        gripBottomTrainingAngle = 0
        rollTrainingAngle = 0
        close()
    }

    function sendPageInfo() {
        if (trainingMenuContainer.visible) {

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
            print("1.Training onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)
//            if((currentPage != pageAlex) && (currentPage != pageNdyag))
//                return;

            print("2.Training onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)

            switch(cmd) {
                case angleDataCmd:
                    joyGripAngle = data1
                    joyArmAngle = data3
                    joyRollAngle = data4

                    gripTopTrainingAngle = data1
                    gripBottomTrainingAngle = -data1
                    armTrainingAngle = data3
                    rollTrainingAngle = data4
                    break;
                default:
                    break;
            }
        }

    }

//////////////////////////////////////////////////////////////////////////////////////

    Rectangle {
        id: trainingMenuBgRect
        visible: true
        anchors.fill: parent
        color: "white"

        Image {
            anchors.fill: parent
            clip: true; cache: false
            source: pngRoSE + "endo_bg.png"
        }

        Text {
            x: 0; y: 50
            width: defaultWidth; height: 50
            text: "Training"
            font.pixelSize: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: textColor
        }

        Row {
            spacing: 100
            anchors.centerIn: parent

            Rectangle {
                x: 10; y: 10
                width: 300; height: 283
                color: "white"

                Image {
                    x: 115; y: 30
                    source: pngRoSE + "grip_body.png"
                    transform: Rotation { origin.x: 203; origin.y: 27; axis { x: 0; y: 0; z: 1 } angle: armTrainingAngle }

                    Image {
                        x:-108; y: 0
                        source: pngRoSE + "grip_bottom1.png"
                        transform: Rotation { origin.x: 125; origin.y: 34; axis { x: 0; y: 0; z: 1 } angle: gripBottomTrainingAngle}
                    }

                    Image {
                        x: -100; y: -25
                        source: pngRoSE + "grip_top.png"
                        transform: Rotation { origin.x: 120; origin.y: 37; axis { x: 0; y: 0; z: 1 } angle: gripTopTrainingAngle }
                    }
                }

            }


            Image {
                source: pngRoSE + "roll_bg.png"

                ProgressCircle {
                    x:-5; y: -5
                    size: 310
                    colorCircle: "blue"
                    colorBackground: "#E6E6E6"
                    arcBegin: rollTrainingAngle
                    arcEnd: rollTrainingAngle + 20
                    lineWidth: 20
                }
            }

            Image {
                width: 153; height: 283
                clip: true; cache: false
                source: pngRoSE + "interface_body.png"
            }
        }


        Timer {
            id: rollTimer1
            interval: 30; running: true; repeat: false
            onTriggered: {
                print("2 in rollTimer1 rollTrainingAngle: ", rollTrainingAngle)
                rollTrainingAngle++

                if(rollTrainingAngle > 359 )
                    rollTrainingAngle = 0

//                if ((rollTrainingAngle > 240) && (rollTrainingAngle < 360)) {
//                    rollTimer1.restart()
//                }
//                else if (rollTrainingAngle < 120) {
//                    rollTimer1.restart()
//                }
//                else if (rollTrainingAngle == 360) {
//                    rollTrainingAngle = 0
//                    rollTimer1.restart()
//                }
//                else if (rollTrainingAngle == 120) {
//                    rollTrainingAngle = 240
//                    rollTimer1.stop()
//                }

                joyRollAngle = rollTrainingAngle/3
                print("33 in rollTimer1 rollTrainingAngle: ", rollTrainingAngle)


                if(gripTopTrainingAngle < 90) {
                    gripTopTrainingAngle++
                    gripBottomTrainingAngle --
                    joyGripAngle = gripTopTrainingAngle/3
                    joyArmAngle = gripTopTrainingAngle/3

                }
                else {
                    joyGripAngle = 0
                    joyArmAngle = 0
                    gripTopTrainingAngle = 0
                    gripBottomTrainingAngle = 0
                }

//                sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )

            }
        }

        Row {
            x: 800; y: 100
            spacing: 80

            Image {
//                x:-108; y: 0
                id: btnGripImg
                width: 100; height: 100
                source: pngRoSE + "interface_grip.png"
                transform: Rotation { origin.x: 80; origin.y: 80; axis { x: 0; y: 0; z: 1 } angle: joyGripAngle}

                MouseArea {
                    anchors.fill: btnGripImg
                    onClicked: {
                        if(gripTopTrainingAngle == 90) {
                            gripTopTrainingAngle = 0
                            gripBottomTrainingAngle = 0
                        }
                        else {
                            gripTopTrainingAngle ++
                            gripBottomTrainingAngle --
                        }

                        joyGripAngle = gripTopTrainingAngle / 3


                        sendCommand = angleDataCmd
                        sendData1 = gripTopTrainingAngle
                        sendData2 = gripBottomTrainingAngle
                        sendData3 = armTrainingAngle
                        sendData4 = rollTrainingAngle
                        sendData5 = 0
                        sendData6 = 0

                        sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )

                    }

//                    onPressAndHold: {
//                        if(gripTopTrainingAngle == 90) {
//                            gripTopTrainingAngle = 0
//                            gripBottomTrainingAngle = 0
//                        }
//                        else {
//                            gripTopTrainingAngle ++
//                            gripBottomTrainingAngle --
//                        }

//                        joyGripAngle = gripTopTrainingAngle / 3

//                    }
                }
            }

            Image {
//                x:-108; y: 0
                id: btnArmImg
                width: 31; height: 103
                source: pngRoSE + "interface_arm.png"
                transform: Rotation { origin.x: 15; origin.y: 100; axis { x: 0; y: 0; z: 1 } angle: joyArmAngle}

                MouseArea {
                    anchors.fill: btnArmImg
                    onClicked: {
                        if(armTrainingAngle == 90){
                            armTrainingAngle = 0
                        }
                        else {
                            armTrainingAngle ++
                        }

                        joyArmAngle = armTrainingAngle/3

                        sendCommand = angleDataCmd
                        sendData1 = gripTopTrainingAngle
                        sendData2 = gripBottomTrainingAngle
                        sendData3 = armTrainingAngle
                        sendData4 = rollTrainingAngle
                        sendData5 = 0
                        sendData6 = 0
                        sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )

                    }

                    onPressAndHold: {
//                        if(armTrainingAngle == 90){
//                            armTrainingAngle = 0
//                        }
//                        else {
//                            armTrainingAngle ++
//                        }

//                        joyArmAngle = armTrainingAngle/3

                    }
                }
            }

            Image {
                id: btnRollImg
                width: 31; height: 103
                source: pngRoSE + "interface_wrist.png"
                transform: Rotation { origin.x: 15; origin.y: 100; axis { x: 0; y: 0; z: 1 } angle: joyRollAngle}

                MouseArea {
                    anchors.fill: btnRollImg
                    onClicked: {
                        if(rollTrainingAngle == 360)
                            rollTrainingAngle = 0
                        else
                            rollTrainingAngle ++

                        joyRollAngle = rollTrainingAngle / 3

                        sendCommand = angleDataCmd
                        sendData1 = gripTopTrainingAngle
                        sendData2 = gripBottomTrainingAngle
                        sendData3 = armTrainingAngle
                        sendData4 = rollTrainingAngle
                        sendData5 = 0
                        sendData6 = 0

                        print("3. sendData1: ", sendData1, "sendData2: ", sendData2, "sendData3: ", sendData3, "sendData4: ",sendData4)

                        sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )

                    }
                    onPressAndHold: {
                        if(rollTrainingAngle == 360)
                            rollTrainingAngle = 0
                        else
                            rollTrainingAngle ++

                        joyRollAngle = rollTrainingAngle / 3
                    }
                }
            }
        }

        TextButton
        {
            x: 850; y: 650
            width: 300; height: 65;
            text: "DONE"
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



