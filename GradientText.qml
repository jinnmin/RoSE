import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Item {
    id: gradiatText
    property rect rTextGeometry
    property alias text: text.text
    property bool bold: true
    property var pixelSize: 26

    Text {
        anchors.fill: parent
        id: text
        text: ""
        font.bold: bold
        font.pixelSize: pixelSize
    }


    Text {
        anchors.fill: parent
        id: text
        text: ""
        font.pixelSize: pixelSize
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        visible: false
    }

    LinearGradient  {
        anchors.fill: text
        source: text
        gradient: Gradient {
            GradientStop { position: 0; color: "#dab8a8" }
            GradientStop { position: 1; color: "#a06d55" }
        }
    }

}


