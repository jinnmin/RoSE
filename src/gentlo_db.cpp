#include "useriface.h"
#include "patient.h"

#if defined(KZ_DB)
void UserIface::init_db()
{

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("/home/app/data/person.db");
    if(m_db.open() == false)
    {
        qDebug() << __func__ <<"connect to DB is failed.";
    }
    QStringList strQryList; strQryList.clear();
    // DEFAULT
    strQryList +=
            "CREATE TABLE 'tb_treatmentReport' ("
            "   'idx'	INTEGER NOT NULL,"
            "   'time'	DATETIME NOT NULL,"
            "   'fluence'	INTEGER,"
            "   'pulse'	INTEGER,"
            "   'spot'	INTEGER,"
            "   'nm'	INTEGER,"
            "   'count'	INTEGER,"
            "   'hz'	INTEGER,"
            "   'cooling'	TEXT,"
            "   'code'	INTEGER,"
            "   PRIMARY KEY('idx')"
            ");"
            ;

    strQryList +=
            "CREATE TABLE 'tb_treatmentSetting' ("
            "	'idx'	INTEGER NOT NULL,"
            "	'mode'	INTEGER ,"
            "	'save_num'	INTEGER,"
            "	'save_name'	TEXT ,"
            "	'pre'	INTEGER ,"
            "	'delay'	INTEGER ,"
            "	'post'	INTEGER ,"
            "	'fluence'	INTEGER ,"
            "	'pulse_duration'	INTEGER ,"
            "	'pulse_rate'	INTEGER ,"
            "	'spot_size'	INTEGER ,"
            "	'beam_shape'	INTEGER ,"
            "	PRIMARY KEY('idx')"
            "   UNIQUE('mode', 'save_num')"
            ");"
            ;

    for(int i=1; i<=10; i++){
        strQryList+=QString("INSERT INTO tb_treatmentSetting (save_num, mode) VALUES (%1, 755)").arg(i);
        strQryList+=QString("INSERT INTO tb_treatmentSetting (save_num, mode) VALUES (%1, 1064)").arg(i);
    }


    QSqlQuery qry;
    foreach(QString strQry, strQryList)
    {
        if(qry.exec(strQry) == false)
        {
            qDebug() << __func__ << "SQL_ERROR: "<<strQry;
            qDebug() << __func__ << "SQL_ERROR: "<<qry.lastError();
        }
    }

}
#endif

void UserIface::loadCommonSettings() {

    mSettingsLang = mConfSettings.get("lang");
    mSettingsGuideLang = mConfSettings.get("guide_lang");
    mSettingsGuideVol = mConfSettings.get("guide_vol");
    mSettingsLcdVol = mConfSettings.get("lcd_vol");
    mSettingsSoundMute = mConfSettings.get("sound_mute");
    mFactoryInit = mConfSettings.get("factory_init");
    mAgingMode = mConfSettings.get("aging_mode");
    mAgingCount = mConfSettings.get("aging_count");
//    mSerialNumber = mConfSettings.get("serial_number");

#if defined(KZ_UI_DEBUG)  // Test Code
        qDebug() << __func__ << " ^^^^^^^^^^^^^^^  mSettingsLang: " <<  mSettingsLang;
#endif
}


void UserIface::saveCommonSettings()
{

    mConfSettings.set("lang", mSettingsLang);
    mConfSettings.set("guide_lang", mSettingsGuideLang);
    mConfSettings.set("guide_vol", mSettingsGuideVol);
    mConfSettings.set("lcd_vol", mSettingsLcdVol);
    mConfSettings.set("sound_mute", mSettingsSoundMute);
    mConfSettings.set("factory_init", mFactoryInit);
//    mConfSettings.set("serial_number", mSerialNumber);
    mConfSettings.write();
}

#if defined(KZ_DB)

int UserIface::db_create_patient()
{
    if(m_patient_birth_mm.toInt() < 10 && m_patient_birth_mm.length() < 2) {
        m_patient_birth_mm.insert(0, QChar('0'));
    }
    if(m_patient_birth_dd.toInt() < 10 && m_patient_birth_dd.length() < 2) {
        m_patient_birth_dd.insert(0, QChar('0'));
    }

    qDebug() << __func__<< "m_first_name" <<m_first_name;
    qDebug() << __func__<< "m_last_name" <<m_last_name;

    QString strQry = QString("insert into tb_person(first_name, last_name, gender, birth) values('%1', '%2', %3, '%4');")
            .arg(m_first_name).arg(m_last_name).arg(m_patient_gender).arg(m_patient_birth_yy+m_patient_birth_mm+m_patient_birth_dd);
    QSqlQuery qry;

    qDebug()<<strQry;

    if(qry.exec(strQry) == false)
    {
        qDebug() << __func__<< strQry;
        qDebug() << __func__<< qry.lastError();
        return -1;
    }

    int id = db_last_insert_rowid();
    qDebug() << __func__<< id;
    db_insert_patient_info(id); //db_last_insert_rowid());
    return id;

}

int UserIface::db_last_insert_rowid()
{
    QString strQry = QString("select last_insert_rowid() as id;");
    QSqlQuery qry;

qDebug() << __func__ <<" ### SQL: " << strQry;
    if(qry.exec(strQry) == false)
    {
qDebug() << __func__<< " ### SQL: " << strQry;
qDebug() << __func__<<" ### SQL Error: " <<qry.lastError();
        return -1;
    }

    int rowid = -1;
    if(qry.next())
    {
        rowid = qry.record().value("id").toInt();
    }

    return rowid;
}

void UserIface::db_insert_patient_info(int id)
{
    if(id < 0) {
qDebug() << __func__<<"Patient id is wrong. "<<id; return;
    }
    QDateTime dt = QDateTime::currentDateTime();
    QString strQry = QString("insert into tb_person_info(id, height, weight, section, treatment, date) "
            "values(%1, '%2', '%3', '%4', '%5', '%6');")
            .arg(id).arg(m_patient_height).arg(m_patient_weight)
            .arg(m_patient_height).arg(m_patient_weight)  // JOON_TEST
// JOON_TEST            .arg(m_patient_treatment_section).arg(m_patient_treatment_info)
            .arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
    QSqlQuery qry;
qDebug() << __func__<<strQry;
    if(qry.exec(strQry) == false)
    {
        qDebug() << __func__<< " ### SQL: " << strQry;
        qDebug() << __func__<<" ### SQL Error: " <<qry.lastError();
        return ;
    }
    QString strQry2 = QString("insert into tb_person_lastdate(id, last_date) "
            "values(%1, '%2');")
            .arg(id)
            .arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
    QSqlQuery qry2;
qDebug() << __func__<<strQry2;
    if(qry.exec(strQry2) == false)
    {
        qDebug() << __func__<<strQry2;
        qDebug() << __func__<<qry2.lastError();
        return ;
    }
}

