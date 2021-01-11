/**
  *********************************************************************************************************************
  * @file       useriface.cpp
  * @date       2020/09/28
  * @author
  * @brief
  *********************************************************************************************************************
  * @copy
  *
  *  This software embodies materials and concepts which are confidential to HIRONIC Inc. and is made available
  *  solely pursuant to the terms of a written license agreement with HIRONIC Inc.
  *
  *  <h2><center>&copy; COPYRIGHT 2020 HIRONIC Inc. </center><h2>
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "useriface.h"

/* Mecro Function  ---------------------------------------------------------------------------------------------------*/
#define Q_HI_LOGO(...)     qDebug() << '[' << __FILE__ << ':' << __FUNCTION__ << ':' << __LINE__ << ']' ##__VA_ARGS__

UserIface* UserIface::mInstance = NULL;

UserIface::UserIface(QObject *parent) :
    QObject(parent),
    mSwUpdaterThread(NULL),
    mFwUpdaterThread(NULL),
    mGettingLogThread(NULL),
    mSender(NULL),
    mReceiver(NULL),
    mSwUpdater(NULL),
    mSerial(NULL),
    mYModem(NULL),
    mLogger(NULL)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    init();
    initPort();

#if defined(KZ_DB)
    init_db();
#endif


    mReceiver = new Receiver(mSerial);
    mSender = new Sender(mSerial);
    connect(mReceiver, SIGNAL(receivedData(QByteArray)), this, SLOT(onParseData(QByteArray)));

    connectDev();

    initCommonSettings();


#if !defined(KZ_DB)
    QObject *gui_obj = m_gui->get_root();
    Q_ASSERT(gui_obj != NULL);
//    connect(gui_obj, SIGNAL(qmls_search_patient(QString)), this, SLOT(db_search_patient(QString)));
    connect(gui_obj, SIGNAL(qmls_selected_patient(int)), this, SLOT(db_selected_patient(int)));
#endif

#if !defined(KZ_DB)
    m_gui = GUI::get_instance();

    m_gui->setModel("patient_model", &m_patient_model);

#endif

#if defined(KZ_UI)

    // MAIN PROCEDURE START : INTERVAL 0.5 SEC
    connect(&m_proc, SIGNAL(timeout()), this, SLOT(proc()));

    m_proc.start(500);
#endif
}

UserIface::~UserIface()
{
    qDebug() << __func__;
    quitThread();

    if (NULL != mSerial) {
        if (mSerial->isOpen()) {
            qDebug() << __func__ << " port closed!";
            mSerial->close();
        }

        delete mSerial;
        mSerial = NULL;
        qDebug() << "mSerial("<< mSerial << ") removed!";
    }
}

UserIface *UserIface::getInstance()
{
   return mInstance;
}


void UserIface::init()
{

    mInstance = this;
    mEmptyPayloadMsg = NULL;

    mIsconnectedSerial = false;
    mLastIsconnectedSerial = false;
    mIsUpdateStatus = false;
    mSystemTimeSettingStatus = false;
    mFWUpdateStatus = false;
    mSWUpdateStatus = false;
    mRESTipSettingStatus = false;
    mCurPageNo = 0;  // Initial 99
    mCurHpInfo = 0;
    mHpConnect = 0;
    mSpotSize = 0;
    mTipSerialNo = 0;
    mTreatmentTimeMin = 0;
    mTreatmentTimeSec = 0;
    mTipRemainHour = 0;
    mTipRemainMin = 0;
    mTipRemainSec = 0;
    mOperationInfo = -1;
    mOperationMode = 0;
    mOperationLevel = 0;
    mStandbyReady = 0;
    mBuzzerSound = 0;
    mDateYear = mSettingDateYear = 0;
    mDateMon = mSettingDateMon = 0;
    mDateDay = mSettingDateDay = 0;
    mDateHour = mSettingDateHour = 0;
    mDateMin = mSettingDateMin = 0;
    mDateSec = mSettingDateSec = 0;
    mTipReleaseYear = 0;
    mTipReleaseMon = 0;
    mTipReleaseDay = 0;
    malarmCode = 0;
    mUpdateKinda = 0;
    mFwMainVersion = 0;
    mFwSonoVersion = 0;
    mNeedleStatus = 0;

    mLoggingHpInfo = 0;
    mLoggingUsedTipCnt = 0;
    mLoggingTotalTMTime = 0;
    mLoggingTipRMTimeHour = 0;
    mLoggingTipRMTimeMin = 0;
    mLoggingTipRMTimeSec = 0;

    mResetTotalTimeHpInfo = 0;  //default Plasonic

#if defined(KZ_DB)
    m_patient_gender =0;
#endif

    memset(&mStartTreatmentTime, 0, sizeof(struct tm));
    memset(&mEndTreatmentTime, 0, sizeof(struct tm));
    memset(&mTipSettingTime, 0, sizeof(struct tm));


}

void UserIface::initCommonSettings()
{
    mConfSettings.set_filepath("/home/app/data/settings.conf");
    mConfSettings.append_list("local", "001");  // 2018-03-12 JJHWANG for Foreign Product
    mConfSettings.append_list("lang", "EN");
    mConfSettings.append_list("guide_lang", "EN");
    mConfSettings.append_list("guide_vol", "50");
    mConfSettings.append_list("lcd_vol", "50");
    mConfSettings.append_list("sound_mute", "0");
    mConfSettings.append_list("passwd", "4848");  // 20180227
    mConfSettings.append_list("factory_init", "0");       // 2018-05-10 JJHWANG
    mConfSettings.append_list("aging_mode", "0");       // 2018-09-18 JJHWANG
    mConfSettings.append_list("aging_count", "0");       // 2018-09-18 JJHWANG
//    mConfSettings.append_list("serial_number", "GENTLO");       // 2018-07-23 JJHWANG

    mConfSettings.update();
    loadCommonSettings();
}


#if defined(KZ_UI)
void UserIface::proc(){
    static unsigned int tick = 0;

    tick++;

    if((tick>5) && ((mCurPageNo == PAGE_HOME) && (mIsconnectedSerial == true )))
    {

    }

}
#endif

void UserIface::setConnectedSerial(bool state)
{

    mIsconnectedSerial =  state;
    qDebug() << __func__ << mIsconnectedSerial;
}

bool UserIface::getConnectedSerial() const
{
    qDebug() << __func__ << "mIsconnectedSerial: " << mIsconnectedSerial;

//    return mIsconnectedSerial;
    return true; // JOON_TEST LZ
}

void UserIface::quitThread()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    emit fwUpdatefinished();
    emit swUpdatefinished();
    emit gettingLogfinished();
}

