#ifndef USERIFACE_H_
#define USERIFACE_H_

#include "common.h"
#include "sender.h"
#include "receiver.h"
#include "swupdater.h"
#include "logger.h"
#include "ymodem.h"
#include "patient.h"

//#if defined(RMS_SERVICE)
#include "websocketthread.h"
//#endif

#include <QList> // SKW 2019-02-13 treatment history

#if defined(KZ_UI)
#include "/home/wind/trunk/lib/conf/conf.h"
#endif
///// Add by JOON

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QFile>
#if defined(KZ_DB)
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#endif
#include <QProcess>
#include <QUdpSocket>
#include <QDir>
#include <QThread>

#include "gui.h"
#include "wifi.h"

#include "sys/time.h"

class UserIface : public QObject
{
    Q_OBJECT

#if defined(KZ_DB)
    Q_PROPERTY(int patient_id READ get_patient_id WRITE set_patient_id NOTIFY sig_patient_id)
    Q_PROPERTY(int patient_gender READ get_patient_gender WRITE set_patient_gender NOTIFY sig_patient_gender)
    Q_PROPERTY(QString first_name READ get_first_name WRITE set_first_name NOTIFY sig_first_name)
    Q_PROPERTY(QString last_name READ get_last_name WRITE set_last_name NOTIFY sig_last_name)
    Q_PROPERTY(QString patient_birth_yy READ get_patient_birth_yy WRITE set_patient_birth_yy NOTIFY sig_patient_birth_yy)
    Q_PROPERTY(QString patient_birth_mm READ get_patient_birth_mm WRITE set_patient_birth_mm NOTIFY sig_patient_birth_mm)
    Q_PROPERTY(QString patient_birth_dd READ get_patient_birth_dd WRITE set_patient_birth_dd NOTIFY sig_patient_birth_dd)
    Q_PROPERTY(QString patient_weight READ get_patient_weight WRITE set_patient_weight NOTIFY sig_patient_weight)
    Q_PROPERTY(QString patient_height READ get_patient_height WRITE set_patient_height NOTIFY sig_patient_height)

#endif

public:
    explicit UserIface(QObject *parent = 0);
    ~UserIface();

    static UserIface *getInstance();
    static UserIface *mInstance;

    void setConnectedSerial(bool);
    bool getConnectedSerial() const;

#if defined(KZ_UI)
    void set_font();   // KZ_UI

    void loadCommonSettings();
    void saveCommonSettings();

#endif

#if defined(KZ_DB)
    void init_db();

    Q_INVOKABLE void setPatientInfo();
    void setCurrentPatientInfo();

    Q_INVOKABLE void searchPatient(QString);
    Q_INVOKABLE void searchTreatmentReport(int mode, int tap, int page);
    Q_INVOKABLE void deletePatient(int, QString, QString);

#endif

    void quitThread();

    Q_INVOKABLE void setPageNo(int);
    void setCurrentPage(int _pageNo);
    Q_INVOKABLE int getPageNo(); //SKW 2019-03-06 TreatmentHistorySingleUser에서 페이지 정보 확인(S/W 중점관리 34번)

#if defined(KZ_UI)
    Q_INVOKABLE void getCartInfo(int);
    Q_INVOKABLE void setCartInfo(int, int, int, int, int, int);
    void setGetCartInfo(int hp_info, int setGet, int countryCode, int serialNo, int currCnt, int totalCnt, int tipType);
#endif

    Q_INVOKABLE void sendToPower(int, int, int, int, int, int, int, int);

    Q_INVOKABLE void startRMS(QString);   // 2019-03-07 RMS Start
    Q_INVOKABLE void stopRMS();    // 2019-03-07 RMS Stop
    Q_INVOKABLE void changeLogoHironic();
    Q_INVOKABLE void changeBiPlasonic_org();
    Q_INVOKABLE void changeBiPlasonic_s();
    Q_INVOKABLE void playAudioGuide(QString); //2019-01-17 SKW Play audio guide
    Q_INVOKABLE void stopAudioGuide(); //2019-01-22 SKW Stop audio guide

