import useriface 1.0
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: reportSettingContainer
    visible: false
    clip: true

    x: 0; y: 70
    width: defaultWidth; height: defaultHeight - y

    MouseArea {
        x: 0; y: 70+78
        width: defaultWidth; height: defaultHeight - y
//        anchors.fill: parent
    }


    function f_open() {
        visible = true

    }

    function f_close() {
        visible = false
        close()
    }

    signal close()

    Rectangle {
        visible: true
        y: 78
        width: defaultWidth; height: defaultHeight - 70 - 78
        color:settingBgColor

        Row {
            spacing: 30
            x: 226; y: 123

            ImgButton {
                id: btnMemory
                width: 256; height: 193;
                img_org:pngDir + "btn_report_1_n.png"
                img_p: pngDir + "btn_report_1_s.png"
                img_n: pngDir + "btn_report_1_n.png"
                onClicked: {
                    dfitHpSettingsTxt.text = "Presetting Memory"
//                    i_status_report.f_open()
                }
            }

            ImgButton {
                id: btnLogFiles
                width: 256; height: 193;
                img_org:pngDir + "btn_report_2_n.png"
                img_p: pngDir + "btn_report_2_s.png"
                img_n: pngDir + "btn_report_2_n.png"
                onClicked: {
                    dfitHpSettingsTxt.text = "Log Files"
//                    i_status_report.f_open()
                }
            }

            ImgButton {
                id: btnStatus
                width: 256; height: 193;
                img_org:pngDir + "btn_report_3_n.png"
                img_p: pngDir + "btn_report_3_s.png"
                img_n: pngDir + "btn_report_3_n.png"
                onClicked: {
                    dfitHpSettingsTxt.text = "Status Report"
//                    i_status_report.f_open()
                }
            }
        }

        Row {
            spacing: 30
            x: 226+143; y: 123+193+30

            ImgButton {
                id: btnError
                width: 256; height: 193;
                img_org:pngDir + "btn_report_4_n.png"
                img_p: pngDir + "btn_report_4_s.png"
                img_n: pngDir + "btn_report_4_n.png"
                onClicked: {
                    dfitHpSettingsTxt.text = "Error history"
//                    i_status_report.f_open()
                }
            }

            ImgButton {
                id: btnSoftware
                width: 256; height: 193;
                img_org:pngDir + "btn_report_5_n.png"
                img_p: pngDir + "btn_report_5_s.png"
                img_n: pngDir + "btn_report_5_n.png"
                onClicked: {
                    dfitHpSettingsTxt.text = "Software Version Report"
//                    i_status_report.f_open()
                }
            }

        }
    }

}