void UserIface::setCurrentPage(int _pageNo)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ <<" 1 pageNo : " << _pageNo;
#endif
    if (getConnectedSerial())
    {
        mSender->cmdUiPageInfo(_pageNo);
    }
}

#if defined(KZ_UI)
void UserIface::setGetCartInfo(int hp_info, int setGet, int countryCode, int serialNo, int currCnt, int totalCnt, int tipType)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ <<" 1 hp_info : " << hp_info;
#endif
    if (getConnectedSerial())
    {
        if(setGet == 1) {
            mSender->cmdUiCurrDate(mDateYear, mDateMon, mDateDay);
            qDebug() << __func__ <<" 1 mDateYear : " << mDateYear;
            qDebug() << __func__ <<" 1 mDateMon : " << mDateMon;
        }

        mSender->cmdUiCartInfo(hp_info, setGet, countryCode, serialNo, currCnt, totalCnt, tipType);

    }
}
#endif

void UserIface::setPageNo(int _pageNo)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " ** 2 pageNo" <<_pageNo;
#endif
    mCurPageNo = _pageNo;
    if((mCurPageNo > PAGESETTINGS) && (mCurPageNo < PAGERMAUTO_RETURN)) {
        return; //  Gentlo only
    }

    else if (mCurPageNo == PAGESETTINGS ) {
        loadCommonSettings();
        return;
    }
    else if (mCurPageNo == PAGERMAUTO_RETURN) {
        mCurPageNo = PAGERMAUTO;
        return; // It should be change
    }

    setCurrentPage(_pageNo);
}

#if defined(KZ_UI)

void UserIface::getCartInfo(int hp_info)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " ** 2 hp_info" <<hp_info;
#endif
    setGetCartInfo(hp_info, 0, 0, 0, 0, 0, 0); // 0: get , 1: set

}

void UserIface::setCartInfo(int hp_info, int countryCode, int serialNo, int currCnt, int totalCnt, int tipType )
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " ** 2 hp_info" <<hp_info;
    qDebug() << __func__ << " ** 2 serialNo" <<serialNo;
#endif
//    mCurHpInfo = hp_info;
    setGetCartInfo(hp_info, 1, countryCode, serialNo, currCnt, totalCnt, tipType); // 0: get , 1: set
}
#endif


/*FUNCTION*-------------------------------------------------------------------------------------------------------*//**
 *
 * @fn          void UserIface::sendToPower(int cmd, int getSet, int data1, int data2, int data3, int data4, int data5, int data6)
 * @return      none.
 * @brief
 *
 *//*----------------------------------------------------------------------------------------------------------Doxygen*/
void UserIface::sendToPower(int cmd, int getSet, int data1, int data2, int data3, int data4, int data5, int data6)
{
    Q_HI_LOGO() << "RoSE cmd: "<< hex  << cmd << data1 << hex << data2 << hex  << data3 << hex << data4 << hex  << data5 << hex << data6 ;

//    if(cmd == 0xfb && mSettingsSoundMute == "0") //soundOnCmd 0xfb(251)
//    {
//        Q_HI_LOGO() << "Pico HI mSettingsSoundMute : " << mSettingsSoundMute << "cmd :" << cmd;
//        return;
//    }

//    if (getConnectedSerial())
    {
        if(cmd == ANGLE_DATA_CMD)
            mSender->cmdSendToPower(cmd, getSet, data1, data2, data3, data4, data5, data6);  // DEVICE_ID_DFIT
//        else
//            emit resCmdFromPower(cmd, getSet, data1, data2, data3, data4, data5, data6); // JOON_TEST

#if !defined(JOON_TEST)
        switch (cmd) {
            case KEEP_ALIVE_CMD:
    //            break;
            case MAIN_FW_CMD:
                data1 = 0;
                data2 = 0;
                data3 = 1;
                break;
            case SUB_FW_CMD:
                data1 = 0;
                data2 = 0;
                data3 = 1;
                break;
            case DATE_CMD:
                data1 = 20;
                data2 = 11;
                data3 = 11;
                data4 = 20;
                data5 = 11;
                data6 = 11;
                break;
            case CART_STATUS_CMD:
                data1 = 1;
                break;
            case INSPECTION_STATUS_CMD:
    //            break;
            case SYSTEM_STATE_CMD:
    //            break;
            case SYS_SET_STATE_CMD:
    //            break;
            case ANGLE_DATA_CMD:
    //            break;
            case ERROR_CMD:
    //            break;
            default:
                 break;
        }

        emit resCmdFromPower(cmd, getSet, data1, data2, data3, data4, data5, data6); // JOON_TEST
#endif

    }
}

/*FUNCTION*-------------------------------------------------------------------------------------------------------*//**
 *
 * @fn          void UserIface::startRMS(QString date)
 * @return      none.
 * @brief       2018-05-28 JJHWANG RMS Start
 *
 *//*----------------------------------------------------------------------------------------------------------Doxygen*/

void UserIface::startRMS(QString date)
{
    mTotalLogFile.append(mSerialNumber);
    mTotalLogFile.append("_");
    mTotalLogFile.append(date);
    mTotalLogFile.append(".csv");

    qDebug() << "mTotalLogFile : " << mTotalLogFile;

#if defined(KZ_UI_DEBUG)
    qDebug() << __func__<< mSerialNumber;
    qDebug() << __func__ << " ^&&^& webSocketTest:  " << m_webSocket.webSocketTest();
#endif
    while (!m_webSocket.webSocketTest()) {
        m_webSocket.rerun();
    }

    m_webSocket.setSerialNo(mSerialNumber);
    m_webSocket.setTotalLogFile(mTotalLogFile);
    m_webSocket.run();

}

void UserIface::stopRMS()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__<< " &&& webSocketTest:  " << m_webSocket.webSocketTest();
#endif
    m_webSocket.close();

}

void UserIface::changeLogoHironic()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    QProcess::execute("/home/pi/hironic/logo_change_hironic.sh");
}

void UserIface::changeBiPlasonic_org()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    QProcess::execute("/home/pi/hironic/change_plasonic_org.sh");
}

void UserIface::changeBiPlasonic_s()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    QProcess::execute("/home/pi/hironic/change_plasonic_s.sh");
}



// ////////// SKW 2019-01-17 Play audio guide ///////////////
void UserIface::playAudioGuide(QString audioFileName)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << audioFileName;
#endif

#if defined(JOON_TEST)

    int vol = mSettingsGuideVol.toInt();
    if(vol == 0) { return; }
    if(vol <= 50){
        vol = (50 - vol) * -1;
        vol *= 20;
//        vol *= 5;
    }else{
        vol *= 1.0; // SKW 2019-04-05 볼륨 깨지는 문제 수정. 50%이상에서는 증폭 없이 원음 출력
    }

    QStringList param; param.clear();
    QString cmd = "/home/pi/hironic/playaudio.sh";
    param.append(QString("%1").arg(vol));
    param.append("/home/app/res/audio/GT_audio/"+ mSettingsGuideLang +"/" + audioFileName);
    QProcess::execute(cmd, param);