    Q_INVOKABLE void setTime(int, int, int, int, int, int);
    void setFwDate(int, int, int, int, int, int);

    Q_INVOKABLE void setTip(int, int, int);
    void setTipSettings(int, int, int);

    Q_INVOKABLE void setStatus(int);
    void setStandbyReady(int, int);

    void parseEmptyPayloadData(QByteArray);
    void parsePayloadData(QByteArray _recvData);

    Q_INVOKABLE void updateSw(int);
    void updateSwApplication();

    Q_INVOKABLE void rmsUpdateSw(int);
    void updateSwApplicationRms();

    Q_INVOKABLE void updateFw(int, bool);
    void updateFwMCU(int);

    Q_INVOKABLE void updateFwRms(int, bool);
    void updateFwMCURms(int);

    bool getUpdateStatus() const;
    Q_INVOKABLE void setSystemReboot(bool);

#if defined(KZ_UI)  // JOON_ADD
    Q_INVOKABLE void saveCommonSet(QString, QString, QString);

    Q_INVOKABLE void setNeedleClean(int);

    Q_INVOKABLE void getCurrPage();

    Q_INVOKABLE void getCurrCartInfo();

    Q_INVOKABLE void getCurrTipInfo();

    Q_INVOKABLE void saveSettings( bool);

#endif

    void setAckToMCU(quint16);
    void responseCmd(quint8, quint8, quint8 *);
    void responseEmptyPayloadCmd(quint16);

    Q_INVOKABLE void getLogger();
    void saveLogDataToUSB();

    Q_INVOKABLE void getTime();
    Q_INVOKABLE void setRaspberryTime();
    void getDateTime();
    void setSystemTime(int, int, int, int, int, int);

    QString getLoggingHPInfo(int);
    QString getLoggingStdbyReady(int);
    QString getLoggingOperMode(int);
    int getLoggingOperLevel(int);
    QString getLoggingErrCode(int);
    QString getLoggingTipSettingInfo(bool);
    void saveLogData();
    void saveLogDataOnAlram();
    void saveLogDataOnTipSettingComplete(bool);

#if defined(KZ_UI_DEBUG)
    void saveLogDataUartConnect();
#endif

    struct tm * getCurrentTime();
    Q_INVOKABLE void connectDev();


#if defined(KZ_UI_GET_HP)
    Q_INVOKABLE void getConnectedHP();

#endif

signals:
    void fwUpdatefinished();
    void swUpdatefinished();
    void gettingLogfinished();
    void connectedSerial(bool connected, int curPage);
    void resSetLevel(bool status);
    void resSetStdbyReady(bool status);
    void resSetBuzzer(bool status);
    void resFwSetDate(bool status);
    void updateStatus(bool status, int result);
    void disconnectHP(int hp, bool status);
    void alramAlert(int alarmHP, int alarmCode);
    void gettingLogStatus(int status, int result);
    void fwUpdateStatus(int status, int result);
    void swUpdateStatus(int status, int result);

    void fwProgress(int progress);
    void fwVersion(int major, int second, int third, int middle);
    void fwFullVersion(int main, int sono);
    void fwCheckedFile(QString fileName);
    void resDateTimeComparison(bool status);

