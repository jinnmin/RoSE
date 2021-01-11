import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Image {
    property string img_n
    property string img_p
    property string text
    property color text_n
    property color text_p
    property int textsize

    clip: true; cache: false
    source: img_n

    signal clicked()
    Text {
        anchors.fill: parent;
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter;
        text: parent.text
        font.pixelSize: parent.textsize
        font.weight: Font.DemiBold
        font.family: "Myriad Pro"
        color: text_n; id: i_text
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            i_text.color = text_p
            parent.source = img_p
        }
        onReleased: {
            i_text.color = text_n
            parent.source = img_n
            parent.clicked()
        }
    }
}