#endif
}

// ////////// SKW 2019-01-22 stop audio guide ///////////////
void UserIface::stopAudioGuide()
{
    QString cmd = "/home/pi/hironic/stopaudio.sh";
    QProcess::execute(cmd);
}
// ////////// SKW 2019-01-28 Play audio guide ///////////////

void UserIface::saveLogData()
{
/*
Mode   | Device Turn On   | R/S   | Frequency  | Level | Start Treatment Time | End Treatment Time | Tip Info        | Tip Remaining Time | Used Tip Count | Used Tip Time | Error Code
Plasma | 2018-02-23 10:18 | READY | Continuous | 5     | 2018-02-23 10:18     | 2018-02-23 10:18   | PST201802231112 | 0:29:42            | 2              | 0:58:05       | It works normal operation.
*/
    struct tm turn_on;
    struct tm tipReleaseInfo;
    struct tm tipTotaltimeInfo;
    mLogger = new Logger;

    turn_on.tm_year = (mDateYear + 2000) - 1900;
    turn_on.tm_mon = (mDateMon - 1);
    turn_on.tm_mday = mDateDay;
    turn_on.tm_hour = mDateHour;
    turn_on.tm_min = mDateMin;
    turn_on.tm_sec = mDateSec;

    tipReleaseInfo.tm_year = (mTipReleaseYear + 2000) - 1900;
    tipReleaseInfo.tm_mon = (mTipReleaseMon - 1);
    tipReleaseInfo.tm_mday = mTipReleaseDay;
    tipReleaseInfo.tm_hour = (mLoggingTipRMTimeHour & 0xF);
    tipReleaseInfo.tm_min = (((mLoggingTipRMTimeMin >> 4) & 0xFF) * 10) + (mLoggingTipRMTimeMin & 0x0F);
    tipReleaseInfo.tm_sec = (((mLoggingTipRMTimeSec >> 4) & 0xFF) * 10) + (mLoggingTipRMTimeSec & 0x0F);

    tipTotaltimeInfo.tm_year = TM_YEAR(mLoggingTotalTMTime);
    tipTotaltimeInfo.tm_mon = TM_MON(mLoggingTotalTMTime);
    tipTotaltimeInfo.tm_mday = TM_DAY(mLoggingTotalTMTime);
    tipTotaltimeInfo.tm_hour = TM_HOUR(mLoggingTotalTMTime);
    tipTotaltimeInfo.tm_min = TM_MIN(mLoggingTotalTMTime);
    tipTotaltimeInfo.tm_sec = TM_SEC(mLoggingTotalTMTime);

    mLogger->log_print(mCurHpInfo, turn_on, getLoggingStdbyReady(mStandbyReady), getLoggingOperMode(mOperationMode),
                       getLoggingOperLevel(mOperationLevel), mStartTreatmentTime, mEndTreatmentTime, tipReleaseInfo, mTipSerialNo,
                       mLoggingUsedTipCnt, tipTotaltimeInfo, getLoggingErrCode(malarmCode));

    if (NULL != mLogger) {
        delete mLogger;
        mLogger = NULL;
    }
}

void UserIface::saveLogDataOnAlram()
{
/*
Mode   | Device Turn On   | R/S   | Frequency  | Level | Start Treatment Time | Alram Occurrence Time | Error Code
Plasma | 2018-02-23 10:18 | READY | Continuous | 5     | 2018-02-23 10:18     | 2018-02-23 10:18      | The Plasma Tip has no inserted.
*/
    struct tm turn_on;
    mLogger = new Logger;

    turn_on.tm_year = (mDateYear + 2000) - 1900;
    turn_on.tm_mon = (mDateMon - 1);
    turn_on.tm_mday = mDateDay;
    turn_on.tm_hour = mDateHour;
    turn_on.tm_min = mDateMin;
    turn_on.tm_sec = mDateSec;

    mLogger->log_printOnAlram(getLoggingHPInfo(mCurHpInfo), turn_on, mStandbyReady, getLoggingStdbyReady(mStandbyReady),
                              getLoggingOperMode(mOperationMode), getLoggingOperLevel(mOperationLevel), mStartTreatmentTime,
                              mEndTreatmentTime, getLoggingErrCode(malarmCode));

    if (NULL != mLogger) {
        delete mLogger;
        mLogger = NULL;
    }
}


void UserIface::saveLogDataOnTipSettingComplete(bool status)
{
/*
Device Turn On   | Tip Setting Complete Time | Tip Info        | Tip Remaining Time | Error Code
2018-02-23 10:18 | 2018-02-23 10:18          | PST201802231112 | 0:30:00            | Tip Setting has succeeded.
*/
    struct tm turn_on;
    struct tm tipReleaseInfo;
    mLogger = new Logger;

    turn_on.tm_year = (mDateYear + 2000) - 1900;
    turn_on.tm_mon = (mDateMon - 1);
    turn_on.tm_mday = mDateDay;
    turn_on.tm_hour = mDateHour;
    turn_on.tm_min = mDateMin;
    turn_on.tm_sec = mDateSec;

    tipReleaseInfo.tm_year = (mTipReleaseYear + 2000) - 1900;
    tipReleaseInfo.tm_mon = (mTipReleaseMon - 1);
    tipReleaseInfo.tm_mday = mTipReleaseDay;
    tipReleaseInfo.tm_hour = (mTipRemainHour & 0xF);
    tipReleaseInfo.tm_min = (((mTipRemainMin >> 4) & 0xFF) * 10) + (mTipRemainMin & 0x0F);
    tipReleaseInfo.tm_sec = (((mTipRemainSec >> 4) & 0xFF) * 10) + (mTipRemainSec & 0x0F);

    mLogger->log_printOnSettingComplete(turn_on, mTipSettingTime, tipReleaseInfo, mTipSerialNo, getLoggingTipSettingInfo(status));

    if (NULL != mLogger) {
        delete mLogger;
        mLogger = NULL;
    }
}