    void resResetTotalTime(bool status);
    void reqPsTipInfoRMTime(int tipRemainHour, int tipRemainMin, int tipRemainSec, double tipSerialNo, int tipReleaseYear, int tipReleaseMon, int tipReleaseDay); // 2018-05-04 for Factory mode
    void reqPsInfoCurTime(int currYear, int currMon, int currDay); // 2018-05-10 for Factory mode
    void reqGetInfoCurTime(int currYear, int currMon, int currDay, int currHour, int currMinute); // 2018-05-10 for Factory mode
#if defined(KZ_UI_GET_HP)
    void currentHP(int hp, bool status);
    void connectedHP(int connRmHP, int connRvHP, int connRnHP, int connRcHP, int connPsHP, int connGpHP, int currPage);
    void resCurSelectHpInfo(int powerTemp,int  repetitionTime,int  powerDepth,int  rfOntime,int  delayTime,int  alarmCount,int  alarmCountMax);
    void resSelectCartInfo(char hpConnect, QString cartridgeInfo, int cartNationCode, double cartSerial, int cartFreq, QString cartridgeType,
                                           char cartPowerCal, double cartCurrentCount, double cartMaxCount);
    void resSelectTipInfo(QString cartridgeInfo, int cartNationCode, double cartSerial);  // PS Tip Info
    void resSelectPageInfo(int currPage);
    void reqSendOperationCartInfo(int curHpInfo, int operationInfo, int cartAlarmCount, double cartRemainCount);
#endif

    void resCmdFromPower(int cmd, int getSet, int  data1,int  data2,int  data3,int  data4, int data5, int data6);

#if defined(KZ_DB)
    void sig_patient_gender();
    void sig_patient_num();// SKW 2019-04-16 num(시술기록번호), feedback 추가
    void sig_first_name();
    void sig_last_name();
    void sig_patient_birth_yy();
    void sig_patient_birth_mm();
    void sig_patient_birth_dd();
    void sig_patient_id();
    void sig_patient_weight();
    void sig_patient_height();

    // 2019-02-13 SKW treatment history
    void sig_patient_history_count();
    void sig_reg_date();
    void sig_hp();
    void sig_face();
    void sig_rmPinInfo();
    void sig_psOpMode();
    void sig_power();
    void sig_repetition();
    void sig_depth();
    void sig_ontime();
    void sig_delay();
    void sig_alarm();
    void sig_alarm_max();
    void sig_patient_history();
    void sig_usedShot(); // SKW 2019-02-25 add usedShotCount
    void sig_usedTime(); // SKW 2019-02-25 add usedShotCount
    void sig_feedback(); // SKW 2019-04-16 num(시술기록번호), feedback 추가
    // 2019-02-13 SKW treatment history

    void resSearchPatient(int count, QString year, QString month,  QString day, int gender, QString firstName, QString lastName, int id, QString height, QString weight);
    void resReportsTreatments(int idx, QString time, int fluence, int pulse, int spot, int nm, int count, int hz, int cooling, int code);

    void resTreatmentSettings(int resMode, int resNum, QString resSaveName, int resPre, int resDelay, int resPost,
                              int resFluence, int resPulseDuration, int resPulseRate, int resSpotSize, int resBeamShape);

    // 2019-02-19 SKW treatment history// SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
    void reSelectTreatmentHistory(int count, int id, int num, QString firstName, QString lastName, QString reg_date,
                                  QString hp, QString face, QString rmPinInfo, QString psOpMode, int power,
                                  int repetition, int depth, int ontime, int delay, int alarm,
                                  int alarm_max, int usedShot, int usedTime, int feedback);

    // 2019-02-12 SKW treatment history// SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
    void reSettingTreatmentHistory(int count, int id, int num, QString firstName, QString lastName, QString reg_date,
                                   QString hp, QString face, QString rmPinInfo, QString psOpMode, int power,
                                   int repetition, int depth, int ontime, int delay, int alarm,
                                   int alarm_max, int usedShot, int usedTime, int feedback);

    // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
    // SKW 2019-02-22 reDetailTreatmentHistory //db_select_treatmentHistory를 사용시 화면 정보가 겹칠 수 있어 유사하게 다시 구현
    void reDetailTreatmentHistory(int count, int id, int num, QString firstName, QString lastName, QString reg_date,
                                  QString hp, QString face, QString rmPinInfo, QString psOpMode, int power,
                                  int repetition, int depth, int ontime, int delay, int alarm,
                                  int alarm_max, int usedShot, int usedTime, int feedback);


#endif
    void resScanResult(int scanedAp);

// SKW 2019-01-31 voltcheck
    void voltcheck (QString check_temperature, QString check_core, QString check_sdram_c, QString check_sdram_i, QString check_sdram_p, float total_volt);

public slots:
    void onFwUpdateDeleteLater();
    void onSwUpdateDeleteLater();
    void onGettingLogDeleteLater();
    void onParseData(QByteArray);
    void closing();
    void onFwUpdateWorkFinished(bool, int);
    void onSwUpdateWorkFinished(bool, int);
    void onGettingLoggingWorkFinished(bool, int);
    void onFwUpdateProgress(int);
    void onCheckedFile(QString);

#if defined(KZ_UI)
    void proc();

#if defined(KZ_DB)
    int db_create_patient();
    int db_last_insert_rowid();
    void db_insert_patient_info(int);
    void db_update_patient_info(int);
    void db_search_patient(QString);
    void db_selected_patient(int);
    void db_delete_patient_info(int, QString, QString);// SKW 2019-01-24 DB Delete // SKW 2019-02-14 drop table(added name option)
    void db_save_treatment_info(int, int); //SKW 2019-02-12 treatment history //SKW 2019-03-22
    void db_select_treatmentHistory(QString); //SKW 2019-02-13 treatment history //SKW 2019-04-18 SELECT문 개선
    void db_setting_treatmentHistory(int); //SKW 2019-02-19 setting treatment history
    void db_backup_to_CSV(); //SKW 2019-02-19 DB backup to CSV
    void db_read_detailHistory(int, QString, QString, QString); // SKW 2019-02-22 csv read
    bool db_check_id(int, QString, QString);// SKW 2019-02-26 id check
    void db_checkAndCreate(QString, QString, QString, QString, QString, int); // SKW 2019-03-22 이름이 동일하면 성별, 이름 성씨, 생년월일이 달라도 동일인물로 여겨 저장 안되는 부분. 중복검사 재구성
    bool db_awsUpload_check(QString); //2019-04-08 AWS S3에 로그를 보냈는지 장비에서 알 수 있는 방법 추가
    void db_patien_info_init(); //SKW 2019-04-09 환자정보 전역변수 초기화 (로그인 해제 시 사용)
    void db_update_feedback(QString, int, int); // SKW 2019-04-15 만족도 점수 업데이트
    int db_select_treatmentHistory_visitCount(); //SKW 2019-04-22 총 방문 횟수를 전달 (TreatmentHistory.qml > visitCount)
    void db_save_errorCode_info(int); //SKW 2019-05-07 주요 목적: 소프트웨어에서 펌웨어 연결 여부를 확인 후 에러코드 기록

    /*LightZen DB*/
    Q_INVOKABLE int db_save_treatmentSetting(int mode, int num, QString saveName, int pre, int delay, int post,
                                  int fluence, int pulseDuration, int pulseRate, int spotSize, int beamShape);
    Q_INVOKABLE int db_save_renameTreatmentSetting(int mode, int num, QString saveName);
    Q_INVOKABLE int db_get_treatmentSetting(int mode, int num);

    void db_search_treatmentReport(int mode, int tap, int page);
    int db_insert_treatmentReport(QString date, int fluence, int pulse, int spot,
                                   int nm, int count, int hz, int cooling, int code);

#endif

public:
    GUI *m_gui;
    QTimer m_proc;
//    Conf m_conf_settings;
#if defined(KZ_DB)
    QSqlDatabase m_db;
#endif
    Conf mConfSettings;
    Conf mConfRmNasolabialMemory;
    Conf mConfRmCheekMemory;
    Conf mConfRmJawlineMemory;
    Conf mConfRmNeckMemory;
    Conf mConfRmNoseMemory;
    Conf mConfRmHeadMemory;

    Conf mConfRvMemory;
    Conf mConfRnMemory;
    Conf mConfRcMemory;

#if defined(DF_UI)
    Conf mConfDFSettings;
#endif

//#if defined(RMS_SERVICE)
    WebSocketThread m_webSocket;
//#endif

#if defined(KZ_DB)
    Patient m_patient_model;

