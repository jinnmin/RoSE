import useriface 1.0
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

//Image
Item
{
    function setTopLogo(objId, _source) {
        // SKW 2019-03-27 비로그인 시 이미지 변경 (화면 전환 후 바로 교체하는 부분)
        if (UserIface.get_patient_id() === -1) {
            objId.x = 489
        }
        else {
            objId.x = 331
        }

        objId.source = _source
    }

    function setPinfoIcon(objId, pressed) {
        // SKW 2019-03-27 비로그인 시 이미지 변경 (화면 전환 후 바로 교체하는 부분)
print("setPinfoIcon UserIface.get_patient_id(): ", UserIface.get_patient_id() )

        if (UserIface.get_patient_id() === -1) {
            objId.x = defaultWidth - (76+12+76)
            objId.width = 76
            if(pressed)
                objId.source = pngDir + "btn_patient_s.png"
            else
                objId.source = pngDir + "btn_patient_n.png" //비로그인
        }
        else {
            objId.x = defaultWidth - (219+12+76)
            objId.width = 219
            if(pressed)
                objId.source = pngDir + "btn_patientsel_s.png"
            else
                objId.source = pngDir + "btn_patientsel_n.png"
        }

    }

    // SKW 2019-04-15 사용자 이름 표기
    function setPinfoName(objId, pressed) {
        if (UserIface.get_patient_id() === -1) {
            objId.text = " "//Unknown" //비로그인
        }
        else {
            print("UserIface.get_first_name().length() : ", UserIface.get_first_name().length)
            if (UserIface.get_first_name().length < 7)
                objId.text = UserIface.get_first_name()
            else
                objId.text = UserIface.get_first_name().substring(0, 6) + "..."
        }
    }

}