#if defined(KZ_UI_DEBUG)
void UserIface::saveLogDataUartConnect()
{
/*
Mode   | Device Turn On   | R/S   | Frequency  | Level | Start Treatment Time | Alram Occurrence Time | Error Code
Plasma | 2018-02-23 10:18 | READY | Continuous | 5     | 2018-02-23 10:18     | 2018-02-23 10:18      | The Plasma Tip has no inserted.
*/
    struct tm turn_on;
    mLogger = new Logger;
    static unsigned count=0;

    turn_on.tm_year = (mDateYear + 2000) - 1900;
    turn_on.tm_mon = (mDateMon - 1);
    turn_on.tm_mday = mDateDay;
    turn_on.tm_hour = mDateHour;
    turn_on.tm_min = mDateMin;
    turn_on.tm_sec = mDateSec;

    count++;
    if (count > 20000)
        count = 0;

    mLogger->log_printTestLog(getLoggingHPInfo(mCurHpInfo), turn_on, mStandbyReady, getLoggingStdbyReady(mStandbyReady),
                              getLoggingOperMode(mOperationMode), getLoggingOperLevel(mOperationLevel), mStartTreatmentTime,
                              mEndTreatmentTime, count);

    if (NULL != mLogger) {
        delete mLogger;
        mLogger = NULL;
    }
}
#endif

QString UserIface::getLoggingHPInfo(int hp)
{
    QString hpInfo;
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    switch (hp) {
    case HP_ALEX:
        hpInfo = "Alexandrite";
        break;
    case HP_NDYAG:
        hpInfo = "Ndyag";
        break;

    }

    return hpInfo;
}

QString UserIface::getLoggingStdbyReady(int stdby_ready)
{
    QString stdbyReadyMode;
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    switch (stdby_ready) {
    case OPERATION_STANDBY:
        stdbyReadyMode = "STANDBY";
        break;
    case OPERATION_READY:
        stdbyReadyMode = "READY";
        break;
    }
    return stdbyReadyMode;
}

QString UserIface::getLoggingOperMode(int mode)
{
    QString operMode;
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    switch (mode) {
    case OPERATION_CONTINUE:
        operMode = "Continuous";
        break;
    case OPERATION_D1:
        operMode = "D1";
        break;
    case OPERATION_D2:
        operMode = "D2";
        break;
    case OPERATION_D3:
        operMode = "D3";
        break;
    case OPERATION_MIX:
        operMode = "Mix";
        break;
    }
    return operMode;
}

int UserIface::getLoggingOperLevel(int level)
{
    int operLevel;
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    switch (level) {
    case OPERATION_LV1:
        operLevel = 1;
        break;
    case OPERATION_LV2:
        operLevel = 2;
        break;
    case OPERATION_LV3:
        operLevel = 3;
        break;
    case OPERATION_LV4:
        operLevel = 4;
        break;
    case OPERATION_LV5:
        operLevel = 5;
        break;
    default:
        operLevel = 0;
        break;
    }
    return operLevel;
}

QString UserIface::getLoggingErrCode(int err)
{
    QString errCode;
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    // SKW 2019-05-03 에러 코드 추가 HSPS 통신 프로토콜(최종변경일: 2018-08-09 버전)참조
    switch (err) {
    case NORMAL_OPERATION:
        errCode = "It works normal operation.";
        break;
    case CHECK_THE_CONNECTOR:
        errCode = "The Plasma connector has no inserted.";
        break;
    case INSERT_THE_TIP:
        errCode = "The Ultrasonic connector has no inserted.";
        break;
    case THE_TIME_IS_EXCEEDED_REPLACE_THE_TIP:
        errCode = "The Plasma Tip has no inserted.";
        break;
    case UNREGISTERED_TIP_HAS_USED:
        errCode = "The Plasma Tip usage time has expired.";
        break;
    case REUSE_TIP_HAS_USED:
        errCode = "Use Reuse Tip";
        break;
    case COUNTRY_CODE_ERROR:
        errCode = "Country Code Error ";
        break;
    case COMMUNICATION_ERROR:
        errCode = "Communication Error, re-insertion of connector";
        break;
    case FAILED_TO_READ_CARTRIDGE_INFORMATION:
        errCode = "Failed to read cartridge information, re-insert cartridge";
        break;
    case MOTOR_ERROR_DURING_SHOT:
        errCode = "Motor Error During Shot";
        break;
    case FW_COMMUNICATION_ERROR:
        errCode = "F/W Communication Error";  // SW 에러 신호 (임의로 20번 사용)
        break;
    default:
        errCode = "Unknown Error Code(" + QString::number(err) + ")";
        break;
    }
    return errCode;
}

QString UserIface::getLoggingTipSettingInfo(bool status)
{
    QString tipSettingMsg;
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    if (status) {
        tipSettingMsg = "Tip Setting has succeeded.";
    }
    else {
        tipSettingMsg = "Tip Setting has failed.";
    }
    return tipSettingMsg;
}

#if defined(KZ_UI)

void UserIface::saveCommonSet(QString guideVol, QString lcd, QString soundMute)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " guideVol" << guideVol;
    qDebug() << __func__ << " lcd" << lcd;
#endif
    mSettingsGuideVol = guideVol;
    mSettingsLcdVol = lcd;
    mSettingsSoundMute = soundMute;

}

void UserIface::getCurrPage()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " mCurPageNo" << mCurPageNo;
#endif
    emit resSelectPageInfo(mCurPageNo);
}

void UserIface::getCurrCartInfo()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " mCartridgeType" << mCartridgeType;
#endif
    emit resSelectCartInfo(mHpConnect, mCartridgeInfo, mCartNationCode, mCartSerial, mCartFreq, mCartridgeType,
                           mCartPowerCal, mCartCurrentCount, mCartMaxCount);
}

void UserIface::getCurrTipInfo()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " mCartridgeType" << mCartridgeType;
#endif
    emit resSelectTipInfo(mCartridgeInfo, mCartNationCode, mCartSerial);

    emit reqPsTipInfoRMTime(mTipRemainHour, mTipRemainMin, mTipRemainSec, mCartSerial, mTipReleaseYear, mTipReleaseMon, mTipReleaseDay);
}

void UserIface::saveSettings(bool save_flag) {

    qDebug() << __func__ << " ^^^^^^^^^^^^^^^  save_flag: " <<  save_flag;

    if(save_flag == true)
        saveCommonSettings();

    loadCommonSettings();

}

#endif

void UserIface::setTipSettings(int sn, int hh, int mm)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    if (getConnectedSerial())
    {
        mSender->cmdSetTipSettings(sn, mDateYear, mDateMon, mDateDay, hh, mm);
    }
}

void UserIface::setTip(int sn, int hh, int mm)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif

    mTipSerialNo = sn;
    mTipRemainHour = hh;
    mTipRemainMin = mm;

    setTipSettings(sn, hh, mm);
}

bool UserIface::getUpdateStatus() const
{
    return mIsUpdateStatus;
}

void UserIface::setSystemReboot(bool flag)
{
    qDebug() << __func__ << "flag : " << flag;

    if (NULL != mSerial) {
        if (mSerial->isOpen()) {
            qDebug() << __func__ << " port closed!";
            mSerial->close();
        }
    }

    if (flag) mSwUpdater->reboot();
}