    // PATIENT INFORMATION  //SKW 2019-02-12 treatment history. get/set 함수 앞에 Q_INVOKABLE 추가
    int m_patient_id = -1;
    Q_INVOKABLE int get_patient_id() { return m_patient_id; }
    Q_INVOKABLE void set_patient_id(int val) { m_patient_id = val; sig_patient_id(); }
    int m_patient_gender;
    Q_INVOKABLE int get_patient_gender() { return m_patient_gender; }
    Q_INVOKABLE void set_patient_gender(int val) { m_patient_gender = val; sig_patient_gender(); }

    QString m_first_name;
    Q_INVOKABLE QString get_first_name() { return m_first_name; }
    Q_INVOKABLE void set_first_name(QString val) { m_first_name = val; sig_first_name(); }
    QString m_last_name;
    Q_INVOKABLE QString get_last_name() { return m_last_name; }
    Q_INVOKABLE void set_last_name(QString val) { m_last_name = val; sig_last_name(); }
    QString m_patient_birth_yy;
    Q_INVOKABLE QString get_patient_birth_yy() { return m_patient_birth_yy; }
    Q_INVOKABLE void set_patient_birth_yy(QString val) { m_patient_birth_yy = val; sig_patient_birth_yy(); }
    QString m_patient_birth_mm;
    Q_INVOKABLE QString get_patient_birth_mm() { return m_patient_birth_mm; }
    Q_INVOKABLE void set_patient_birth_mm(QString val) { m_patient_birth_mm = val; sig_patient_birth_mm(); }
    QString m_patient_birth_dd;
    Q_INVOKABLE QString get_patient_birth_dd() { return m_patient_birth_dd; }
    Q_INVOKABLE void set_patient_birth_dd(QString val) { m_patient_birth_dd = val; sig_patient_birth_dd(); }
    QString m_patient_weight;
    Q_INVOKABLE QString get_patient_weight() { return m_patient_weight; }
    Q_INVOKABLE void set_patient_weight(QString val) { m_patient_weight = val; sig_patient_weight(); }
    QString m_patient_height;
    Q_INVOKABLE QString get_patient_height() { return m_patient_height; }
    Q_INVOKABLE void set_patient_height(QString val) { m_patient_height = val; sig_patient_height(); }


    // SKW 2019-02-13 treatment history
    int m_patient_history_count;
    Q_INVOKABLE int get_patient_history_count() { return m_patient_history_count; }
    Q_INVOKABLE void set_patient_history_count(int val) { m_patient_history_count = val; sig_patient_history_count(); }
    QString m_reg_date;
    Q_INVOKABLE QString get_reg_date() { return m_reg_date; }
    Q_INVOKABLE void set_reg_date(QString val) { m_reg_date = val; sig_reg_date(); }
    QString m_hp;
    Q_INVOKABLE QString get_hp() { return m_hp; }
    Q_INVOKABLE void set_hp(QString val) { m_hp = val; sig_hp(); }
    QString m_face;
    Q_INVOKABLE QString get_face() { return m_face; }
    Q_INVOKABLE void set_face(QString val) { m_face = val; sig_face(); }
    QString m_rmPinInfo;
    Q_INVOKABLE QString get_rmPinInfo() { return m_rmPinInfo; }
    Q_INVOKABLE void set_rmPinInfo(QString val) { m_rmPinInfo = val; sig_rmPinInfo(); }
    QString m_psOpMode;
    Q_INVOKABLE QString get_psOpMode() { return m_psOpMode; }
    Q_INVOKABLE void set_psOpMode(QString val) { m_psOpMode = val; sig_psOpMode(); }
    int m_power;
    Q_INVOKABLE int get_power() { return m_power; }
    Q_INVOKABLE void set_power(int val) { m_power = val; sig_power(); }
    int m_repetition;
    Q_INVOKABLE int get_repetition() { return m_repetition; }
    Q_INVOKABLE void set_repetition(int val) { m_repetition = val; sig_repetition(); }
    int m_depth;
    Q_INVOKABLE int get_depth() { return m_depth; }
    Q_INVOKABLE void set_depth(int val) { m_depth = val; sig_depth(); }
    int m_ontime;
    Q_INVOKABLE int get_ontime() { return m_ontime; }
    Q_INVOKABLE void set_ontime(int val) { m_ontime = val; sig_ontime(); }
    int m_delay;
    Q_INVOKABLE int get_delay() { return m_delay; }
    Q_INVOKABLE void set_delay(int val) { m_delay = val; sig_delay(); }
    int m_alarm;
    Q_INVOKABLE int get_alarm() { return m_alarm; }
    Q_INVOKABLE void set_alarm(int val) { m_alarm = val; sig_alarm(); }
    int m_alarm_max;
    Q_INVOKABLE int get_alarm_max() { return m_alarm_max; }
    Q_INVOKABLE void set_alarm_max(int val) { m_alarm_max = val; sig_alarm_max(); }

