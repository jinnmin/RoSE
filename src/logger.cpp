#include "logger.h"

Logger::Logger(QObject *parent) :
    QObject(parent),
    fsWatcher(NULL)
{
    //detectDev();
}

Logger::~Logger()
{

}

// unused now
void Logger::DetectedDev(QString dev)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << dev;
#endif
    QDir directory(dev);
    QStringList FilesList = directory.entryList(QDir::AllDirs |  QDir::NoDotAndDotDot);
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << FilesList;
    qDebug() << __func__ << FilesList.size();

    for (int loop = 0; loop < FilesList.size(); loop++) {
        qDebug() << __func__ << FilesList[loop];
    }
#endif
}

// unused now
void Logger::detectDev()
{
     qDebug() << __func__;
     fsWatcher = new QFileSystemWatcher(this);
     QString path = USB_PATH;
     fsWatcher->addPath(path);
     connect(fsWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(DetectedDev(QString)));
}

void Logger::getLogData()
{
    int ret = FAIL;

    ret = copyFiles();

    emit workFinished(((ret == SUCCESS)? true : false), ret);
}

// unused now
void Logger::getFileList(QString path)
{
    QStringList strFilters;

    strFilters += "log_*.csv";
    QDirIterator iterDir(path, strFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    while (iterDir.hasNext())
    {
#if defined(KZ_UI_DEBUG)
        qDebug() << "next : " << iterDir.next();
        qDebug() << __func__ << iterDir.fileName();
#endif

        // 해당 파일의 md5sum 계산.
        this->mMd5hash = getFileMd5Sum(iterDir.filePath()).toHex();

        // usb에 파일 복사.
        //ret = copyFiles(iterDir.filePath(), iterDir.fileName());
        //if (ret < SUCCESS) return ret;
    }
}

QString Logger::getDirList(QString dirPath)
{
    QString dirName(QString::null);
    QString insideDirPath(QString::null);

    QDir directory(dirPath);
    QStringList dirList = directory.entryList(QDir::AllDirs |  QDir::NoDotAndDotDot);

    for (int i = 0; i < dirList.size(); i++) {
#if defined(KZ_UI_DEBUG)
        qDebug() << __func__ << dirList.at(i);
#endif
        if (HIRONIC == dirList.at(i)) {
#if defined(KZ_UI_DEBUG)
            qDebug() << __func__ << "found hironic!";
#endif
            dirName = dirList.at(i);
            mInsideDirPath = dirPath;
            break;
        }

        insideDirPath.append(dirPath);
        insideDirPath.append("/");
        insideDirPath.append(dirList.at(i));
        qDebug() << __func__ << "inside dir path : " << mInsideDirPath;

        // recursive
        dirName = getDirList(insideDirPath);
        qDebug() << __func__ << "inside dir name :" << dirName;
        insideDirPath.clear();

        if (HIRONIC == dirName) break;
    }

    return dirName;
}

int Logger::copyFiles()
{
    int ret = -1;
    QString cmd, usb, dirName;

    dirName = getDirList(USB_PATH);
    if (NULL == dirName) {
        qDebug() << "dir name" << dirName;
        return ERR_DIR_NOT_FOUND;
    }

    usb.append(mInsideDirPath);
    usb.append("/");
    usb.append(dirName);
    usb.append("/");

    checkUsbDIR(usb);

    // copy
    cmd.append("cp -rf");
    cmd.append(" ");
    cmd.append(TARGET_PATH);
    cmd.append(LOG);
    cmd.append(" ");
    cmd.append(usb);
    qDebug() << "cmd path : " << cmd;
    ret = system(cmd.toStdString().c_str());
    qDebug() << "cmd result : " << ret;

    cmd.clear();
    cmd.append("sync");
    system(cmd.toStdString().c_str());  // sync
    system(cmd.toStdString().c_str());  // sync


    // copy comand return values :: (0) -> success, (0 < result) -> fail.
    if (ret == 0) {
        qDebug() << "file copy has completed";
        QProcess::execute("/home/app/script/change_plasonic_org.sh");
        return SUCCESS;
    }
    else {
        qDebug() << "file copy has failed";
        return FAIL;
    }
}

void Logger::checkUsbDIR(QString usbLogPath)
{
    QDir usbDir(usbLogPath);

    if (!usbDir.exists()) {
        qDebug() << __func__ << "usb log directory is not exist!";
        usbDir.mkpath(usbLogPath);
        qDebug() << __func__ << "usb log directory is created!";
    }
    else {
        qDebug() << __func__ << "usb log directory is alreay exist!";
    }
}

void Logger::checkTargetDIR()
{
    QDir targetDir(TARGET_LOG_PATH);

    if (!targetDir.exists()) {
        qDebug() << __func__ << "target log directory is not exist!";
        targetDir.mkpath(TARGET_LOG_PATH);
        qDebug() << __func__ << "target log directory is created!";
    }
    else {
        qDebug() << __func__ << "target log directory is alreay exist!";
    }
}

QByteArray Logger::getFileMd5Sum(QString filepath)
{
    QByteArray md5hash;

    QFile file(filepath);
    file.open(QIODevice::ReadOnly);

    if (file.isOpen()) {
        QCryptographicHash md5gen(QCryptographicHash::Md5);

        md5gen.addData(file.readAll());
        md5hash = md5gen.result();

        qDebug() << __func__ << md5hash.toHex();
        file.close();
    }
    else {
        qDebug() << __func__ << "failed to open file" << filepath;
    }

    return md5hash;
}

void Logger::log_print(int hpMode, struct tm turn_on, QString stdbyReadyMode, QString operMode, int level, struct tm start_treat,
                      struct tm end_treat, struct tm tip_info, int tip_serial, int used_tip_cnt, struct tm tipTotaltimeInfo, QString errCode)
{
    ofstream log;
    QString cmd, item;
    struct tm *getCurDateTime;
    int dateTimeMon;

    checkTargetDIR();

    getCurDateTime = getCurrentTime();

    dateTimeMon = getCurDateTime->tm_mon + 1;

    mDateTime.append(mDateTime.number(getCurDateTime->tm_year+1900));
    if ( 10 > dateTimeMon) {    // 1월 ~ 9월 일 때.
        mDateTime.append(mDateTime.number(0));
    }
    mDateTime.append(mDateTime.number(dateTimeMon));
    // mDateTime.append(mDateTime.number(getCurDateTime->tm_mday));

    cmd.append(TARGET_LOG_PATH);
    cmd.append("Gentlo");
    cmd.append(mDateTime);
    cmd.append("log.csv");

    char buff[2048] = {0};
    memset(buff, 0, sizeof(buff));

    qDebug() << "1. open :" << cmd.toStdString().c_str();

    log.open(cmd.toStdString().c_str(), ios::app);

    char operTimeString[128] = {0};
    char startTMTimeString[128] = {0};
    char endTMTimeString[128] = {0};
    char tipReleaseInfo[32] = {0};
    char tipRemainingTime[32] = {0};
    char tipTotalTime[32] = {0};
    char LogBuf[128] = {0};
    char itemStrBuf[512] = {0};

    item.append("Mode");
    item.append(",Device Turn On");
    item.append(",R/S");
    item.append(",Frequency");
    item.append(",Level");
    item.append(",Start Treatment Time");
    item.append(",End Treatment Time");
    item.append(",Tip Info");
    item.append(",Tip Remaining Time");
    item.append(",Used Tip Count");
    item.append(",Used Tip Time");
    item.append(",Error Code\n");

    strftime(operTimeString, 80, "%Y-%m-%d %H:%M", &turn_on);
    strftime(startTMTimeString, 80, ",%Y-%m-%d %H:%M", &start_treat);
    strftime(endTMTimeString, 80, ",%Y-%m-%d %H:%M", &end_treat);
    strftime(tipReleaseInfo, 30, ",PST%Y%m%d", &tip_info);
    strftime(tipRemainingTime, 30, ",%H:%M:%S", &tip_info);
    strftime(tipTotalTime, 30, ",%H:%M:%S", &tipTotaltimeInfo);

    // item
    sprintf(itemStrBuf, "%s", item.toStdString().c_str());
    strcat(buff, itemStrBuf);

     qDebug() << __func__ << " Target Handpiece  hpMode:" << hex << hpMode;

    // hp mode
    if (hpMode == DF_BD) {
        sprintf(LogBuf, "%s,", "Alexandrite");
        strcat(buff, LogBuf);
    }
    else if (hpMode == HP_NDYAG) {
        sprintf(LogBuf, "%s,", "Ndyag");
        strcat(buff, LogBuf);
    }

    // date and time
    strcat(buff, operTimeString);
    // operation mode, level
    sprintf(LogBuf, ",%s,%s,%d", stdbyReadyMode.toStdString().c_str(), operMode.toStdString().c_str(), level);
    strcat(buff, LogBuf);
    // start treatment time
    strcat(buff, startTMTimeString);
    // end treatment time
    strcat(buff, endTMTimeString);

    switch (hpMode) {
        case DF_BD:
            // Tip Information
            strcat(buff, tipReleaseInfo);
            // Tip Serial No
            sprintf(LogBuf, "%d", tip_serial);
            strcat(buff, LogBuf);
            // Tip Remaining Time
            strcat(buff, tipRemainingTime);
            // Tip Used Count
            sprintf(LogBuf, ",%d", used_tip_cnt);
            strcat(buff, LogBuf);
            // Tip Total Time
            strcat(buff, tipTotalTime);
            break;

        default:
            break;
    }

    // Tip Error Code
    sprintf(LogBuf, ",%s\n", errCode.toStdString().c_str());
    strcat(buff, LogBuf);

    log << buff;
    log << "\n";
    log.close();
}

void Logger::log_printOnAlram(QString hpMode, struct tm turn_on, int stdbyReady, QString stdbyReadyMode, QString operMode, int level, struct tm start_treat,
                      struct tm end_treat, QString errCode)
{
    ofstream log;
    QString cmd, item;
    struct tm *getCurDateTime;
    int dateTimeMon;

    checkTargetDIR();

    getCurDateTime = getCurrentTime();

    dateTimeMon = getCurDateTime->tm_mon + 1;

    mDateTime.append(mDateTime.number(getCurDateTime->tm_year+1900));
    if ( 10 > dateTimeMon) {    // 1월 ~ 9월 일 때.
        mDateTime.append(mDateTime.number(0));
    }
    mDateTime.append(mDateTime.number(dateTimeMon));
    // mDateTime.append(mDateTime.number(getCurDateTime->tm_mday));

    cmd.append(TARGET_LOG_PATH);
    cmd.append("Gentlo");
    cmd.append(mDateTime);
    cmd.append("log.csv");

    char buff[2048] = {0};
    memset(buff, 0, sizeof(buff));

    qDebug() << "2. open :" << cmd.toStdString().c_str();
    log.open(cmd.toStdString().c_str(), ios::app);

    char operTimeString[128] = {0};
    char startTMTimeString[128] = {0};
    char endTMTimeString[128] = {0};
    char LogBuf[128] = {0};
    char itemStrBuf[512] = {0};

    item.append("Mode");
    item.append(",Device Turn On");
    item.append(",R/S");
    item.append(",Frequency");
    item.append(",Level");
    item.append(",Start Treatment Time");
    item.append(",Alram Occurrence Time");
    item.append(",Error Code\n");

    strftime(operTimeString, 80, "%Y-%m-%d %H:%M", &turn_on);
    strftime(startTMTimeString, 80, ",%Y-%m-%d %H:%M", &start_treat);
    strftime(endTMTimeString, 80, ",%Y-%m-%d %H:%M", &end_treat);

    // item
    sprintf(itemStrBuf, "%s", item.toStdString().c_str());
    strcat(buff, itemStrBuf);

    // hp mode
    sprintf(LogBuf, "%s,", hpMode.toStdString().c_str());
    strcat(buff, LogBuf);

    // date and time
    strcat(buff, operTimeString);
    // operation mode, level
    sprintf(LogBuf, ",%s,%s,%d", stdbyReadyMode.toStdString().c_str(), operMode.toStdString().c_str(), level);
    strcat(buff, LogBuf);

    if (stdbyReady) {
        // start treatment time
        strcat(buff, startTMTimeString);
    }

    // end treatment time
    strcat(buff, endTMTimeString);
    // Tip Error Code
    sprintf(LogBuf, ",%s\n", errCode.toStdString().c_str());
    strcat(buff, LogBuf);

    log << buff;
    log << "\n";
    log.close();
}

void Logger::log_printOnSettingComplete(struct tm turn_on, struct tm TipSettingTime, struct tm tip_info, int tip_serial, QString errCode)
{
    ofstream log;
    QString cmd, item;
    struct tm *getCurDateTime;
    int dateTimeMon;

    checkTargetDIR();

    getCurDateTime = getCurrentTime();

    dateTimeMon = getCurDateTime->tm_mon + 1;

    mDateTime.append(mDateTime.number(getCurDateTime->tm_year+1900));
    if ( 10 > dateTimeMon) {    // 1월 ~ 9월 일 때.
        mDateTime.append(mDateTime.number(0));
    }
    mDateTime.append(mDateTime.number(dateTimeMon));
    // mDateTime.append(mDateTime.number(getCurDateTime->tm_mday));

    cmd.append(TARGET_LOG_PATH);
    cmd.append("Gentlo");
    cmd.append(mDateTime);
    cmd.append("log.csv");

    char buff[1024] = {0};
    memset(buff, 0, sizeof(buff));

    qDebug() << "3. open :" << cmd.toStdString().c_str();
    log.open(cmd.toStdString().c_str(), ios::app);

    char operTimeString[128] = {0};
    char tipSettingCompleteTimeString[128] = {0};
    char tipReleaseInfo[32] = {0};
    char tipRemainingTime[32] = {0};
    char LogBuf[128] = {0};
    char itemStrBuf[512] = {0};

    item.append("Device Turn On");
    item.append(",Tip Setting Complete Time");
    item.append(",Tip Info");
    item.append(",Tip Remaining Time");
    item.append(",Error Code\n");

    strftime(operTimeString, 80, "%Y-%m-%d %H:%M", &turn_on);
    strftime(tipSettingCompleteTimeString, 80, ",%Y-%m-%d %H:%M", &TipSettingTime);
    strftime(tipReleaseInfo, 30, ",PST%Y%m%d", &tip_info);
    strftime(tipRemainingTime, 30, ",%H:%M:%S", &tip_info);

    // item
    sprintf(itemStrBuf, "%s", item.toStdString().c_str());
    strcat(buff, itemStrBuf);

    // date and time
    strcat(buff, operTimeString);
    // tip setting completed time
    strcat(buff, tipSettingCompleteTimeString);
    // Tip Information
    strcat(buff, tipReleaseInfo);
    // Tip Serial No
    sprintf(LogBuf, "%d", tip_serial);
    strcat(buff, LogBuf);
    // Tip Remaining Time
    strcat(buff, tipRemainingTime);
    // Tip Error Code
    sprintf(LogBuf, ",%s\n", errCode.toStdString().c_str());
    strcat(buff, LogBuf);

    log << buff;
    log << "\n";
    log.close();
}

#if defined(KZ_UI_DEBUG)
void Logger::log_printTestLog(QString hpMode, struct tm turn_on, int stdbyReady, QString stdbyReadyMode, QString operMode, int level, struct tm start_treat,
                      struct tm end_treat, int writeCount)
{
    ofstream log;
    QString cmd, item;
    struct tm *getCurDateTime;
    int dateTimeMon;

    checkTargetDIR();

    getCurDateTime = getCurrentTime();

    dateTimeMon = getCurDateTime->tm_mon + 1;

    mDateTime.append(mDateTime.number(getCurDateTime->tm_year+1900));
    if ( 10 > dateTimeMon) {    // 1월 ~ 9월 일 때.
        mDateTime.append(mDateTime.number(0));
    }
    mDateTime.append(mDateTime.number(dateTimeMon));
    // mDateTime.append(mDateTime.number(getCurDateTime->tm_mday));

    cmd.append(TARGET_LOG_PATH);
    cmd.append("GTDB");
    cmd.append(mDateTime);
    cmd.append("log.csv");

    char buff[2048] = {0};
    memset(buff, 0, sizeof(buff));

    qDebug() << "2. open :" << cmd.toStdString().c_str();
    log.open(cmd.toStdString().c_str(), ios::app);

    char operTimeString[128] = {0};
    char startTMTimeString[128] = {0};
    char endTMTimeString[128] = {0};
    char LogBuf[128] = {0};
    char itemStrBuf[512] = {0};

    item.append("Mode");
    item.append(",Device Turn On");
    item.append(",R/S");
    item.append(",Frequency");
    item.append(",Level");
    item.append(",Start Treatment Time");
    item.append(",Alram Occurrence Time");
    item.append(",Write Count\n");

    strftime(operTimeString, 80, "%Y-%m-%d %H:%M", &turn_on);
    strftime(startTMTimeString, 80, ",%Y-%m-%d %H:%M", &start_treat);
    strftime(endTMTimeString, 80, ",%Y-%m-%d %H:%M", &end_treat);

    // item
    sprintf(itemStrBuf, "%s", item.toStdString().c_str());
    strcat(buff, itemStrBuf);

    // hp mode
    sprintf(LogBuf, "%s,", hpMode.toStdString().c_str());
    strcat(buff, LogBuf);

    // date and time
    strcat(buff, operTimeString);
    // operation mode, level
    sprintf(LogBuf, ",%s,%s,%d", stdbyReadyMode.toStdString().c_str(), operMode.toStdString().c_str(), level);
    strcat(buff, LogBuf);

    if (stdbyReady) {
        // start treatment time
        strcat(buff, startTMTimeString);
    }

    // end treatment time
    strcat(buff, endTMTimeString);
    // Tip Error Code
    sprintf(LogBuf, ",%d\n", writeCount);
    strcat(buff, LogBuf);

    log << buff;
    log << "\n";
    log.close();
}

#endif

struct tm * Logger::getCurrentTime()
{
    struct timeval curTimeVal;
    struct tm *getCurTime;
    //char getDateTime[128] = {0};

    gettimeofday(&curTimeVal, NULL);
    getCurTime = (struct tm *)localtime((time_t *)&curTimeVal.tv_sec);
    //memset(getDateTime, 0x00, sizeof(getDateTime));

    //sprintf(getDateTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", getCurTime->tm_year+1900, getCurTime->tm_mon+1,
    //        getCurTime->tm_mday, getCurTime->tm_hour, getCurTime->tm_min, getCurTime->tm_sec, curTimeVal.tv_usec/1000);

    //qDebug() << "system time 2 = " << getDateTime;

    return getCurTime;
}

void Logger::do_work()
{
    getLogData();
}
