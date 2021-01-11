import useriface 1.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: customtext
    property rect rTextGeometry
    property alias text: text.text
    property bool bold: true
    property var pixelSize: 30

    Text {
        anchors.fill: parent
        id: text
        text: ""
        font.bold: bold
        font.pixelSize: pixelSize
    }

    onRTextGeometryChanged: {
        customtext.x = rTextGeometry.x
        customtext.y = rTextGeometry.y
        customtext.width = rTextGeometry.width
        customtext.height = rTextGeometry.height
    }

}

