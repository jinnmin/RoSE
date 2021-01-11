import useriface 1.0
import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "."

Item {
    id: timeSettingContainer

    x: 0; y: 0
    width: defaultWidth; height: defaultHeight
    clip: true
///////////////////// 2018-09-17 Text Edit
    property int editField : 0
    readonly property int firstNameEdit:    0
    readonly property int lastNameEdit:     1
    readonly property int birthMonthEdit:   2
    readonly property int birthDayEdit:     3
    readonly property int birthYearEdit:    4
    readonly property int heightEdit:       5
    readonly property int weightEdit:       6


    property int mYear
    property int mMonth
    property int mDay
    property int mHour
    property int mMinute
    property int mSec

    property int lastPage : 0

    //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
    property bool popupRectSignal : false

///////////////////////////////////

    /*
     * z-order of mainContainer is 2
     * main(z-order(1)) - mainContainer(z-order(2))
     */

    signal mainSettingExit();

    function sendPageInfo() {
        if (timeSettingContainer.visible) {
            print("8. page TimeSettings before setPageNo");

            UserIface.getCurrPage()

            UserIface.setPageNo(pageTimeSetting)
        }
    }

    function setImageSource(objId, _source) {
        objId.source = _source
    }

    function activateCursor(objId) {
        print("TimeSetting  ****** activateCursor")
        inputMonthTxt.cursorVisible = false
        inputDayTxt.cursorVisible = false
        inputYearTxt.cursorVisible = false
        inputHourTxt.cursorVisible = false
        inputMinTxt.cursorVisible = false

        objId.cursorVisible = true;

    }

    function setInputTextColor(objId) {
        print("TimeSetting  ****** setInputTextColor")

        inputMonthTxt.color = guideTextColor
        inputDayTxt.color = guideTextColor
        inputYearTxt.color = guideTextColor
        inputHourTxt.color = guideTextColor
        inputMinTxt.color = guideTextColor

        objId.color = inputTextColor;

    }

    function setInputTextClear(objId) {
        print("TimeSetting  ****** setInputTextClear")

        objId.text = ""

    }
/////// GENTLO //////////////////////

    function qmlf_keypad(name, type, obj, num)
    {
        print("TimeSetting  ****** Click 1 qmlf_keypad obj.text: " + obj.text)
        qmlf_keypad_text_clear()
        obj.text = ""
        i_keypad.f_open_ex(name, type, obj, num)
    }

    function qmlf_keypad_text_clear()
    {
        i_keypad.text = ""
    }
    function qmlf_keypad_close()
    {
        i_keypad.visible = false
    }

    function setCurrentDateTime(year, month, day, hour, minute) {
        print(year, ":", month, ":", day)

        inputYearTxt.text = year
        inputMonthTxt.text = month
        inputDayTxt.text = day

        if(hour < 10)
            inputHourTxt.text = "0" + hour
        else
            inputHourTxt.text = hour  // Set Current time

        if(minute < 10)
            inputMinTxt.text = "0" + minute
        else
            inputMinTxt.text = minute  // Set Current time


        return ; // KZ_UI_DEBUG
    }

    Connections {
        target: UserIface
        onResSelectPageInfo: {
            print("TimeSettings onResSelectPageInfo", currPage)
            lastPage = currPage

            UserIface.getTime()

        }

        onResFwSetDate: {
            print("onResFwSetDate", status);
            if (status) {
                UserIface.getTime();
            }
            else timeSettingStatus = status
        }

        onReqPsInfoCurTime: { // 2018-05-10 for Factory
            print("currYear", currYear)
            currDate(currYear, currMon, currDay)
        }

        onReqGetInfoCurTime: {
            print("currYear", currYear)
            print("currYear", currMon)
            print("currYear", currDay)
            print("currYear", currHour)
            print("currYear", currMinute)

            mYear = "20" + currYear
            mMonth = currMon
            mDay = currDay
            mHour = currHour
            mMinute = currMinute

            setCurrentDateTime(mYear, mMonth, mDay, mHour, mMinute)
        }
    }

    Loader {
        id: layerLoader
        property bool valid: item !== null
    }

    Connections {
        onMainSettingExit: {
            print(layerLoader.item + "pageTimeSettingInfo is deactived")
            layerLoader.active = false
            homeContainer.targetExit();
        }
    }

    Rectangle {
        id: mainContainerBgRect
        visible: true
        anchors.fill: parent
        color:bgColor


        ImgButton {
            id: btnHomeImage
            width: 76; height: 70;
            img_org: pngDir + "btn_home_n.png"
            img_p: pngDir + "btn_home_s.png";
            img_n: pngDir + "btn_home_n.png"
            onClicked: {
                print("HP Settings btnHomeImage")
                timeSettingContainer.mainSettingExit()
            }
        }

        Image { 
            id: topLogoIcon
            x: 110; y: 0
            width: 172; height: 98
            clip: true; cache: false
            source: pngDir + "top_logo.png"
        }

        Text {
            id: patientInfoLogoTxt
            x: 20+80+10+172
            width: 478; height: 98
            text: "Settings"
            color: logoTextColor
            font.pixelSize:  34
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.family: "Myriad Pro"
        }

        Rectangle {
            id: timeSettingBgRect
            visible: true
            x:0; y:140
            width: defaultWidth; height: defaultHeight-140
            color:bgColor

            Image { 
                id: timeSettingImg
                x: 0; y: 0
                width: defaultWidth; height:  360
                clip: true; cache: false
                source: pngDir + "bg_timeset.png"
            }

            Row {
                spacing: 18
                x: 95; y: 124

                Image { 
                    id: monthInputImg
                    width: 192; height:  56
                    clip: true; cache: false
                    source: pngDir + "input_date.png"
                }

                Image { 
                    id: dayInputImg
                    width: 192; height:  56
                    clip: true; cache: false
                    source: pngDir + "input_date.png"
                }

                Image { 
                    id: yearInputImg
                    width: 192; height:  56
                    clip: true; cache: false
                    source: pngDir + "input_date.png"
                }
            }

            Row {
                spacing: 18
                x: 115; y: 124

                TextInput {
                    id: inputMonthTxt
                    width: 192; height: 56
                    text: qsTr("00")
                    color: guideTextColor
                    font.pixelSize:  30
                    maximumLength: 2
                    horizontalAlignment: TextInput.AlignLeft
                    verticalAlignment: TextInput.AlignVCenter
                    font.family: "Myriad Pro"
                    cursorVisible: activateCursor(inputMonthTxt)  // false

                    MouseArea {
                        anchors.fill: inputMonthTxt
                        onPressed: {
                            setInputTextColor(inputMonthTxt)
                        }
                        onClicked: {
                            setInputTextClear(inputMonthTxt)
                            qmlf_keypad_close()
                            activateCursor(inputMonthTxt)
                            qmlf_keypad("month_txt_input", "numonly", inputMonthTxt, 2)
                        }
                    }
                }

                TextInput {
                    id: inputDayTxt
                    width: 192; height: 56
                    text: qsTr("00")
                    color: guideTextColor
                    font.pixelSize:  30
                    maximumLength: 2
                    horizontalAlignment: TextInput.AlignLeft
                    verticalAlignment: TextInput.AlignVCenter
                    font.family: "Myriad Pro"
                    cursorVisible: activateCursor(inputDayTxt)  // false

                    MouseArea {
                        anchors.fill: inputDayTxt
                        onPressed: {
                            setInputTextColor(inputDayTxt)
                        }
                        onClicked: {
                            setInputTextClear(inputDayTxt)
                            qmlf_keypad_close()
                            activateCursor(inputDayTxt)
                            qmlf_keypad("day_txt_input", "numonly", inputDayTxt, 2)
                        }
                    }
                }

                TextInput {
                    id: inputYearTxt
                    width: 192; height: 56
                    text: qsTr("0000")
                    color: guideTextColor
                    font.pixelSize:  30
                    maximumLength: 4
                    horizontalAlignment: TextInput.AlignLeft
                    verticalAlignment: TextInput.AlignVCenter
                    font.family: "Myriad Pro"
                    cursorVisible: activateCursor(inputYearTxt)  // false

                    MouseArea {
                        anchors.fill: inputYearTxt
                        onPressed: {
                            setInputTextColor(inputYearTxt)
                        }
                        onClicked: {
                            setInputTextClear(inputYearTxt)
                            qmlf_keypad_close()
                            activateCursor(inputYearTxt)
                            qmlf_keypad("year_txt_input", "numonly", inputYearTxt, 4)
                        }
                    }
                }
            }

            Row {
                spacing: 38
                x: 95; y: 124+56+84

                Image { 
                    id: hourInputImg
                    width: 287; height:  56
                    clip: true; cache: false
                    source: pngDir + "input_time.png"
                }

                Image { 
                    id: minuteInputImg
                    width: 287; height:  56
                    clip: true; cache: false
                    source: pngDir + "input_time.png"
                }
            }

            Row {
                spacing: 38
                x: 115; y: 124+56+84

                TextInput {
                    id: inputHourTxt
                    width: 287; height: 56
                    text: qsTr("00")
                    color: guideTextColor
                    font.pixelSize:  30
                    maximumLength: 2
                    horizontalAlignment: TextInput.AlignLeft
                    verticalAlignment: TextInput.AlignVCenter
                    anchors.margins: 20
                    font.family: "Myriad Pro"
                    cursorVisible: activateCursor(inputHourTxt)  // false

                    MouseArea {
                        anchors.fill: inputHourTxt
                        onPressed: {
                            setInputTextColor(inputHourTxt)
                        }
                        onClicked: {
                            setInputTextClear(inputHourTxt)
                            qmlf_keypad_close()
                            activateCursor(inputHourTxt)
                            qmlf_keypad("hour_txt_input", "numonly", inputHourTxt, 2)
                        }
                    }
                }

                TextInput {
                    id: inputMinTxt
                    width: 287; height: 56
                    text: qsTr("00")
                    color: guideTextColor
                    font.pixelSize:  30
                    maximumLength: 2
                    horizontalAlignment: TextInput.AlignLeft
                    verticalAlignment: TextInput.AlignVCenter
                    anchors.margins: 20
                    font.family: "Myriad Pro"
                    cursorVisible: activateCursor(inputMinTxt)  // false

                    MouseArea {
                        anchors.fill: inputMinTxt
                        onPressed: {
                            setInputTextColor(inputMinTxt)
                        }
                        onClicked: {
                            setInputTextClear(inputMinTxt)
                            qmlf_keypad_close()
                            activateCursor(inputMinTxt)
                            qmlf_keypad("min_txt_input", "numonly", inputMinTxt, 2)
                        }
                    }
                }

            }
        }

    }





///// BACK & SAVE

        Image { 
            id: btnBackImg
            x: 83; y: 98+1046
            width: 330; height: 84
            clip: true; cache: false
            source: pngDir + "btn_n.png"

            Text {
                id: btnBackTxt
                text: qsTr("BACK")
                x:40; y: 19
                width: 250; height: 40
                horizontalAlignment: TextInput.AlignHCenter
                anchors.margins: 0
                font.pixelSize:  34
                font.family: "Myriad Pro"
                color: textColor
            }

            MouseArea {
                anchors.fill: btnBackImg
                onClicked: {
                    setSelectPage(pageLoader, "qrc:/SettingsCommon.qml")
                    UserIface.stopAudioGuide() //SKW 2019-01-28 Stop Audio guide
                }
                onPressed: { setImageSource(btnBackImg, pngDir + "btn_s.png") }
                onReleased: { setImageSource(btnBackImg, pngDir + "btn_n.png") }
            }
        }

        Image { 
            id: btnSaveImg
            x: btnBackImg.x + 330 + 14; y: 1144
            width: 330; height: 84
            clip: true; cache: false
            source: pngDir + "btn_n.png"

            Text {
                id: btnSaveTxt
                text: qsTr("DONE")
                x:40; y: 19
                width: 250; height: 40
                horizontalAlignment: TextInput.AlignHCenter
                anchors.margins: 0
                font.pixelSize:  34
                font.family: "Myriad Pro"
                color: textColor
            }

            MouseArea {
                anchors.fill: btnSaveImg
                onClicked: {
                    if(popupRectSignal) { //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력 (현재 페이지에 머물도록)
                        print(" Day error ")
                        popupRect.visible = true
                        updateText.text = " Day error "
                    } else {
                        mYear -= 2000
                        mSec = 0
                        UserIface.setTime(mYear, mMonth, mDay, mHour, mMinute, mSec)
                        setSelectPage(pageLoader, "qrc:/SettingsCommon.qml")
                        UserIface.stopAudioGuide() //SKW 2019-01-28 Stop Audio guide
                    }

                }
                onPressed: {
                    setImageSource(btnSaveImg, pngDir + "btn_s.png")
                }
                onReleased: {
                    setImageSource(btnSaveImg, pngDir + "btn_n.png")
                }
            }
        }


    Component.onCompleted: {
        print(" Time Settings Component.onCompleted")

        sendPageInfo()
    }


    // KEYPAD
    Keypad
    {
        id: i_keypad;
        onClose:
        {
            print("TimeSetting Keypad close : ",name, text)
            mYear = parseInt(inputYearTxt.text)
            mMonth = parseInt(inputMonthTxt.text)
            mDay = parseInt(inputDayTxt.text)
            mHour = parseInt(inputHourTxt.text)
            mMinute = parseInt(inputMinTxt.text)

            // SKW 2019-03-27 의미 없는 시간 방지
            print("mMonth % 2 ", mMonth % 2) // SKW 2019-03-27 mon check
            popupRectSignal = false
            print("popupRectSignal ", popupRectSignal)

//            //print("QDateTime ", QDateTime.currentDateTime())
//            dateString = currentDate.toLocaleDateString();
//            print("currDate :  ", Date.fromLocaleDateString(dateString))

            //기준시 미달 검사
            if(mYear < 2019 || mYear <= 2019 && mMonth < 3 || mYear <= 2019 && mMonth <= 3 && mDay < 27){
                print(" Day error : Past '2019.03.27' ")
                popupRect.visible = true
                updateText.text = "Day error : Past '2019.03.27' "
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
            } //기준시 미달

            /***************************************************/
            // 4로 나누었을 때 나머지가 0이고
            // 100으로 나누었을 때 나머지가 0이 아니면 윤년입니다.
            // 단, 예외적으로 100으로 나누었을때 나머지가 0이어도
            // 400으로 나누었을때 나머지가 0이면 무조건 윤년입니다.
            /***************************************************/

            //윤년 검사
            if (mYear % 4 == 0 && mYear % 100 != 0 || mYear % 400 == 0){
                if((mMonth == 2 && mDay > 29) ) {
                    print("Day error : February '29th' over") //윤년인데 29일 이상 입력
                    popupRect.visible = true
                    updateText.text = "Day error : February '29th' over"
                    popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
                }
            } else if (mMonth == 2 && mDay > 28){
                print("Day error : February '28th' over")  //평년인데 2월 28일 이상 입력
                popupRect.visible = true
                updateText.text = "Day error : February '28th' over"
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
            } else if(mMonth % 2 == 0 && mDay > 30) {
                //print("mMonth % 2 == 0 && mDay > 30 : ",mYear, " ^^ ", mMonth, " ^^ ", mDay)
                print("Day error : '30th' over") //평년인데 30일 이상 입력
                popupRect.visible = true
                updateText.text = "Day error : '30th' over"
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
            } else if(mMonth % 2 == 1 && mDay > 31) {
                print("Day error : '31th' over") //평년인데 31일 이상 입력
                popupRect.visible = true
                updateText.text = "Day error : '31th' over"
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
            }
             if (mMonth < 1 || mMonth > 12){
                print("Month error : Month over") //입력 월 초과
                popupRect.visible = true
                updateText.text = "Month error : Month over"
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
             }
            //입력 일 검사 //30, 31일은 월 검사에서
            if (mDay < 1 ){
                print("Day error : '1th' under") //입력 일이 1보다 작을 경우
                popupRect.visible = true
                updateText.text = "Day error : '1th' under"
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
            }

            // 시간 검사
            if (mHour > 24 || mHour < 1){
                popupRect.visible = true
                updateText.text = "Hour error"
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
            }else if (mMinute  > 59 || mMinute  < 0){ // SKW 2019-04-02 0분도 입력 가능하도록 수정
                popupRect.visible = true
                updateText.text = "Min error"
                popupRectSignal = true //SKW 2019-03-27 의미없는 날자 값으로 설정버튼 클릭 시 경고 팝업창 출력
            }

            print("TimeSetting Keypad close : ",mYear, " ^^ ", mMonth, " ^^ ", mDay, " ^^ ", mHour, " ^^ ", mMinute)
        }
    }

    ////////////// Pop up ///////////////////
        Rectangle {
            id: popupRect
            visible: false
            width: 722; height: 444
            x: ((800 - popupRect.width) / 2)
            y: ((1280 - popupRect.height) / 2)
            color: "transparent"

            Rectangle {
                id:popupOpacityRect
                x: - popupRect.x; y: - popupRect.y
                width: defaultWidth; height: defaultHeight
                color: "#000000"
                opacity: 0.5
                MouseArea {
                    anchors.fill: parent
                }
            }

            Image {
                id: popupImgbg
                width: 722; height: 444
                clip: true; cache: false
                source: pngDir + "bg_popup.png"
            }

            Text {
                id: updateTitleText
                text: qsTr("WARNING")
                x: 0
                y: 64
                width: 722; height: 46
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize:  34
                color: popupTitleColor
            }

            Text {
                id: updateText
                text: qsTr("Time Setting ...")
                x: 0
                y: 64 + 46 + 25
                width: 722; height: 138
                font.bold: false
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize:  34
                color: textColor
            }

            TextButton
            {
                id: btnDone
                width: 290; height: 80;
                x: ((popupRect.width - 290) / 2)
                y:  300
                text: "OK"
                textsize: 34
                img_p: pngDir + "btn_popup_s.png";
                img_n: pngDir + "btn_popup_n.png"
                text_p: textColor
                text_n: textColor
                onClicked: {
                    popupRect.visible = false
                }
            }
        }//popup

}
