import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    property bool check: false
    property string img_checked
    property string img_unchecked
    property string text
    property int text_margin
    property int text_y
    property int text_w
    property int text_h
    property int img_w
    property int img_h
    property color text_checked_color
    property color text_unchecked_color
    property int pixelSize
    signal checked(bool val)

    onCheckChanged: checked(check)

    width: text_w + text_margin + img_w; height: img_h
    Image {
        id: i_img; width: img_w; height: img_h
        source: check? img_checked : img_unchecked
        clip: true; cache: false
    }
    Text { anchors.left: i_img.right; anchors.leftMargin: text_margin
        width: text_w; height: text_h; font.pixelSize: parent.pixelSize
        color: check? text_checked_color : text_unchecked_color
        verticalAlignment: Text.AlignVCenter; y: text_y
        text: parent.text
    }
    MouseArea {
        anchors.fill: parent; onClicked: {
            parent.check = !check
        }
    }
}