void UserIface::db_update_patient_info(int id)
{
    if(id < 0) {
        qDebug()<<"Patient id is wrong. "<<id; return;
    }
    QDateTime dt = QDateTime::currentDateTime();
    QString strQry = QString("insert into tb_person_info(id, height, weight, section, treatment, date) "
            "values(%1, '%2', '%3', '%4', '%5', '%6');")
            .arg(id).arg(m_patient_height).arg(m_patient_weight)
            .arg(m_patient_height).arg(m_patient_weight)  // JOON_TEST
// JOON_TEST            .arg(m_patient_treatment_section).arg(m_patient_treatment_info)
            .arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
    QSqlQuery qry;
    qDebug() << __func__<< "1. strQry: " << strQry;

    if(qry.exec(strQry) == false)
    {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry.lastError();
        return ;
    }
    QString strQry2 = QString("update tb_person_lastdate set last_date = '%1' where id=%2;")
            .arg(dt.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(id);
    QSqlQuery qry2;
    qDebug() << strQry2;

    if(qry.exec(strQry2) == false)
    {
        qDebug() << __func__<<strQry2;
        qDebug() << __func__<<qry2.lastError();
        return ;
    }
}
#endif

#if 1

int UserIface::db_save_treatmentSetting(int mode, int num, QString saveName, int pre, int delay, int post,
                                        int fluence, int pulseDuration, int pulseRate, int spotSize, int beamShape)
{
    qDebug()<<"db_save_treatmentSetting :  ";
    QString strQry;

    strQry = QString("UPDATE tb_treatmentSetting SET save_name='%1', pre=%2, delay=%3, post=%4, fluence=%5, "
                     "pulse_duration=%6, pulse_rate=%7, spot_size=%8, beam_shape=%9 "
                     "WHERE save_num=%10 AND mode=%11;")
            .arg(saveName).arg(pre).arg(delay).arg(post).arg(fluence)
            .arg(pulseDuration).arg(pulseRate).arg(spotSize).arg(beamShape)
            .arg(num).arg(mode);



    qDebug()<<"strQry --> "<<strQry;

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return -1;
    }

    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        return -2;
    }

    return 0;
}

int UserIface::db_save_renameTreatmentSetting(int mode, int num, QString saveName)
{
    qDebug()<<"db_save_renameTreatmentSetting :  ";
    QString strQry;

    strQry = QString("UPDATE tb_treatmentSetting SET save_name='%1'"
                     "WHERE save_num=%10 AND mode=%11;")
            .arg(saveName)
            .arg(num).arg(mode);



    qDebug()<<"strQry --> "<<strQry;

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return -1;
    }

    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        return -2;
    }

    return 0;
}

int UserIface::db_get_treatmentSetting(int mode, int num)
{
    qDebug()<<"db_get_treatmentSetting : mode --> "<<mode<<"  num"<<num;
    QString strQry;

    strQry = QString("SELECT * FROM tb_treatmentSetting WHERE mode=%1 AND save_num=%2")
            .arg(mode).arg(num);

    qDebug()<<"strQry --> "<<strQry;

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return -1;
    }

    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        return -2;
    }

    while (qry->next()) {
        qDebug()<<"test db :: "<<qry->value(3).toString();

        mCcdPreSpray = qry->value(4).toInt();
        mCcdDelay = qry->value(5).toInt();
        mCcdPostSpray = qry->value(6).toInt();
        mFlunceEnergy = qry->value(7).toInt();
        mPulseDuration = qry->value(8).toInt();
        mPulseRate = qry->value(9).toInt();
        mSpotSize = qry->value(10).toInt();
        mBeamShape = qry->value(11).toInt();

        emit resTreatmentSettings(qry->value(1).toInt(), qry->value(2).toInt(), qry->value(3).toString(),qry->value(4).toInt(),
                                  qry->value(5).toInt(), qry->value(6).toInt(), qry->value(7).toInt(), qry->value(8).toInt(),
                                  qry->value(9).toInt(), qry->value(10).toInt(), qry->value(11).toInt()
                                  );
    }

    delete qry;
}

/******************************** db_search_treatmentReport ********************************
 * Get data according to the currently selected tab
 * tap: 1 --> get all data, LIMIT 11 OFFSET startPage
 * tap: 2 --> get code>0 data, LIMIT 11 OFFSET startPage
 * @ parm int tap : Treatments OR LastFaults
 * @ parm int page : Page number of the screen to show
*******************************************************************************************/
void UserIface::db_search_treatmentReport(int mode, int tap, int page)
{
    qDebug()<<"db_search_treatmentReport : page --> "+page;
    QString strQry;
    int startPage;

    if(page!=0 && page != 1)
        startPage = (page*11)-11;
    else
        startPage = 0;

    if(tap==2){
        if(mode == 755)         //pageAlex
            strQry = QString("SELECT * FROM tb_treatmentReport WHERE code>0 AND nm=755  ORDER BY idx DESC LIMIT 11 OFFSET %1").arg(startPage);
        else if(mode == 1064)   //pageNdyag
            strQry = QString("SELECT * FROM tb_treatmentReport WHERE code>0 AND nm=1064 ORDER BY idx DESC LIMIT 11 OFFSET %1").arg(startPage);
        else
            return;
    }

    else{
        if(mode == 755)         //pageAlex
            strQry = QString("SELECT * FROM tb_treatmentReport WHERE nm=755  ORDER BY idx DESC LIMIT 11 OFFSET %1").arg(startPage);
        else if(mode == 1064)   //pageNdyag
            strQry = QString("SELECT * FROM tb_treatmentReport WHERE nm=1064 ORDER BY idx DESC LIMIT 11 OFFSET %1").arg(startPage);
        else
            return;
    }


    qDebug()<<"strQry --> "<<strQry;

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return;
    }

    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        return ;
    }

    int rowIdx=startPage;
    while (qry->next()) {
        rowIdx++;
        emit resReportsTreatments(rowIdx, qry->value(1).toString(), qry->value(2).toInt(), qry->value(3).toInt(), qry->value(4).toInt(),
                                  qry->value(5).toInt(), qry->value(6).toInt(), qry->value(7).toInt(), qry->value(8).toInt(), qry->value(9).toInt());
    }

    delete qry;
}

/******************************** db_insert_treatmentReport ********************************
 * insert into [tb_treatmentReport] data
 * @ parm QString date, int fluence, int pulse, int spot, int nm, int count, int hz, int cooling, int code
 * @ return : negative number in case of SQL error
*******************************************************************************************/
int UserIface::db_insert_treatmentReport(QString date, int fluence, int pulse, int spot, int nm, int count, int hz, int cooling, int code)
{
    QString strQry;


    strQry = QString("INSERT INTO tb_treatmentReport (time, fluence, pulse, spot, nm, count, hz, cooling, code)"
                     "VALUES ('%1', %2, %3, %4, %5 ,%6 , %7, %8 , %9);")
            .arg(date).arg(fluence).arg(pulse).arg(spot).arg(nm).arg(count).arg(hz).arg(cooling).arg(code);


    qDebug()<<"strQry --> "<<strQry;

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return -1;
    }

    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        return -2;
    }

    delete qry;
    return 0;
}