    //SKW 2019-02-25 shotCount
    int m_usedShot;
    Q_INVOKABLE int get_usedShot() { return m_usedShot; }
    Q_INVOKABLE void set_usedShot(int val) { m_usedShot = val; sig_usedShot(); }
    int m_usedTime;
    Q_INVOKABLE int get_usedTime() { return m_usedTime; }
    Q_INVOKABLE void set_usedTime(int val) { m_usedTime = val; sig_usedTime(); }

    //SKW 2019-03-28 RM_PageInfo
    int m_RmPage;
    Q_INVOKABLE int get_RmPage() { return m_RmPage; }
    Q_INVOKABLE void set_RmPage(int val) { m_RmPage = val;  }

    //SKW 2019-03-28 total shot
    int m_rmTotalShot; // = mRmTotalCount;
//    Q_INVOKABLE int get_rmTotalShot() { return m_rmTotalShot ; }
//    Q_INVOKABLE void set_rmTotalShot(int val) { m_rmTotalShot  = val; sig_usedShot(); }
    Q_INVOKABLE QString get_rmTotalShot() { return mRmTotalCount ; }
    Q_INVOKABLE void set_rmTotalShot(QString val) { mRmTotalCount  = val;  }
    int m_rnTotalShot; // = mRnTotalCount;
//    Q_INVOKABLE int get_rnTotalShot() { return m_rnTotalShot ; }
//    Q_INVOKABLE void set_rnTotalShot(int val) { m_rnTotalShot  = val; sig_usedShot(); }
    Q_INVOKABLE QString get_rnTotalShot() { return mRnTotalCount ; }
    Q_INVOKABLE void set_rnTotalShot(QString val) { mRnTotalCount  = val;  }

    // SKW 2019-04-16 num(시술기록번호), feedback 추가
    int m_patientListNum;
    Q_INVOKABLE int get_patientListNum() { return m_patientListNum; }
    Q_INVOKABLE void set_patientListNum(int val) { m_patientListNum = val;  }
    int m_feedback;
    Q_INVOKABLE int get_feedback() { return m_feedback; }
    Q_INVOKABLE void set_feedback(int val) { m_feedback = val;  }

#endif

// WIFI_TEST    Wifi m_wifi;
    QStringList m_scan_list;

public slots:
    Q_INVOKABLE void clear_conf();
    Q_INVOKABLE int scan();
    Q_INVOKABLE QString get_scan_list_at(int);
    Q_INVOKABLE int conn(QString, QString);
    Q_INVOKABLE QString get_address();

#endif

private:
    void init();
    void initPort();
    void initCommonSettings();

    QThread *mSwUpdaterThread;
    QThread *mFwUpdaterThread;
    QThread *mGettingLogThread;
    Sender *mSender;
    Receiver *mReceiver;
    SwUpdater *mSwUpdater;
    QSerialPort *mSerial;
    YMODEM *mYModem;
    struct message *mMsg;
    struct emptyPayloadMsg *mEmptyPayloadMsg;
    Logger *mLogger;

