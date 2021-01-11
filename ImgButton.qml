import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Image {
    id: i_img_button
    clip: true; cache: false
    property url img_org
    property url img_n
    property url img_p
    source: img_org
//    source: img_n

    signal pressed()
    signal clicked()
    signal released()
    signal exited()
    signal pressAndHold()

    MouseArea {
        anchors.fill: parent
        onPressed: {
            parent.source = parent.img_p
            i_img_button.clicked()
        }

        onClicked: {
            parent.source = parent.img_n
        }

        onReleased: {
            parent.source = parent.img_n
            i_img_button.released()

        }
        onPressAndHold: {
            i_img_button.pressAndHold()
        }
    }
}