void UserIface::db_search_patient(QString word)
{
    QString strQry = QString(
        "select tp.gender, tp.id as id, tp.first_name as first_name, tp.last_name as last_name, tp.birth as birth, tpi.height as height, tpi.weight as weight from tb_person as tp, tb_person_info as tpi, tb_person_lastdate as tpl "
        "where tp.id = tpi.id and tpi.id = tpl.id and tpl.last_date = tpi.date and (tp.first_name like '\%%1\%' or tp.last_name like '\%%1\%' or tp.birth like '\%%1\%');"
        ).arg(word);

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return;
    }
// 2018-04-11 JJHWANG Searching Change     if(word != "") {
        if(qry->exec(strQry) == false) {
            qDebug() << __func__<<strQry;
            qDebug() << __func__<<qry->lastError();
            return ;
        }
// 2018-04-11 JJHWANG Searching Change     }

    // TODO: assign lists
    m_patient_model.patient_listup(qry);

    qDebug() << __func__<<"m_patient_model.m_patient_list: " << m_patient_model.m_patient_list;

    int cnt = m_patient_model.m_patient_list.count();

    qDebug() << __func__<< cnt;

    if(cnt == 0)
        emit resSearchPatient(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    else {
        for(int i = 0 ; i < cnt ; i++) {
            patient_element *p = m_patient_model.m_patient_list.at(i);
            qDebug() << " %%%%% SQL : " << i <<". " << p->birth<<", " <<p->gender<<", " <<p->first_name<<", " <<p->last_name<<", " <<p->id<<", "
             <<p->height<<", " <<p->weight;
            emit resSearchPatient(cnt, p->birth.mid(0, 4), p->birth.mid(4, 2), p->birth.mid(6, 2), p->gender, p->first_name, p->last_name, p->id, p->height, p->weight);
        }
    }


    delete qry;
}

void UserIface::db_selected_patient(int row)
{
    if(m_patient_model.m_patient_list.count() < row+1) {
        return;
    }
    patient_element *p = m_patient_model.m_patient_list.at(row);
    qDebug() <<p->birth<<", " <<p->gender<<", " <<p->first_name<<", " <<p->last_name<<", " <<p->id<<", "
         <<p->height<<", " <<p->weight;
    set_patient_id(p->id);
    set_first_name(p->first_name);
    set_last_name(p->last_name);
    set_patient_gender(p->gender);
    set_patient_birth_yy(p->birth.mid(0, 4));
    set_patient_birth_mm(p->birth.mid(4, 2));
    set_patient_birth_dd(p->birth.mid(6, 2));
    set_patient_height(p->height);
    set_patient_weight(p->weight);
// JOON_TEST    gui_message("update_patient");
}


// SKW 2019-01-24 DB Delete // SKW 2019-02-14 drop table(added name option)
void UserIface::db_delete_patient_info(int id, QString firstName, QString lastName)
{
    if(id < 0) {
        qDebug() << __func__<<"Patient id is wrong. "<<id; return;
    }

//    Error:  QSqlError("21", "Unable to execute multiple statements at a time", "not an error")
//    QString strQry1 = QString( "delete from tb_person where id=%1; delete from tb_person_info where id= %1; delete from tb_person_lastdate where id= %1;" )
//            .arg(id);


    QString strQry1 = QString("delete from tb_person where id=%1;" ).arg(id);

    QString strQry2 = QString("delete from tb_person_info where id= %1;" ).arg(id);

    QString strQry3 = QString("delete from tb_person_lastdate where id= %1;" ).arg(id);

    QString strQry4 = QString("drop table if exists tb_patientID_%1_%2_%3;").arg(id).arg(firstName).arg(lastName);   // 2019-02-14 SKW DB drop

    //SKW 2019-04-01 RMS백업 전송을 위한 테이블(tb_patient_total.csv)에서 해당 환자의 시술 기록이 삭제
    QString strQry5 = QString("delete from tb_patient_total where id= %1;" ).arg(id);

    QSqlQuery qry1;
//    QSqlQuery qry2;
//    QSqlQuery qry3;

    qDebug() << __func__<<strQry1;
    if(qry1.exec(strQry1) == false)
    {
        qDebug() << __func__<< " ### SQL 1 : " << strQry1;
        qDebug() << __func__<<" ### SQL 1  Error: " <<qry1.lastError();
        return ;
    }

    qDebug() << __func__<<strQry2;
    if(qry1.exec(strQry2) == false)
    {
        qDebug() << __func__<< " ### SQL 2 : " << strQry2;
        qDebug() << __func__<<" ### SQL 2  Error: " <<qry1.lastError();
        return ;
    }

    qDebug() << __func__<<strQry3;
    if(qry1.exec(strQry3) == false)
    {
        qDebug() << __func__<< " ### SQL 3 : " << strQry3;
        qDebug() << __func__<<" ### SQL 3  Error: " <<qry1.lastError();
        return ;
    }

    qDebug() << __func__<<strQry4;
    if(qry1.exec(strQry4) == false)
    {
        qDebug() << __func__<< " ### SQL 4 : " << strQry4;
        qDebug() << __func__<<" ### SQL 4  Error: " <<qry1.lastError();
        return ;
    }

    //SKW 2019-04-01 RMS백업 전송을 위한 테이블(tb_patient_total.csv)에서 해당 환자의 시술 기록이 삭제
    qDebug() << __func__<<strQry5;
    if(qry1.exec(strQry5) == false)
    {
        qDebug() << __func__<< " ### SQL 5 : " << strQry5;
        qDebug() << __func__<<" ### SQL 5  Error: " <<qry1.lastError();
        return ;
    }

    db_patien_info_init(); // 2019-04-15 선택되어 있는 환자를 Delete했으나 선택이 풀리지 않음 > 삭제 후 전역번수 초기화
}