void UserIface::connectDev()
{
    initPort();

    mSender->cmdConnect();
}


#if defined(KZ_UI_GET_HP)
void UserIface::getConnectedHP()
{
    mSender->cmdGetConnectedHP();
}
#endif

int UserIface::getPageNo()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "mCurPageNo" << mCurPageNo;
#endif
    return mCurPageNo;
}

void UserIface::setFwDate(int yy, int mm, int dd, int h, int m, int s)
{
    if (getConnectedSerial())
    {
        mSender->cmdSetFwDate(yy, mm, dd, h, m, s);
    }
}

void UserIface::setTime(int yy, int mm, int dd, int h, int m, int s)
{
    // qDebug() << yy << ":" << mm << ":" << dd << ":" << h << ":" << m << ":" << s;
    // qDebug() << (DECTOBCD(yy) & 0xFF) << ":" << DECTOBCD(mm) << ":" << DECTOBCD(dd);
    // qDebug() << DECTOBCD(h) << ":" << DECTOBCD(m) << ":" << DECTOBCD(s);
    // qDebug() << hex << (DECTOBCD(yy) & 0xFF) << ":" << hex << DECTOBCD(mm) << ":" << hex << DECTOBCD(dd);
    // qDebug() << hex << DECTOBCD(h) << ":" << hex << DECTOBCD(m) << ":" << hex << DECTOBCD(s);
    mSettingDateYear = yy;
    mSettingDateMon = mm;
    mSettingDateDay = dd;
    mSettingDateHour = h;
    mSettingDateMin = m;
    mSettingDateSec = s;
    setFwDate((DECTOBCD(yy) & 0xFF), DECTOBCD(mm), DECTOBCD(dd), DECTOBCD(h), DECTOBCD(m), DECTOBCD(s));
}

void UserIface::getDateTime()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    if (getConnectedSerial())
    {
        mSender->cmdGetFwDate();
    }
}

void UserIface::setStandbyReady(int hp_info, int status)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "* hp_info :" << hp_info << "mCurHpInfo :" << hex << mCurHpInfo << "status :" << status;
    qDebug() << __func__ << "* mPulseDuration :" << hex << mPulseDuration << "mPsuVol :" << hex << mPsuVol;
    qDebug() << __func__ << "* mPulseRate :" << hex << mPulseRate << "mCcdPreSpray :" << hex << mCcdPreSpray;
    qDebug() << __func__ << "* mCcdDelay :" << hex << mCcdDelay << "mCcdPostSpray :" << hex << mCcdPostSpray;
    qDebug() << __func__ << "* mAimingBeam :" << hex << mAimingBeam << "mCcdPostSpray :" << hex << mCcdPostSpray;
    qDebug() << __func__ << "* mSporSize :" << hex << mSporSize << "mCcdOnOff :" << hex << mCcdOnOff;
#endif
    mCurHpInfo = hp_info;

    if (getConnectedSerial())
    {
        mSender->cmdSetStandbyReady(hp_info, mPulseDuration, mPsuVol, mPulseRate, mCcdPreSpray, mCcdDelay, mCcdPostSpray, mAimingBeam,  status, mSporSize, mCcdOnOff);
    }
}

void UserIface::setStatus(int standby_ready)
{

    mStandbyReady = standby_ready;

    struct tm * curTime = getCurrentTime();
    mStartTreatmentTime.tm_year = curTime->tm_year;
    mStartTreatmentTime.tm_mon = curTime->tm_mon;
    mStartTreatmentTime.tm_mday = curTime->tm_mday;
    mStartTreatmentTime.tm_hour = curTime->tm_hour;
    mStartTreatmentTime.tm_min = curTime->tm_min;
    mStartTreatmentTime.tm_sec = curTime->tm_sec;
    // qDebug() << "startTreatmentTime : " << mStartTreatmentTime.tm_year << mStartTreatmentTime.tm_mon << mStartTreatmentTime.tm_mday;
    // qDebug() << "startTreatmentTime : " << mStartTreatmentTime.tm_hour <<  mStartTreatmentTime.tm_min << mStartTreatmentTime.tm_sec;
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "standby_ready :" << standby_ready;
#endif

    switch (getPageNo()) {
        case PAGE_ALEX:
            setStandbyReady(HP_ALEX, standby_ready);
            break;
        case PAGE_NDYAG:
            setStandbyReady(HP_NDYAG, standby_ready);
            break;
        default:
            setStandbyReady(HP_ALEX, standby_ready);
            break;

    }
}

void UserIface::setAckToMCU(quint16 resMsg)
{
    if (getConnectedSerial())
    {
        mSender->cmdSetAckToMCU(resMsg);
    }
}

void UserIface::onFwUpdateDeleteLater()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif

    if (NULL != mFwUpdaterThread) {
        // thread stop
        mFwUpdaterThread->quit();
        mFwUpdaterThread->wait();

        delete mFwUpdaterThread;
        mFwUpdaterThread = NULL;
        qDebug() << __func__ << " fw updater thread quit!";

        if (NULL != mYModem) {
            delete mYModem;
            mYModem = NULL;
            qDebug() << __func__ << "mYmodem object has deleted.";
        }
    }
}

void UserIface::onSwUpdateDeleteLater()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif

    if (NULL != mSwUpdater) {
        delete mSwUpdater;
        mSwUpdater = NULL;
    }
}

void UserIface::onGettingLogDeleteLater()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif

    if (NULL != mLogger) {
        delete mLogger;
        mLogger = NULL;
    }
}

void UserIface::onParseData(QByteArray _recvData)
{

    QByteArray recvData = _recvData;

    parsePayloadData(recvData);

//    QByteArray recvData = _recvData;
//    unsigned long recvDataSize = 0;

//    recvDataSize = recvData.size();

//    if (MSG_PROTOCOL_SIZE == recvDataSize) {
//        parseEmptyPayloadData(recvData);
//    }
//    else if (MSG_PROTOCOL_SIZE < recvDataSize){
//        parsePayloadData(recvData);
//    }
//    else {
//        qDebug() << "wronged packet :" << recvDataSize;
//        return;
//    }
}

void UserIface::closing()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "5 In()";
#endif
    quitThread();
}

void UserIface::onFwUpdateWorkFinished(bool status, int result)
{
    qDebug() << __func__ << status;

    mFWUpdateStatus = false;

    if (status)
        emit fwUpdateStatus(true, result);
    else
        emit fwUpdateStatus(false, result);

    emit fwUpdatefinished();
}

void UserIface::onFwUpdateProgress(int progress)
{
    emit fwProgress(progress);
}