    bool mIsconnectedSerial;
    bool mLastIsconnectedSerial;
    bool mIsUpdateStatus;
    bool mSystemTimeSettingStatus;
    bool mFWUpdateStatus;
    bool mSWUpdateStatus;
    bool mRESTipSettingStatus;
    bool mRmsUpdate;
    int mCurPageNo;
    int mCurHpInfo;
    int mHpConnect;
    int mSpotSize;
    int mBeamShape;
    int mTipSerialNo;
    int mTreatmentTimeMin;
    int mTreatmentTimeSec;
    int mTipRemainHour;
    int mTipRemainMin;
    int mTipRemainSec;
    int mOperationInfo;
    int mOperationMode;
    int mOperationLevel;
    int mStandbyReady;
    int mBuzzerSound;
    int mDateYear, mSettingDateYear;
    int mDateMon, mSettingDateMon;
    int mDateDay, mSettingDateDay;
    int mDateHour, mSettingDateHour;
    int mDateMin, mSettingDateMin;
    int mDateSec, mSettingDateSec;
    int mTipReleaseYear;
    int mTipReleaseMon;
    int mTipReleaseDay;
    int mUpdateKinda;
    int mFwMainVersion;
    int mFwSonoVersion;
    int malarmCode;
    int mAlarmHP;
    int mNeedleStatus;

    int mLoggingHpInfo;
    int mLoggingUsedTipCnt;
    unsigned long mLoggingTotalTMTime;    // Total TreatMent Time
    int mLoggingTipRMTimeHour;      // Tip ReMaining Time
    int mLoggingTipRMTimeMin;
    int mLoggingTipRMTimeSec;

    struct tm mStartTreatmentTime;
    struct tm mEndTreatmentTime;
    struct tm mTipSettingTime;

    int mResetTotalTimeHpInfo;

#if defined(DF_UI)
    char mCcdOnOff;                     //  'ccd
    int mCcdPreSpray;                   //  'pre'
    int mCcdDelay;                      //  'delay'
    int mCcdPostSpray;                  //  'post'
    int mAimingBeam;                    //   'aiming_beam'
    int mPulseDuration;                 //   'pulse_duration'

    int mFlunceEnergy;                  //   'fluence'
    int mRepetitionRate;                //   'repetition_rate'

    int mCalibrate;                     //    Reserved
    int mPurge;                         //    purge
    int mHPSelection;                   //    hp_manual_selection
    int mFractionMode;                  //   faractional_mode
    int mWaveForm;                      //   755/1064

    int mPsuVol;
    int mPulseRate;
    char mSporSize;                     //    spot_size

//    int mCurHpInfo;                     //    mCurHpInfo  defined already
#endif


#if defined(KZ_UI)
    int mPowerTemp;                 // int
    int mRepetition;
    int mSpacingRfpowerDepth;
    int mLengthRfOntime;
    int mTreatDelayTime;
    int mAlarmCount;
    int mAlarmCountMax;


    QString lang_type;
    int mLangType;

// SETTINGS
    QString mSettingsLang;
    QString mSettingsGuideLang;
    QString mSettingsEffectVol;
    QString mSettingsGuideVol;
    QString mSettingsLcdVol;
    QString mSettingsSoundMute;

    // Initialize
    QString mFactoryInit;  // 2018-05-10 JJHWANG
    QString mAgingMode;  // 2018-05-10 JJHWANG
    QString mAgingCount;
    QString mRmTotalCount;
    QString mRnTotalCount;
    QString mSerialNumber;
    QString mTotalLogFile;

    char mCartInfo[6];
//    double mCartridgeInfo;
    QString mCartridgeInfo;
    int mCartNationCode;
    double mCartSerial;
    int mCartFreq;
    char mCartType[4];
    QString mCartridgeType;
    char mCartPowerCal;
    double mCartCurrentCount;
    double mCartMaxCount;
    double mCartCurrentCountBack;
    int mCartAlarmCount;
    double mCartRemainCount;
#endif
};

#endif // USERIFACE_H_