// SKW 2019-02-12 treatment history
void UserIface::db_save_treatment_info(int id, int pageNo){

    //SKW 2019-04-01 total shot save
    saveCommonSettings();//DB에 저장 시 장비의 토탈 샷도 기록 (샷 수가 0이 아닐 때 DB저장 호출)

//    if(id < 0) {
//qDebug() << __func__<<"Patient id is wrong. "<<id; return;
//    }
//    int facePart = parseInt( get_face() );
    QString face = "";  qDebug() << __func__<<"face "<<face;
    if (get_face() == "1") face = "Head";
    else if (get_face() == "2") face = "Nose";
    else if (get_face() == "3") face = "Neck";
    else if (get_face() == "4") face = "Jawline";
    else if (get_face() == "5") face = "Cheek";
    else if (get_face() == "6") face = "Nasolabial";
    else if (get_face() == "7") face = "Eyes";
    else face = "";//0

    QString usedHpInfo="";
    int m_PowerLevel = mPowerTemp;
    if(pageNo == 1) {
        usedHpInfo = "Plasma";
        m_PowerLevel = m_PowerLevel+1; // SKW 2019-04-04 PS 파워레벨이 내부적으로 0~4 지만 표기는 1~5인 문제 수정
    }
    else if(pageNo == 2)
        usedHpInfo = "Ultrasonic";
    else if(pageNo == 3)
        usedHpInfo = "RM_Auto";
    else if(pageNo == 4)
        usedHpInfo = "RM_Manual";
    else if(pageNo == 5)
        usedHpInfo = "RN";
    else if(pageNo == 6)
        usedHpInfo = "RC";
    else if(pageNo == 7)
        usedHpInfo = "RV";

//    qDebug() << __func__<<"db_save_treatment_info. "<< get_patient_id();
//    qDebug() << __func__<<"db_save_treatment_info. "<< get_first_name();
//    qDebug() << __func__<<"db_save_treatment_info. "<< get_last_name();

    //SKW 2019-03-07 이름이 없는 경우 시술 기록 테이블 생성 x // SKW 2019-04-25 비로그인으로 시술할 경우 Unknown유저로 저장(id: 0번 회원)
    if(get_first_name()=="" && get_last_name()=="")
    {
        set_first_name("Unknown");
        set_last_name("Unknown");
        id = 0;
    }

    QStringList strQryList; strQryList.clear();
    QString idsql = QStringLiteral("%1").arg(id);

       // DEFAULT
       strQryList +=
       "create table if not exists tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name() +""
       "("
       "   id          integer,"
       "   num          integer not null primary key autoincrement,"
       "   first_name        varchar(15) NULL,"
       "   last_name        varchar(15) NULL,"
       "   reg_date    datetime default(datetime('now', 'localtime')), "
       "   hp    varchar(10), "
       "   face    varchar(10), "
       "   rmPinInfo    varchar(10), "
       "   psOpMode    varchar(10), "
       "   power    integer, "
       "   repetition        integer,"
       "   depth        integer,"
       "   ontime      integer," // 1: male 0: female
       "   delay       integer,"
       "   alarm       integer,"
       "   alarm_max       integer,"
       "   usedShot       integer,"
       "   usedTime       integer,"
       "   feedback       integer"
       ");"; // SKW 2019-04-16 num(시술기록번호), feedback 추가
       QSqlQuery qry;   // SKW 2019-02-25 add "usedShotCount    integer"
       foreach(QString strQry, strQryList)
       {
           if(qry.exec(strQry) == false)
           {
               qDebug() << __func__ << "SQL_ERROR: "<<strQry;
               qDebug() << __func__ << "SQL_ERROR: "<<qry.lastError();
           }
       }

    QDateTime dt = QDateTime::currentDateTime();

//    // SKW 2019-02-25 add usedShotCount
//    QString strQry = QString("insert into tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name()
//                             +"(id, first_name, last_name, reg_date, hp, face, rmPinInfo, psOpMode, power, repetition, depth, ontime, delay, alarm, alarm_max, usedShot, usedTime) "
//            "values(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17');")
//            .arg(id).arg(get_first_name()).arg(get_last_name()).arg(dt.toString("yyyy-MM-dd hh:mm:ss")).arg(usedHpInfo).arg(face).arg(m_rmPinInfo).arg(m_psOpMode)
//            .arg(m_power).arg(m_repetition).arg(m_depth).arg(m_ontime)
//            .arg(m_delay).arg(m_alarm).arg(m_alarm_max).arg(m_usedShot).arg(m_usedTime);

    // SKW 2019-04-03 DB저장 시 함수 호출 횟수 줄이기 (Power부터 AlarmMax까지 호출 안함)
    QString strQry = QString("insert into tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name()
                             +"(id, first_name, last_name, reg_date, hp, face, rmPinInfo, psOpMode, power, repetition, depth, ontime, delay, alarm, alarm_max, usedShot, usedTime) "
            "values(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17');")
            .arg(id).arg(get_first_name()).arg(get_last_name()).arg(dt.toString("yyyy-MM-dd hh:mm:ss")).arg(usedHpInfo).arg(face).arg(m_rmPinInfo).arg(m_psOpMode)
            .arg(m_PowerLevel).arg(mRepetition).arg(mSpacingRfpowerDepth).arg(mLengthRfOntime).arg(mTreatDelayTime).arg(mAlarmCount).arg(mAlarmCountMax)
            .arg(m_usedShot).arg(m_usedTime);

    QSqlQuery qry2;
    qDebug() << __func__<<strQry;
    if(qry2.exec(strQry) == false)
    {
        qDebug() << __func__<< " ### SQL: " << strQry;
        qDebug() << __func__<<" ### SQL Error: " <<qry2.lastError();
        return ;
    }

    //SKW 2019-03-07 tb_patient_total 모든 시술을 한 곳에 기록하는 테이블 생성 및 저장
    strQryList +=
    "create table if not exists tb_patient_total "
    "("
    "   id          integer,"
    "   num          integer not null,"
    "   first_name        varchar(15) NULL,"
    "   last_name        varchar(15) NULL,"
    "   reg_date    datetime default(datetime('now', 'localtime')), "
    "   hp    varchar(10), "
    "   face    varchar(10), "
    "   rmPinInfo    varchar(10), "
    "   psOpMode    varchar(10), "
    "   power    integer, "
    "   repetition        integer,"
    "   depth        integer,"
    "   ontime      integer," // 1: male 0: female
    "   delay       integer,"
    "   alarm       integer,"
    "   alarm_max       integer,"
    "   usedShot       integer,"
    "   usedTime       integer,"
    "   feedback       integer"
    ");"; // SKW 2019-04-16 num(시술기록번호), feedback 추가

    foreach(QString strQry, strQryList)
    {
        if(qry.exec(strQry) == false)
        {
            qDebug() << __func__ << "SQL_ERROR: "<<strQry;
            qDebug() << __func__ << "SQL_ERROR: "<<qry.lastError();
        }
    }

    //SKW 2019-04-16
    //개인 기록 테이블에서 자동으로 증가하는 num값도 함께 가져오기 위해 select해서 insert
    //num값은 추후 id와 num값을 이용하여 feedback(만족도) 점수 업데이트에 사용함.
    //위에서 사용한 개인 기록 테이블(tb_patientId_'id'_'first'_'last')에서 가장 큰 num값을 가진 기록을 입력.
    //가장 큰 num값은 autoincrement로 인하여 가장 마지막에 들어간 값을 의미
    // 요약 => 가장 마지막에 입력된 값을 total 테이블에도 입력
    QString strQry2 = QString("insert into tb_patient_total select * from tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name() +
                              " where num = (select max(num) from tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name() + ");");

    qDebug() << __func__<<strQry2;
    if(qry2.exec(strQry2) == false)
    {
        qDebug() << __func__<< " ### SQL: " << strQry2;
        qDebug() << __func__<<" ### SQL Error: " <<qry2.lastError();
        return ;
    }

    QString strQry3 = QString("insert into tb_person_lastdate(id, last_date) "
            "values(%1, '%2');")
            .arg(id)
            .arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
//    QSqlQuery qry3;
    qDebug() << __func__<<strQry3;
    if(qry2.exec(strQry3) == false)
    {
        qDebug() << __func__<<strQry3;
        qDebug() << __func__<<qry2.lastError();
        return ;
    }


// System Log
    // SKW 2019-05-02 //장비가 켜진 시간, 시스템 로그에 기록 대기
    struct tm turn_on;
    turn_on.tm_year = (mDateYear + 2000) - 1900;
    turn_on.tm_mon = (mDateMon - 1);
    turn_on.tm_mday = mDateDay;
    turn_on.tm_hour = mDateHour;
    turn_on.tm_min = mDateMin;
    turn_on.tm_sec = mDateSec;
    char operTimeString[128] = {0};
    strftime(operTimeString, 80, "%Y-%m-%d %H:%M:%S", &turn_on);
    qDebug() << __func__ << "SKW 2019-05-02 Test: turn_on: "<< operTimeString;
//    qDebug() << __func__ << "SKW 2019-05-02 Test: turn_on: "<< operTimeString.toString("yyyy-MM-dd hh:mm:ss");


    // SKW 2019-05-02 //에러코드 반환, 시스템 로그에 기록 대기
    QString errorCode = getLoggingErrCode(malarmCode);
    qDebug() << __func__ << "SKW 2019-05-02 Test: errorCode: "<< errorCode;

    // SKW 2019-05-02 System Log 기록하는 테이블 생성 및 저장
    strQryList +=
    "create table if not exists tb_system_log "
    "("
    "   id          integer,"
    "   num          integer not null,"
    "   first_name        varchar(15) NULL,"
    "   last_name        varchar(15) NULL,"
    "   reg_date    datetime default(datetime('now', 'localtime')), "
    "   hp    varchar(10), "
    "   face    varchar(10), "
    "   rmPinInfo    varchar(10), "
    "   psOpMode    varchar(10), "
    "   power    integer, "
    "   repetition        integer,"
    "   depth        integer,"
    "   ontime      integer," // 1: male 0: female
    "   delay       integer,"
    "   alarm       integer,"
    "   alarm_max       integer,"
    "   usedShot       integer,"
    "   usedTime       integer,"
    "   feedback       integer,"
    "   device_turn_on       datetime,"
    "   errorCode       varchar(20)"
    ");"; // SKW 2019-04-16 num(시술기록번호), feedback 추가

    foreach(QString strQry, strQryList)
    {
        if(qry.exec(strQry) == false)
        {
            qDebug() << __func__ << "SQL_ERROR: "<<strQry;
            qDebug() << __func__ << "SQL_ERROR: "<<qry.lastError();
        }
    }

    // 21개 컬럼 중 19개 동일한 컬럼 입력 //한 번에 19개 읽기 + 2개 추가입력이 컬럼 갯수 차이로 인해 안되어, 19개 읽고, 2개는 업데이트방식으로 진행
    strQry = QString("INSERT INTO tb_system_log (id, num, first_name, last_name, reg_date, hp, face, rmPinInfo, psOpMode, power, repetition, depth, ontime, delay, alarm, alarm_max, usedShot, usedTime, feedback) "
                     "SELECT id, num, first_name, last_name, reg_date, hp, face, rmPinInfo, psOpMode, power, repetition, depth, ontime, delay, alarm, alarm_max, usedShot, usedTime, feedback "
                     "FROM tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name() + " "
                     "WHERE num = (SELECT max(num) FROM tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name() +
                     ");");

            //strQry = QString("update tb_patient_total set feedback = '" + QStringLiteral("%1").arg(feedbackValue) + "' where id = '" + id + "' and num = '" + QStringLiteral("%1").arg(num) + "';");
    qDebug() << __func__<<strQry;
    if(qry2.exec(strQry) == false)
    {
        qDebug() << __func__<< " ### SQL: " << strQry;
        qDebug() << __func__<<" ### SQL Error: " <<qry2.lastError();
        return ;
    }

    // 컬럼 갯수가 다른 부분은 추가 업데이트
    strQry = QString("UPDATE tb_system_log "
                     "SET device_turn_on = '"+ QStringLiteral("%1").arg(operTimeString) +"', errorCode = '"+ QStringLiteral("%2").arg(errorCode) +"' "
                     "WHERE id = '" + idsql + "' and num = (SELECT max(num) FROM tb_patientId_"+ idsql +"_"+ get_first_name() +"_"+ get_last_name() +
                     ");");


    qDebug() << __func__<<strQry;
    if(qry2.exec(strQry) == false)
    {
        qDebug() << __func__<< " ### SQL: " << strQry;
        qDebug() << __func__<<" ### SQL Error: " <<qry2.lastError();
        return ;
    }


}