void UserIface::onCheckedFile(QString fileName)
{
    qDebug() << __func__ << fileName;

    emit fwCheckedFile(fileName);
}

void UserIface::onSwUpdateWorkFinished(bool status, int result)
{
    qDebug() << __func__ << "status" << status;
    qDebug() << __func__ << "result" << result;

    mIsUpdateStatus = status;
    mSWUpdateStatus = false;

    if (status)
        emit swUpdateStatus(true, result);
    else
        emit swUpdateStatus(false, result);


    emit updateStatus(mIsUpdateStatus, result);

    emit swUpdatefinished();
}

void UserIface::onGettingLoggingWorkFinished(bool status, int result)
{
    qDebug() << __func__ << status;

    emit gettingLogStatus(status, result);
    emit gettingLogfinished();
}

void UserIface::parseEmptyPayloadData(QByteArray _recvData)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "6 In()";
    qDebug() << __func__ << "2. Recv Data : " << _recvData;
#endif

    mEmptyPayloadMsg = new emptyPayloadMsg;
    mEmptyPayloadMsg = reinterpret_cast<struct emptyPayloadMsg*>(_recvData.data());
    qDebug() << __func__ << "mEmptyPayloadMsg : " << hex << mEmptyPayloadMsg->mStx;

    mEmptyPayloadMsg->mStx      = qToBigEndian(mEmptyPayloadMsg->mStx);

//    mEmptyPayloadMsg->msgCode   = qToBigEndian(mEmptyPayloadMsg->msgCode);
    mEmptyPayloadMsg->mEtx      = qToBigEndian(mEmptyPayloadMsg->mEtx);

#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "41 In()";
    qDebug() << "mStx" << hex << mEmptyPayloadMsg->mStx;
    qDebug() << "msgCode" << hex << mEmptyPayloadMsg->msgCode;
    qDebug() << "mEtx" << hex << mEmptyPayloadMsg->mEtx;

    qDebug() << __func__ << "42 In()";
#endif

    if (START_BIT == mEmptyPayloadMsg->mStx && STOP_BIT == mEmptyPayloadMsg->mEtx) {
//        if (MSG_DEFAULT_SIZE == mEmptyPayloadMsg->msgLen) {
//            responseEmptyPayloadCmd(mEmptyPayloadMsg->msgCode);
//        }
    }
}

void UserIface::parsePayloadData(QByteArray _recvData)
{
#if defined(KZ_UI_DEBUG)
    Q_HI_LOGO() << "7 In()";
    Q_HI_LOGO() << "3. Recv Data : " << _recvData;
#endif
    int payloadSize = _recvData.size();
    mMsg = new message;
    mMsg->msgPayload = new quint8[payloadSize];

    QDataStream stream(&_recvData, QIODevice::ReadOnly);

    stream >> mMsg->mStx >>  mMsg-> deviceId >> mMsg->msgCode >>  mMsg-> msgLen >> mMsg->msgInstruction ;

    for(int loop = 0 ; loop < 6; loop++) {
        stream >> mMsg->msgPayload[loop];
    }

    stream >> mMsg->mCrc >> mMsg->mEtx;

#if defined(KZ_UI_DEBUG)
    QString dbgMsg = "";

    dbgMsg = QString("START BIT : 0x%1").arg(mMsg->mStx, 4, 16, QChar('0') );
    dbgMsg += ", ";
    dbgMsg += QString("MSG : 0x%1").arg( mMsg->deviceId, 4, 16, QChar('0') );
    dbgMsg += ", ";
    dbgMsg += QString("MSG : 0x%1").arg( mMsg->msgCode, 4, 16, QChar('0') );
    dbgMsg += ", ";
    dbgMsg = QString("START BIT : 0x%1").arg(mMsg->msgLen, 4, 16, QChar('0') );
    dbgMsg += ", ";
    dbgMsg += QString("MSG : 0x%1").arg( mMsg->msgInstruction, 4, 16, QChar('0') );
    dbgMsg += ", ";
    dbgMsg += QString("CRC : 0x%1").arg( mMsg->mCrc, 4, 16, QChar('0') );
    dbgMsg += ", ";
    dbgMsg += QString("STOP BIT : 0x%1").arg( mMsg->mEtx, 4, 16, QChar('0') );

    Q_HI_LOGO() << dbgMsg;
#endif

    if (START_BIT == mMsg->mEtx)
        mMsg->mEtx = STOP_BIT;

    if (START_BIT == mMsg->mStx && ((STOP_BIT == mMsg->mEtx)||(USER_STOP_BIT == mMsg->mEtx))) {
        {
            responseCmd(mMsg->msgCode, mMsg->msgInstruction, mMsg->msgPayload);
        }
    }

    if (NULL != mMsg) {
        if (NULL != mMsg->msgPayload)
            delete[] mMsg->msgPayload;
        delete mMsg;
    }
}

void UserIface::getLogger()
{
    saveLogDataToUSB();
}

void UserIface::saveLogDataToUSB()
{
//    int ret;
    mGettingLogThread = new QThread(this);
    mLogger = new Logger;

    // connect UserIface <-> SwUpdater
    connect(mGettingLogThread, SIGNAL(started()), mLogger, SLOT(do_work()));
    connect(mLogger, SIGNAL(workFinished(bool, int)), this, SLOT(onGettingLoggingWorkFinished(bool, int)));
    connect(this, SIGNAL(gettingLogfinished()), mGettingLogThread, SLOT(quit()));           // request to thread quit
    connect(mGettingLogThread, SIGNAL(finished()), this, SLOT(onGettingLogDeleteLater()));  // response from thread quit, we need to write thread exit code within onDeleteLater().
    connect(mGettingLogThread, SIGNAL(finished()), mGettingLogThread, SLOT(deleteLater()));

    mLogger->moveToThread(mGettingLogThread);
    mGettingLogThread->start();
}

void UserIface::setRaspberryTime()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << " &&&&  getTime ()" << __func__;
#endif
    getDateTime();
}

void UserIface::getTime()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << " &&&&  getTime ()" << __func__;
#endif
    getDateTime();
}

