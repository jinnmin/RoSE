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
    id: insertCartridgeContainer
    visible: false
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true

    MouseArea {
        anchors.fill: parent
    }

    readonly property int defaultAngle:         360

    signal close()

///////////// HP Info End //////////////////
    function f_open() {
        visible = true

        sendPageInfo()
        sendCommand = cartStatusCmd
        sendInstruction = 1
        sendData1 = sendData2 = sendData3 = sendData4 = sendData5 = sendData6 = 0
        sendCmd(sendCommand, sendInstruction, sendData1, sendData2, sendData3, sendData4, sendData5, sendData6 )

    }


    function f_close() {
        visible = false

        close()
    }

    function sendPageInfo() {
        if (insertCartridgeContainer.visible) {

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
                case cartStatusCmd:
                    if(data1 == 1) {
                        popupRect.visible = true
                        updateText = "Cartridge is disconnted"
                    }
                    else if(data1 == 2) {
                        popupRect.visible = true
                        updateText = "Cartridge is wrong"
                    }

                    break;
            }
        }

    }

//////////////////////////////////////////////////////////////////////////////////////

    Rectangle {
        id: insertCartBgRect
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
            text: "Insert the cartridge "
            font.pixelSize: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: textColor
        }

        Image {
            x: (defaultWidth - width) / 2; y: (defaultHeight - height)/2
            width: 474; height: 408
            clip: true; cache: false
            source: pngRoSE + "main_body.png"
        }

        Image {
            x: 900; y: 430
            width: 116; height: 71
            clip: true; cache: false
            source: pngRoSE + "red_arrow.png"
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