//SKW 2019-05-07 주요 목적: 소프트웨어에서 펌웨어 연결 여부를 확인 후 에러코드 기록
// 펌웨어에서 RTC 시간을 받지 못하면 장비가 켜진 시간이 2000-00-00 00:00:00 으로 표기됨
void UserIface::db_save_errorCode_info(int err){

    // 장비가 켜진 시간
    struct tm turn_on;
    turn_on.tm_year = (mDateYear + 2000) - 1900;
    turn_on.tm_mon = (mDateMon - 1);
    turn_on.tm_mday = mDateDay;
    turn_on.tm_hour = mDateHour;
    turn_on.tm_min = mDateMin;
    turn_on.tm_sec = mDateSec;
    char operTimeString[128] = {0};
    strftime(operTimeString, 80, "%Y-%m-%d %H:%M:%S", &turn_on);
    qDebug() << __func__ << " turn_on: "<< operTimeString;
//    qDebug() << __func__ << " turn_on: "<< operTimeString.toString("yyyy-MM-dd hh:mm:ss");

    //에러코드 반환, 시스템 로그에 기록 대기
    QString errorCode = getLoggingErrCode(err);
    qDebug() << __func__ << "SKW 2019-05-02 Test: errorCode: "<< errorCode;

    QStringList strQryList;
    strQryList.clear();

    strQryList +=
    "create table if not exists tb_system_log "
    "("
    "   id          integer,"
    "   num          integer not null,"
    "   first_name        varchar(15) NULL,"
    "   last_name        varchar(15) NULL,"
    "   reg_date    datetime default(datetime('now', 'localtime')), "
    "   hp    varchar(10), "
    "   face    varchar(10), "
    "   rmPinInfo    varchar(10), "
    "   psOpMode    varchar(10), "
    "   power    integer, "
    "   repetition        integer,"
    "   depth        integer,"
    "   ontime      integer," // 1: male 0: female
    "   delay       integer,"
    "   alarm       integer,"
    "   alarm_max       integer,"
    "   usedShot       integer,"
    "   usedTime       integer,"
    "   feedback       integer,"
    "   device_turn_on       datetime,"
    "   errorCode       varchar(20)"
    ");"; // SKW 2019-04-16 num(시술기록번호), feedback 추가

    QSqlQuery qry;
    foreach(QString strQry, strQryList)
    {
        if(qry.exec(strQry) == false)
        {
            qDebug() << __func__ << "SQL_ERROR: "<<strQry;
            qDebug() << __func__ << "SQL_ERROR: "<<qry.lastError();
        }
    }

    // 에러코드 기록 시 num은 -1로 기록
    // num은 시술만족도 업데이트를 위해 각 환자별 시술순서를 기록한 번호
    QString strQry = QString("INSERT INTO tb_system_log (num, device_turn_on, errorCode) "
                             "VALUES (-1, '"+ QStringLiteral("%1").arg(operTimeString) +"', '"+ QStringLiteral("%2").arg(errorCode) +"' "
                             ");");

    QSqlQuery qry2;
    qDebug() << __func__<<strQry;
    if(qry2.exec(strQry) == false)
    {
        qDebug() << __func__<< " ### SQL: " << strQry;
        qDebug() << __func__<<" ### SQL Error: " <<qry2.lastError();
        return ;
    }
}