void UserIface::setSystemTime(int yy, int mm, int dd, int h, int m, int s)
{
    struct tm tm_src;
    // struct timeval tv = {0, 0};

    // struct timeval mytime;
    // struct tm *myLocalTime;
    // char myDateTime[128] = {0};
    QString timeAndDate;
    char timeString[128] = {0};

     int result;

    qDebug() << "system time 1 = " << yy << mm << dd << h << m << s;

    if(yy > 19)
        yy = 19;

    tm_src.tm_year = (yy+2000) - 1900;
    tm_src.tm_mon  = (mm - 1);
    tm_src.tm_mday = dd;
    tm_src.tm_hour = h;
    tm_src.tm_min  = m;
    tm_src.tm_sec  = s;

    // tv.tv_sec = mktime(&tm_src);

    // pi@raspberrypi:~$ sudo date –s “21 June 13:45 2012”

    strftime(timeString, 80, "%Y-%m-%d %H:%M:%S", &tm_src);
    //qDebug() << "TimeString : " << timeString;

    timeAndDate.append("sudo date -s \"");
    timeAndDate.append(timeString);
    timeAndDate.append("\"");
    qDebug() << " time : " << timeAndDate.toStdString().c_str();

    result = system(timeAndDate.toStdString().c_str());
    qDebug() << " result : " << result;

    mSystemTimeSettingStatus = true;

    // result = settimeofday(&tv, NULL);
    // if (result < 0) {
    //     qDebug() << "Error setting the Time! - " << result;
    // }
    // else {
    //     qDebug() << "Time has been configured!";
    // }

    // gettimeofday(&mytime, NULL);
    // myLocalTime = (struct tm*)localtime((time_t *)&mytime.tv_sec);
    // memset(myDateTime, 0x00, sizeof(myDateTime));

    // sprintf(myDateTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", myLocalTime->tm_year+1900, myLocalTime->tm_mon+1,
    //         myLocalTime->tm_mday, myLocalTime->tm_hour, myLocalTime->tm_min, myLocalTime->tm_sec, mytime.tv_usec/1000);

    // qDebug() << "system time 2 = " << myDateTime;
}

void UserIface::responseEmptyPayloadCmd(quint16 code)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "8 In(), msgCode : " << hex << code;
#endif
    int msgCode = (int)code;

    switch (msgCode) {

        default:
            break;
    }

}

void UserIface::responseCmd(quint8 code, quint8 getSet, quint8 *payload)
{

    int param1 = 0;
    int param2 = 0;
    int param3 = 0;
    int param4 = 0;
    int param5 = 0;
    int param6 = 0;

    int msgCode = (int)code;

    int msgGetSet = getSet;

    param1 = payload[0];
    param2 = payload[1];
    param3 = payload[2];
    param4 = payload[3];
    param5 = payload[4];
    param6 = payload[5];


#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " ** in responseCmd In(), msgCode : " << hex << msgCode;
//    qDebug() << __func__ << " ** in responseCmd In(), msgGetSet : " << hex << msgGetSet;
//    qDebug() << __func__ << " ** in responseCmd In(), param1 : " << param1;
//    qDebug() << __func__ << " ** in responseCmd In(), param2 : " << param2;
//    qDebug() << __func__ << " ** in responseCmd In(), param3 : " << param3;
//    qDebug() << __func__ << " ** in responseCmd In(), param4 : " << param4;
//    qDebug() << __func__ << " ** in responseCmd In(), param5 : " << param5;
//    qDebug() << __func__ << " ** in responseCmd In(), param6 : " << param6;
#endif

    switch (msgCode) {
        case KEEP_ALIVE_CMD:
//            break;
        case MAIN_FW_CMD:
//            break;
        case SUB_FW_CMD:
//            break;
        case DATE_CMD:
//            break;
        case CART_STATUS_CMD:
//            break;
        case INSPECTION_STATUS_CMD:
//            break;
        case SYSTEM_STATE_CMD:
//            break;
        case SYS_SET_STATE_CMD:
//            break;
        case ANGLE_DATA_CMD:
//            break;
        case ERROR_CMD:
//            break;
        default:
            emit resCmdFromPower(msgCode, msgGetSet, param1, param2, param3, param4, param5, param6);
            break;
    }
}

void UserIface::updateSw(int kinda)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    mUpdateKinda = kinda;
    updateSwApplication();
}

void UserIface::rmsUpdateSw(int kinda)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    mUpdateKinda = kinda;
    updateSwApplicationRms();
}

void UserIface::updateSwApplication()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    mSwUpdaterThread = new QThread(this);
    mSwUpdater = new SwUpdater();

    // connect UserIface <-> SwUpdater
    connect(mSwUpdaterThread, SIGNAL(started()), mSwUpdater, SLOT(do_work()));
    connect(mSwUpdater, SIGNAL(workFinished(bool, int)), this, SLOT(onSwUpdateWorkFinished(bool, int)));
    connect(this, SIGNAL(swUpdatefinished()), mSwUpdaterThread, SLOT(quit()));           // request to thread quit
    connect(mSwUpdaterThread, SIGNAL(finished()), this, SLOT(onSwUpdateDeleteLater()));  // response from thread quit, we need to write thread exit code within onDeleteLater().
    connect(mSwUpdaterThread, SIGNAL(finished()), mSwUpdaterThread, SLOT(deleteLater()));

    mSwUpdater->moveToThread(mSwUpdaterThread);
    mSwUpdaterThread->start();
}

void UserIface::updateSwApplicationRms()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__;
#endif
    mSwUpdaterThread = new QThread(this);
    mSwUpdater = new SwUpdater();

    // connect UserIface <-> SwUpdater
    connect(mSwUpdaterThread, SIGNAL(started()), mSwUpdater, SLOT(do_work_rms()));
    connect(mSwUpdater, SIGNAL(workFinished(bool, int)), this, SLOT(onSwUpdateWorkFinished(bool, int)));
    connect(this, SIGNAL(swUpdatefinished()), mSwUpdaterThread, SLOT(quit()));           // request to thread quit
    connect(mSwUpdaterThread, SIGNAL(finished()), this, SLOT(onSwUpdateDeleteLater()));  // response from thread quit, we need to write thread exit code within onDeleteLater().
    connect(mSwUpdaterThread, SIGNAL(finished()), mSwUpdaterThread, SLOT(deleteLater()));

    mSwUpdater->moveToThread(mSwUpdaterThread);
    mSwUpdaterThread->start();
}

void UserIface::updateFw(int kinda, bool rms)
{

    mUpdateKinda = kinda;
    mFWUpdateStatus = true;
    mRmsUpdate = rms;

#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "1. mUpdateKinda" << hex << mUpdateKinda;
    qDebug() << __func__ << "1. mUpdateKinda" << hex << mRmsUpdate;
#endif

    if (getConnectedSerial())
    {
        mSender->cmdReboot(mUpdateKinda);
    }
}

void UserIface::updateFwRms(int kinda, bool rms)
{

    mUpdateKinda = kinda;
    mFWUpdateStatus = true;
    mRmsUpdate = rms;

#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "2. mUpdateKinda" << hex << mUpdateKinda;
    qDebug() << __func__ << "2. rms: " <<  rms;
    qDebug() << __func__ << "2. mRmsUpdate: " <<  mRmsUpdate;
#endif

    if (getConnectedSerial())
    {
        mSender->cmdReboot(mUpdateKinda);
    }
}

