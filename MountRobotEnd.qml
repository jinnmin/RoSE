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
    id: mountRobotEndContainer
    visible: false
    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true

    MouseArea {
        anchors.fill: parent
    }

    signal close()

///////////// HP Info End //////////////////
    function f_open() {
        visible = true

        sendPageInfo()

    }


    function f_close() {
        visible = false

        close()
    }

    function sendPageInfo() {
        if (mountRobotEndContainer.visible) {

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
            print("1.MountRobot onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)
            if((currentPage != pageAlex) && (currentPage != pageNdyag))
                return;

            print("2.MountRobot onResCmdFromPower  cmd: ", cmd, "getSet: ", getSet, "data1: ", data1, "data2: ", data2, "data3: ", data3, "data4: ", data4, "data5: ", data5, "data6: ", data6)

        }

    }

//////////////////////////////////////////////////////////////////////////////////////

    Rectangle {
        id: mountRobotEndBgRect
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
            text: "Mount the robot at the end of endoscope"
            font.pixelSize: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: textColor
        }

        Image {
            x: (defaultWidth - width) / 2; y: (defaultHeight - height)/2
            width: 734; height: 330
            clip: true; cache: false
            source: pngRoSE + "mount_endoscope_end.png"
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