// SKW 2019-02-13 treatment history, 시술 기록 전체 //SKW 2019-04-18 SELECT문 개선
void UserIface::db_select_treatmentHistory(QString selected_hp) {
    QString strQry =  "select * from tb_patientID_" +QStringLiteral("%1").arg(get_patient_id())+ "_" +get_first_name()+ "_" +get_last_name()+" where " + selected_hp + ";"; //SKW 2019-04-18 SELECT문 개선

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return;
    }
    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        qDebug() << __func__<<get_hp();

        // 환자 정보 생성 후, 읽어올 기록이 없는 경우, HP정보 초기화(LOAD DATA클릭 시 페이지 랜덤 이동 문제)
        emit reSelectTreatmentHistory(0, 0, 0, "0", "0", "0",
                                      "0", "0", "0", "0", 0,
                                      0, 0, 0, 0, 0,
                                      0, 0, 0, 0);  // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
        set_hp("");
        return ;
    }

    m_patient_model.patient_listup(qry);

    qDebug() << __func__<<"m_patient_model.m_patient_list: " << m_patient_model.m_patient_list;

    int cnt = m_patient_model.m_patient_list.count();

    qDebug() << __func__<< cnt;

// SKW 2019-02-13 treatment history
    if(cnt == 0) {
        //QList<patient_element *> queryResult;
        //queryResult.clear();
        emit reSelectTreatmentHistory(0, 0, 0, "0", "0", "0",
                                      "0", "0", "0", "0", 0,
                                      0, 0, 0, 0, 0,
                                      0, 0, 0, 0);  // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
                }
    else {
        //QList<patient_element *> queryResult;
        //queryResult.clear();
        for(int i = 0 ; i < cnt ; i++) {
            patient_element *p = m_patient_model.m_patient_list.at(i);
            qDebug() << " selectTreatmentHistory %%%%% SQL : " << i <<". " << p->reg_date<<", " <<p->hp<<", " <<p->rmPinInfo <<", " <<p->feedback;
            //queryResult << m_patient_model.m_patient_list.at(i);

            // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
            emit reSelectTreatmentHistory(cnt, p->id, p->num, p->first_name, p->last_name, p->reg_date,
                                          p->hp, p->face, p->rmPinInfo, p->psOpMode, p->power,
                                          p->repetition, p->depth, p->ontime, p->delay, p->alarm,
                                          p->alarm_max, p->usedShot, p->usedTime, p->feedback);


            //UserIface.setOptions();

        }
    }
    delete qry;
}


void UserIface::db_setting_treatmentHistory(int currIndex){ //currIndex : 시술 히스토리 리스트뷰의 인덱스 번호
    QString strQry =  "select * from tb_patientID_" +QStringLiteral("%1").arg(get_patient_id())+ "_" +get_first_name()+ "_" +get_last_name()+";";

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return;
    }
    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        qDebug() << __func__<<get_hp();

        // 환자 정보 생성 후, 읽어올 기록이 없는 경우, HP정보 초기화(LOAD DATA클릭 시 페이지 랜덤 이동 문제)
        set_hp("");
        return ;
    }

    m_patient_model.patient_listup(qry);

    qDebug() << __func__<<"m_patient_model.m_patient_list: " << m_patient_model.m_patient_list;

    int cnt = m_patient_model.m_patient_list.count();

    qDebug() << __func__<< cnt;

// SKW 2019-02-13 treatment history
    if(cnt == 0) {
        //QList<patient_element *> queryResult;
        //queryResult.clear();
        emit reSettingTreatmentHistory(0, 0, 0, "0", "0", "0",
                                       "0", "0", "0", "0", 0,
                                       0, 0, 0, 0, 0,
                                       0, 0, 0, 0);  // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
    }
    else {
        patient_element *p = m_patient_model.m_patient_list.at(currIndex);
        qDebug() << " db_setting_treatmentHistory %%%%% SQL : " << currIndex <<". " << p->reg_date<<", " <<p->hp<<", " <<p->rmPinInfo;

        emit reSettingTreatmentHistory(cnt, p->id, p->num, p->first_name, p->last_name, p->reg_date,
                                       p->hp, p->face, p->rmPinInfo, p->psOpMode, p->power,
                                       p->repetition, p->depth, p->ontime, p->delay, p->alarm,
                                       p->alarm_max, p->usedShot, p->usedTime, p->feedback);  // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
    }
    delete qry;
}


//SKW 2019-02-19 DB table backup to csv
void UserIface::db_backup_to_CSV(){

    int db_cnt = 0;
    QList<QString> qryList;    qryList.clear(); //s.toList();

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return;
    }

    QString strQry =  "SELECT name FROM sqlite_master WHERE type='table';";

    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
