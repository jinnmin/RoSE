#ifndef SWUPDATER_H
#define SWUPDATER_H

#include "common.h"

#define FILENAME        "dfit_new"
#define SCRIPTNAME      "sw_update.sh"
#define FILEPNG         "*.png"

class SwUpdater : public QObject
{
    Q_OBJECT
public:
    explicit SwUpdater(QObject *parent = 0);
    ~SwUpdater();

    void reboot();

signals:
    void workFinished(bool, int);

public slots:
    void do_work();
    void do_work_rms();
//    void DetectedDev(QString);
//    void DetectedDev2(QString);

private:
    void detectDev();
    void searchDev();
    void searchDevRms();
    QString getDirList(QString);
    int startApplicationUpdate();
    int startApplicationUpdateRms();
    QByteArray getFileMd5Sum(QString);
    int copyUpdateFiles(QString, QString);
    void checkTargetDIR();
//    QFileSystemWatcher *fsWatcher;
    QByteArray mMd5hash;
    void systemReboot();
    QString mInsideDirPath;
};

#endif // SWUPDATER_H
