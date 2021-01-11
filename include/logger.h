#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"
#include <fstream>
#include <sys/time.h>

using namespace std;
using std::ofstream;
using std::ifstream;

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    ~Logger();

    void detectDev();
    void getLogData();
    void getFileList(QString path);
    QString getDirList(QString);
    int copyFiles();
    QByteArray getFileMd5Sum(QString);
    void checkTargetDIR();
    void checkUsbDIR(QString);
    void log_print(int, struct tm, QString, QString, int, struct tm, struct tm, struct tm,
                  int, int, struct tm, QString);
    void log_printOnAlram(QString, struct tm, int, QString, QString, int, struct tm, struct tm, QString);
    void log_printOnSettingComplete(struct tm, struct tm, struct tm, int, QString);
    struct tm * getCurrentTime();

#if defined(KZ_UI_DEBUG)
    void log_printTestLog(QString, struct tm, int, QString, QString, int, struct tm, struct tm, int);
#endif

signals:
    void workFinished(bool, int);

public slots:
    void do_work();
    void DetectedDev(QString dev);

private:
    QByteArray mMd5hash;
    QString mDateTime;
    QFileSystemWatcher *fsWatcher;
    QString mInsideDirPath;
};

#endif // LOGGER_H