//        qDebug() << __func__<<qry->size();
        qDebug() << __func__<<qry->lastError();

        // 환자 정보 생성 후, 읽어올 기록이 없는 경우, HP정보 초기화(LOAD DATA클릭 시 페이지 랜덤 이동 문제)
        set_hp("");
        return ;
    }else{
        qDebug() << __func__<<strQry;

        while( qry->next() ){
            db_cnt++;
            qryList.append( qry->value(0).toString() );
            qDebug() << __func__<<qry->value(0).toString();
        }
        qDebug() << __func__<<db_cnt;

    }
//READ

//SAVE
//************************************//
    //sqlite3 설치방법
    //1. sudo apt update
    //2. sudo apt-get install sqlite3
//************************************//    //home/pi/hironic/log/  /home/app/data/
    for (int i=0; i<db_cnt; i++) {
        //SKW 2019-03-07 이전 방식 참고용
//        QString str = QString("sqlite3 -header -csv /home/app/data/person.db \"select * from "+ qryList[i] +";\" > //home/pi/hironic/log/"+ qryList[i] +".csv");
//        QProcess myProcess;
//        myProcess.setStandardOutputFile("//home/pi/hironic/log/" +qryList[i] +".csv");
//        myProcess.start(str);
//        myProcess.waitForFinished();
//        qDebug() << __func__<< str;      //db_backup_to_CSV

        //SKW 2019-03-07 기존의 로그 파일에 새 로그를 쓰기를 위한 권한 변경, SQLite to file(csv) save
        QProcess::execute("/home/app/script/sqliteToCsv.sh "+qryList[i]);
    }//SAVE
    delete qry;

//************************************************************************//
//    AWS S3연결을 위한 awscli(aws의 서비스 api를 호출할 수 있는 툴) 설치방법
//    1. sudo apt update
//    2. sudo apt-get install python3
//    3. wget https://bootstrap.pypa.io/get-pip.py
//    4. sudo python get-pip.py
//    5. sudo pip install awscli
//    6. sudo aws configure
//----------------------------------------------------------------------
//    * AWS client 설정 입력 방법(configure)
//    - AWS Access Key ID [None]: AKIAJBB5IPHWIJFGXZLA
//    - AWS Secret Access Key [None]: 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
//    - Default region name [None]: ap-northeast-2
//    - Default output format [None]: json
//----------------------------------------------------------------------
//    7. sudo aws s3 ls //접속 확인 : 버킷의 목록 확인
//    8. sudo aws s3 rm s3://hironicstor/LOG/GENTLO/HGENTLO001_20190405.csv //삭제하는 방법
//************************************************************************//
    //2019-04-08 upload to AWS S3
//    QProcess::execute("sudo aws s3 cp /home/pi/hironic/log/tb_patient_total.csv s3://hironicstor/LOG/GENTLO/HGENTLO001_20190405.csv");

}

//2019-04-08 AWS S3에 로그를 보냈는지 장비에서 알 수 있는 방법 추가
bool UserIface::db_awsUpload_check(QString date){
    QString cmd;
    QProcess process;

    cmd.append("sudo aws s3 cp");
    cmd.append(" ");
    cmd.append(DB_FILE_DIR);
    cmd.append(DB_FILE_NAME);
    cmd.append(" ");
    cmd.append(S3_FILE_DIR);
    cmd.append(mSerialNumber);
    cmd.append("_");
    cmd.append(date);
    cmd.append(".csv");

    qDebug() << "cmd path : " << cmd;


    process.start(cmd);

    process.waitForFinished();
    QString output(process.readAllStandardOutput());

    if(output.contains("Completed")){
        qDebug() << "aws s3 sendCheck : Completed";
        return true;
    } else {
        qDebug() << "aws s3 sendCheck : Upload Failed" << output;
        return false;
    }
}

// SKW 2019-02-22 read patient treatment history from db, 특정 인물의 기록
void UserIface::db_read_detailHistory(int id, QString firstName, QString lastName, QString date) {
    QString strQry =  "select * from tb_patientID_" +QStringLiteral("%1").arg(id)+ "_" +firstName+ "_" +lastName+ " where reg_date = '"+date+"';";

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return;
    }
    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        qDebug() << __func__<<get_hp();

        // 환자 정보 생성 후, 읽어올 기록이 없는 경우, HP정보 초기화(LOAD DATA클릭 시 페이지 랜덤 이동 문제)
        set_hp("");
        return ;
    }

    m_patient_model.patient_listup(qry);

    qDebug() << __func__<<"m_patient_model.m_patient_list: " << m_patient_model.m_patient_list;

    int cnt = m_patient_model.m_patient_list.count();

    qDebug() << __func__<< cnt;

    if(cnt == 0){       // (cnt != 1)
        emit reDetailTreatmentHistory(0, 0, 0, "0", "0", "0",
                                      "0", "0", "0", "0", 0,
                                      0, 0, 0, 0, 0,
                                      0, 0, 0, 0); // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가
    }else{

        for(int i = 0 ; i < cnt ; i++) {
            patient_element *p = m_patient_model.m_patient_list.at(i);
            qDebug() << "detail history : " << i <<". " << p->id <<", " << p->first_name<<", " << p->last_name
                    << ", " << p->hp << ", " << p->face << ", " << p->rmPinInfo << ", " << p->psOpMode << ", " << p->power
                    << ", " << p->repetition << ", " << p->depth << ", " << p->ontime << ", " << p->delay << ", " << p->alarm
                    << ", " << p->alarm_max << ", " << p->usedShot << ", " << p->usedTime; // SKW 2019-02-25 add usedShotCount

            emit reDetailTreatmentHistory(cnt, p->id, p->num, p->first_name, p->last_name, p->reg_date,
                                          p->hp, p->face, p->rmPinInfo, p->psOpMode, p->power,
                                          p->repetition, p->depth, p->ontime, p->delay, p->alarm,
                                          p->alarm_max, p->usedShot, p->usedTime, p->feedback); // SKW 2019-02-25 add usedShotCount // SKW 2019-04-16 num(시술기록번호), feedback 추가

        }
    }
    delete qry;
}

// SKW 2019-02-26
bool UserIface::db_check_id(int id, QString firstName, QString lastName) {
    QString strQry =  "select id from tb_person where id = '" +QStringLiteral("%1").arg(id)+ "' and first_name = '" + firstName + "' and last_name = '" + lastName + "';";
    qDebug() << __func__<<strQry;

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
//        return;
    }
    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        qDebug() << __func__<<get_hp();

        // 환자 정보 생성 후, 읽어올 기록이 없는 경우, HP정보 초기화(LOAD DATA클릭 시 페이지 랜덤 이동 문제)
        set_hp("");
    }

    m_patient_model.patient_listup(qry);

    qDebug() << __func__<<"m_patient_model.m_patient_list: " << m_patient_model.m_patient_list;

    int cnt = m_patient_model.m_patient_list.count();

    qDebug() << __func__<< cnt;

    if(cnt == 0)return false;   //no data
    else return true;   //id exist
}