void UserIface::updateFwMCU(int kinda)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "10 In(), kinda : " << hex << kinda;
#endif

    if (NULL != mSerial) {
        if (mSerial->isOpen())  {
            qDebug() << __func__ << "mSerial for main thread is opened. It will closed.";
            mSerial->close();
            qDebug() << __func__ << "mSerial is closed.";
        }
    }

    mFwUpdaterThread = new QThread(this);
    mYModem = new YMODEM(kinda);

    // connect UserIface <-> FwUpdater
    connect(mFwUpdaterThread, SIGNAL(started()), mYModem, SLOT(do_work()));
    connect(mYModem, SIGNAL(workFinished(bool, int)), this, SLOT(onFwUpdateWorkFinished(bool, int)));
    connect(mYModem, SIGNAL(checkedFile(QString)), this, SLOT(onCheckedFile(QString)));
    connect(mYModem, SIGNAL(progress(int)), this, SLOT(onFwUpdateProgress(int)));
    connect(this, SIGNAL(fwUpdatefinished()), mFwUpdaterThread, SLOT(quit()));           // request to thread quit
    connect(mFwUpdaterThread, SIGNAL(finished()), this, SLOT(onFwUpdateDeleteLater()));  // response from thread quit, we need to write thread exit code within onDeleteLater().
    connect(mFwUpdaterThread, SIGNAL(finished()), mFwUpdaterThread, SLOT(deleteLater()));

    mYModem->moveToThread(mFwUpdaterThread);
    mFwUpdaterThread->start();
}

void UserIface::updateFwMCURms(int kinda)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "10 In(), kinda : " << hex << kinda;
#endif

    if (NULL != mSerial) {
        if (mSerial->isOpen())  {
            qDebug() << __func__ << "mSerial for main thread is opened. It will closed.";
            mSerial->close();
            qDebug() << __func__ << "mSerial is closed.";
        }
    }

    mFwUpdaterThread = new QThread(this);
    mYModem = new YMODEM(kinda);

    // connect UserIface <-> FwUpdater
    connect(mFwUpdaterThread, SIGNAL(started()), mYModem, SLOT(do_work_rms()));
    connect(mYModem, SIGNAL(workFinished(bool, int)), this, SLOT(onFwUpdateWorkFinished(bool, int)));
    connect(mYModem, SIGNAL(checkedFile(QString)), this, SLOT(onCheckedFile(QString)));
    connect(mYModem, SIGNAL(progress(int)), this, SLOT(onFwUpdateProgress(int)));
    connect(this, SIGNAL(fwUpdatefinished()), mFwUpdaterThread, SLOT(quit()));           // request to thread quit
    connect(mFwUpdaterThread, SIGNAL(finished()), this, SLOT(onFwUpdateDeleteLater()));  // response from thread quit, we need to write thread exit code within onDeleteLater().
    connect(mFwUpdaterThread, SIGNAL(finished()), mFwUpdaterThread, SLOT(deleteLater()));

    mYModem->moveToThread(mFwUpdaterThread);
    mFwUpdaterThread->start();
}

void UserIface::initPort()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "11 In()";
#endif
    if (NULL == mSerial) {
        mSerial = new QSerialPort(this);
        qDebug() << mSerial << "created!";
        mSerial->setPortName(SERIAL_DEV);
        mSerial->setBaudRate(QSerialPort::Baud19200);
        mSerial->setDataBits(QSerialPort::Data8);
        mSerial->setParity(QSerialPort::NoParity);
        mSerial->setStopBits(QSerialPort::OneStop);
        mSerial->setFlowControl(QSerialPort::NoFlowControl);
        mSerial->setSettingsRestoredOnClose(false);
    }
    else {
        qDebug() << __func__ << "mSerial is not NULL";
    }
}

struct tm * UserIface::getCurrentTime()
{
    struct timeval curTimeVal;
    struct tm *getCurTime;

    gettimeofday(&curTimeVal, NULL);
    getCurTime = (struct tm *)localtime((time_t *)&curTimeVal.tv_sec);

    return getCurTime;
}

void UserIface::setNeedleClean(int mode)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "Clean mode: " << mode;
#endif
    if (getConnectedSerial())
    {
        mSender->cmdNeedleClean(mode);
    }
}

#if defined(KZ_DB)

void UserIface::searchPatient(QString text) {
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "searchPatient: " << text;
#endif

    db_search_patient(text);
}

void UserIface::searchTreatmentReport(int mode, int tap, int page)
{
    //for sql insert test
//    db_insert_treatmentReport(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"),11,22,33,44,55,66,77,88);

    db_search_treatmentReport(mode, tap, page);
}

//SKW 2019-01-24 // SKW 2019-02-14 drop table(added name option)
void UserIface::deletePatient(int id, QString firstName, QString lastName) {
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "db_delete_patient id : " << id;
#endif
    db_delete_patient_info(id, firstName, lastName); // SKW 2019-02-14 drop table(added name option)
}

void UserIface::setPatientInfo() {
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "20 In()";
#endif

    setCurrentPatientInfo();
}

void UserIface::setCurrentPatientInfo() {
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "20 In()";
    qDebug() << __func__ <<" m_patient_id: " << m_patient_id;
    qDebug() << __func__ <<" m_first_name: " << m_first_name;
    qDebug() << __func__ <<" m_last_name: " << m_last_name;
    qDebug() << __func__ <<" m_patient_birth_yy: " << m_patient_birth_yy;
    qDebug() << __func__ <<" m_patient_birth_mm: " << m_patient_birth_mm;
    qDebug() << __func__ <<" m_patient_birth_dd: " << m_patient_birth_dd;
    qDebug() << __func__ <<" m_patient_height: " << m_patient_height;
    qDebug() << __func__ <<" m_patient_weight: " << m_patient_weight;
    qDebug() << __func__ <<" m_patient_gender: " << m_patient_gender;
#endif

    if(m_patient_birth_yy == "0000" ||
            m_patient_birth_mm == "00" ||
            m_patient_birth_dd == "00" ||
            m_patient_height == "" || m_patient_height == "0" ||
            m_patient_weight == "" || m_patient_weight == "0" ||
            m_first_name =="noname" || m_last_name == "noname"
            )
    {
        qDebug() << __func__ <<"a patient_data is not enough.";
    }
    else
    {
        if(m_patient_id < 0)
        {
            qDebug() << __func__<<"CREATE"<<m_patient_id;
            m_patient_id = db_create_patient();
            qDebug()<<"CREATE VALUE: "<<m_patient_id;
        }
        else
        {
            qDebug() << __func__<<"UPDATE VALUE: "<<m_patient_id;
            db_update_patient_info(m_patient_id);
        }
    }
}

#endif



