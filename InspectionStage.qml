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
    id: inspectionStageContainer
    visible: false
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true

    MouseArea {
        anchors.fill: parent
    }

    readonly property int defaultAngle:         360
    readonly property int gripStage:            1
    readonly property int rollStage:            2
    readonly property int endStage:             3

    property int gripTopAngle:                  0
    property int gripBottomAngle:               0
    property int rollAngle:                     0

    property int currentStage:                  gripStage

    signal close()

///////////// HP Info End //////////////////
    function f_open() {
        visible = true
        currentStage = gripStage
        rowInspectionGrip.visible = true
        rowInspectionRoll.visible = false
        gripTimer.start()
        gripTopAngle = 0
        gripBottomAngle = 0
        rollAngle = 0

        print("gripBottomAngle: ", gripBottomAngle)

        btnArmInspectionDone.visible = false
        sendPageInfo()

    }


    function f_close() {
        visible = false
        gripTopAngle = 0
        gripBottomAngle = 0
        rollAngle = 0
        currentStage = gripStage
        testRow.visible = true
        close()
    }

    function sendPageInfo() {
        if (inspectionStageContainer.visible) {

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

            switch(cmd) {
                case angleDataCmd:
//                    joyGripAngle = data1
//                    joyArmAngle = data3
//                    joyRollAngle = data4

//                    gripTopTrainingAngle = data1
//                    gripBottomTrainingAngle = -data1
//                    armTrainingAngle = data3
//                    rollTrainingAngle = data4
                    break;
                default:
                    break;
            }
        }

    }

//////////////////////////////////////////////////////////////////////////////////////

    Rectangle {
        id: inspectionStageBgRect
        visible: true
        anchors.fill: parent
        color: "white"


        Text {
            id: txtInspection
            x: 0; y: 50
            width: defaultWidth; height: 50
            text: "Inspection - grip "
            font.pixelSize: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: textColor
        }

        Row {
            id: rowInspectionGrip
            visible: true
            anchors.centerIn: parent
            spacing: 150

            Rectangle {
                x: 10; y: 10
                width: 300; height: 283
                color: "white"

                Image {
                    x: 115; y: 30
                    source: pngRoSE + "grip_body.png"
                    transform: Rotation { origin.x: 203; origin.y: 27; axis { x: 0; y: 0; z: 1 } angle: gripTopAngle }

                    Image {
                        x:-108; y: 0
                        source: pngRoSE + "grip_bottom1.png"
                        transform: Rotation { origin.x: 125; origin.y: 34; axis { x: 0; y: 0; z: 1 } angle: gripBottomAngle}
                    }

                    Image {
                        x: -100; y: -25
                        source: pngRoSE + "grip_top.png"
                        transform: Rotation { origin.x: 120; origin.y: 37; axis { x: 0; y: 0; z: 1 } angle: gripTopAngle }
                    }
                }


            }

            Image {
                width: 153; height: 283
                clip: true; cache: false
                source: pngRoSE + "interface_body.png"
            }

        }

        Row {
            id: rowInspectionRoll
            visible: false
            anchors.centerIn: parent
            spacing: 150

            Image {
                source: pngRoSE + "roll_bg.png"

                ProgressCircle {
                    x:-5; y: -5
                    size: 310
                    colorCircle: "blue"
                    colorBackground: "#E6E6E6"
                    arcBegin: rollAngle //243
                    arcEnd: rollAngle + 20 //arcBegin + (defaultAngle/6)/pulseDurationMax * pulseDuration
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
            id: gripTimer
            interval:10; running: true; repeat: false
            onTriggered: {
                print("1 in gripTimer gripTopAngle: ", gripTopAngle)
                gripTopAngle++
                gripBottomAngle --

                if(gripTopAngle < 90) {
                    gripTimer.restart()
                    sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )
                }
                else {
                    gripTimer.stop()
                    btnArmInspectionDone.visible = true
                }
            }
        }

        Timer {
            id: rollTimer
            interval: 30; running: true; repeat: false
            onTriggered: {
                print("1 in rollTimer rollAngle: ", rollAngle)
                rollAngle++
                if(rollAngle < 340) {
                    rollTimer.restart()
                }
                else {
                    rollTimer.stop()
                    btnArmInspectionDone.visible = true

                }
            }
        }

        Row {
            id: testRow
            x: 200; y: 620
            visible: true
            spacing: 20

            ImgButton {
                width: 45; height: 45
                img_org: "file:///home/app/res/image/Gentlo/controller_left_s.png"
                img_p: "file:///home/app/res/image/Gentlo/controller_left_s.png"
                img_n: "file:///home/app/res/image/Gentlo/controller_left_s.png"

                onClicked: {
                    gripTimer.interval -= 5
                    if( gripTimer.interval < 5)
                        gripTimer.interval = 5
                }
            }

            Text {
                id: timerTxt
                text: gripTimer.interval + " ms"
                width: 100; height: 45
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.margins: 0
                font.pixelSize:  45

                font.family: "Myriad Pro"
                color: textColor
            }

            ImgButton {
                width: 45; height: 45
                img_org: "file:///home/app/res/image/Gentlo/controller_right_s.png"
                img_p: "file:///home/app/res/image/Gentlo/controller_right_s.png"
                img_n: "file:///home/app/res/image/Gentlo/controller_right_s.png"

                onClicked: {
                    gripTimer.interval += 5
                }
            }

        }

        TextButton
        {
            visible: testRow.visible
            x: 210; y: 700
            width: 200; height: 65;
            text: "TEST"
            textsize: 34
            img_p: pngDir + "btn_s.png";
            img_n: pngDir + "btn_n.png"
            text_p: textColor
            text_n: textColor

            onClicked: {
                gripTopAngle = 0
                gripBottomAngle = 0
                gripTimer.restart()

            }

        }

        TextButton
        {
            id: btnArmInspectionDone
            x: 850; y: 650
            width: 300; height: 65;
            visible: false
            text: "DONE"
            textsize: 34
            img_p: pngDir + "btn_s.png";
            img_n: pngDir + "btn_n.png"
            text_p: textColor
            text_n: textColor

            onClicked: {
                if(currentStage == gripStage) {
                    gripTimer.stop()

                    rowInspectionGrip.visible = false
                    currentStage = rollStage
                    rowInspectionRoll.visible = true
                    testRow.visible = false
                    rollTimer.start()

                }
                else if(currentStage == rollStage) {
                    currentStage = endStage
                    rowInspectionRoll.visible = true
                    rollAngle = 0
                    rollTimer.stop()
                    f_close()

                }
                else {
                    f_close()
                }

            }

        }


    }

}