//SKW 2019-04-09 환자정보 전역변수 초기화 (로그인 해제 시 사용)
void UserIface::db_patien_info_init(){
    set_patient_id(-1);
    set_first_name("");
    set_last_name("");
    set_patient_birth_yy("");
    set_patient_birth_mm("");
    set_patient_birth_dd("");
    set_patient_weight("");//function setBmi()
    set_patient_height("");//function setBmi()
    set_patient_gender(0);
}

// SKW 2019-03-22 이름이 동일하면 성별, 이름 성씨, 생년월일이 달라도 동일인물로 여겨 저장 안되는 부분. 중복검사 재구성
void UserIface::db_checkAndCreate(QString firstName, QString lastName, QString year, QString month, QString day, int gender)
{

    if(month.toInt() < 10 && month.length() < 2) {
        month.insert(0, QChar('0'));
    }
    if(day.toInt() < 10 && day.length() < 2) {
        day.insert(0, QChar('0'));
    }
    QString birth = year + month + day;

    qDebug() << __func__<< firstName << lastName << birth << gender;
    QString strQry = QString(
        "select * from tb_person where first_name like '%1' and last_name like '%2' and birth like '%3' and gender like '%4';"
    ).arg(firstName).arg(lastName).arg(birth).arg(gender);

    qDebug() << __func__<<strQry;

//    first_name        varchar(15) NULL,"
//        "   last_name        varchar(15) NULL,"
//        "   gender      int         NULL," // 1: male 0: female
//        "   birth       varchar(6)  NULL"
////    QString strQry = QString(
////        "select tp.gender, tp.reg_date, tp.last_date, tp.id as id, tp.name as name, tp.birth as birth, tpi.height as height, tpi.weight as weight from tb_person as tp, tb_person_info as tpi "
////        "where tp.id = tpi.id and tp.id = '\%%1\%' order by tpi.no desc limit 1;"
////    ).arg(id);
//    QString strQry = QString(
////        "select tp.gender, tp.reg_date, tp.last_date, tp.id as id, tp.name as name, tp.birth as birth, tpi.height as height, tpi.weight as weight from tb_person as tp, tb_person_info as tpi, tb_person_lastdate as tpl"
////        "where tp.id = tpi.id and tpi.id = tpl.id and tpl.last_date = tpi.date and (tp.name like '\%%1\%' or tp.birth like '\%%1\%');"
//        "select tp.gender, tp.id as id, tp.first_name as first_name, tp.last_name as last_name, tp.birth as birth, tpi.height as height, tpi.weight as weight from tb_person as tp, tb_person_info as tpi, tb_person_lastdate as tpl "
////        "where tp.id = tpi.id and tpi.id = tpl.id and tpl.last_date = tpi.date and (tp.first_name like '\%%1\%' or tp.birth like '\%%1\%');"
//        "where tp.id = tpi.id and tpi.id = tpl.id and tpl.last_date = tpi.date and (tp.first_name like '\%%1\%' or tp.last_name like '\%%1\%' or tp.birth like '\%%1\%');"
//        ).arg(word);

    QSqlQuery *qry = new QSqlQuery();
    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return;
    }

    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        return ;
    }

    m_patient_model.patient_listup(qry);

    qDebug() << __func__<<"m_patient_model.m_patient_list: " << m_patient_model.m_patient_list;

    int cnt = m_patient_model.m_patient_list.count();

    qDebug() << __func__<< cnt;

    if(cnt == 0)
        emit resSearchPatient(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    else {
        for(int i = 0 ; i < cnt ; i++) {
            patient_element *p = m_patient_model.m_patient_list.at(i);
            qDebug() << " %%%%% SQL : " << i <<". " << p->birth<<", " <<p->gender<<", " <<p->first_name<<", " <<p->last_name<<", " <<p->id<<", "
                     <<p->height<<", " <<p->weight;
            emit resSearchPatient(cnt, p->birth.mid(0, 4), p->birth.mid(4, 2), p->birth.mid(6, 2), p->gender, p->first_name, p->last_name, p->id, p->height, p->weight);
        }
    }


    delete qry;
}


// SKW 2019-04-15 만족도 점수 업데이트
void UserIface::db_update_feedback(QString id, int num, int feedbackValue) {
    //호출 시 전역번수 id, firstname, lastname 업데이트 필요 //매개변수: 유저 고유 아이디, 변경할 줄 번호(시술 기록), 만족도 점수
    QString strQry = QString("update tb_patientId_"+ id +"_"+ get_first_name() +"_"+ get_last_name() + " set feedback = '" + QStringLiteral("%1").arg(feedbackValue) + "' where num = '" + QStringLiteral("%1").arg(num) + "';");

    QSqlQuery qry;
    qDebug() << strQry;

    if(qry.exec(strQry) == false)
    {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry.lastError();
        return ;
    }

    //SKW 2019-04-16 tb_patient_total update
    strQry = QString("update tb_patient_total set feedback = '" + QStringLiteral("%1").arg(feedbackValue) + "' where id = '" + id + "' and num = '" + QStringLiteral("%1").arg(num) + "';");

    qDebug() << strQry;

    if(qry.exec(strQry) == false)
    {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry.lastError();
        return ;
    }

    //SKW 2019-05-03 tb_system_log update
    strQry = QString("update tb_system_log set feedback = '" + QStringLiteral("%1").arg(feedbackValue) + "' where id = '" + id + "' and num = '" + QStringLiteral("%1").arg(num) + "';");

    qDebug() << strQry;

    if(qry.exec(strQry) == false)
    {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry.lastError();
        return ;
    }
}

//SKW 2019-04-22 총 방문 횟수를 전달 (TreatmentHistory.qml > visitCount)
int UserIface::db_select_treatmentHistory_visitCount() {
    QString strQry =  "select * from tb_patientID_" +QStringLiteral("%1").arg(get_patient_id())+ "_" +get_first_name()+ "_" +get_last_name() + ";"; //SKW 2019-04-18 SELECT문 개선

    QSqlQuery *qry = new QSqlQuery();

    if(qry == NULL) {
        qDebug() << __func__<<"malloc() error!!";
        return 0;
    }
    if(qry->exec(strQry) == false) {
        qDebug() << __func__<<strQry;
        qDebug() << __func__<<qry->lastError();
        qDebug() << __func__<<get_hp();

        return 0;
    }

    m_patient_model.patient_listup(qry);

    qDebug() << __func__<<"m_patient_model.m_patient_list: " << m_patient_model.m_patient_list;

    int cnt = m_patient_model.m_patient_list.count();

    qDebug() << __func__<< cnt;

    return cnt; //총 방문 횟수를 전달

    delete qry;
}

#endif
