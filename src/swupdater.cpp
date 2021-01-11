#include "swupdater.h"

SwUpdater::SwUpdater(QObject *parent) :
    QObject(parent)/*,
    fsWatcher(NULL)*/
{

}

SwUpdater::~SwUpdater()
{

}

void SwUpdater::reboot()
{
    systemReboot();
}

void SwUpdater::do_work()
{
    searchDev();
}

void SwUpdater::do_work_rms()
{
    searchDevRms();
}

//void SwUpdater::DetectedDev(QString dev)
//{
//    qDebug() << __func__ << dev;
//    QDir directory(dev);
//    QStringList FilesList = directory.entryList(QDir::AllDirs |  QDir::NoDotAndDotDot);
//    qDebug() << __func__ << FilesList;
//    qDebug() << __func__ << FilesList.size();

//    for (int loop = 0; loop < FilesList.size(); loop++) {
//        qDebug() << __func__ << FilesList[loop];
//    }
//}

//void SwUpdater::DetectedDev2(QString dev2)
//{
//    qDebug() << __func__ << dev2;
//}

//void SwUpdater::detectDev()
//{
//    qDebug() << __func__;
//     fsWatcher = new QFileSystemWatcher(this);
//     QString path = USB_PATH;
//     fsWatcher->addPath(path);
//     connect(fsWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(DetectedDev(QString)));
//     connect(fsWatcher, SIGNAL(fileChanged(QString)), this, SLOT(DetectedDev2(QString)));
//}

void SwUpdater::searchDev()
{
    int result = -1;
    qDebug() << __func__;

    result = startApplicationUpdate();

    qDebug() << __func__ << "result"  << result;

    if (SUCCESS == result) {
        qDebug() << __func__ << "Update Sucess!";
        emit workFinished(true, result);
    }
    else {
        qDebug() << __func__ << "Update Fail!";
        emit workFinished(false, result);
    }
}

void SwUpdater::searchDevRms()
{
    int result = -1;
    qDebug() << __func__;

    result = startApplicationUpdateRms();

    qDebug() << __func__ << "result"  << result;

    if (SUCCESS == result) {
        qDebug() << __func__ << "Update Sucess!";
        emit workFinished(true, result);
    }
    else {
        qDebug() << __func__ << "Update Fail!";
        emit workFinished(false, result);
    }
}

QString SwUpdater::getDirList(QString dirPath)
{
    QString dirName(QString::null);
    QString insideDirPath(QString::null);

    QDir directory(dirPath);
    QStringList dirList = directory.entryList(QDir::AllDirs |  QDir::NoDotAndDotDot);

    for (int i = 0; i < dirList.size(); i++) {
        qDebug() << __func__ << dirList.at(i);

        if (HIRONIC == dirList.at(i)) {
            qDebug() << __func__ << "found hironic!";
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

int SwUpdater::startApplicationUpdate()
{
    int ret = FAIL;
    int returnVal = FAIL;
    QStringList strFilters;
    QString usb(QString::null);
    QString dirName(QString::null);


    dirName = getDirList(USB_PATH);
    if (NULL == dirName) {
        qDebug() << "dir name" << dirName;
        return ERR_DIR_NOT_FOUND;
    }

    usb.append(mInsideDirPath);
    usb.append("/");
    usb.append(dirName);
    usb.append("/");

    strFilters += SCRIPTNAME;
    strFilters += FILENAME;
    strFilters += FILEPNG;

    QDirIterator iterDir(usb, strFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    while (iterDir.hasNext())
    {
        iterDir.next();
        qDebug() << __func__ << iterDir.fileName();
        // qDebug() << __func__ << iterDir.fileInfo().isExecutable();
        // qDebug() << __func__ << iterDir.filePath();
        // qDebug() << getFileMd5Sum(iterDir.filePath()).toHex();

        this->mMd5hash = getFileMd5Sum(iterDir.filePath()).toHex();

        ret = copyUpdateFiles(iterDir.filePath(), iterDir.fileName());
        if (ret == SUCCESS)
            returnVal = ret;    // gentlo, update_new.sh 파일 두가지 중 하나만 복사되어도 성공.
    }

    if(ret == SUCCESS)
    {
        qDebug() << __func__ << " start SW Upgrade" << endl;

        QString cmd = "/home/app/script/sw_update.sh";

        QProcess process;
        process.start("sh", QStringList() << "-c" << cmd);

        process.waitForFinished();
        QString output(process.readAllStandardOutput());

        if(output.contains("sw upgrade done")){

            qDebug() << "*** SW_upgrade success";
            returnVal = SUCCESS;
            return returnVal;

        }
        else {
            qDebug() << "*** SW_upgrade fail";
            returnVal = FAIL;
            sleep(2000);
            return returnVal;
        }
    }

    return returnVal;
}

int SwUpdater::startApplicationUpdateRms()
{

    int returnVal = FAIL;


    qDebug() << __func__ << " start rms_sw_down" << endl;

    QString cmd = "/home/pi/hironic/script/rms_sw_down.sh";

    QProcess process;
    process.start("sh", QStringList() << cmd);

    process.waitForFinished();
    QString output(process.readAllStandardOutput());

    if(output.contains("update done")){

        qDebug() << "*** SW_upgrade success";
        returnVal = SUCCESS;
        return returnVal;

    }
    else {
        qDebug() << "*** RMS SW_upgrade fail";
        returnVal = FAIL;
        sleep(2000);
        return returnVal;
    }



}

QByteArray SwUpdater::getFileMd5Sum(QString filepath)
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

int SwUpdater::copyUpdateFiles(QString filepath, QString filename)
{
    int ret = FAIL;
    QString cmd, target;

    // checkout target directory
    checkTargetDIR();

    // copy
    cmd.append("cp -f");
    cmd.append(" ");
    cmd.append(filepath);
    cmd.append(" ");
    cmd.append(TARGET_PATH);
    qDebug() << "cmd path : " << cmd;
    ret = system(cmd.toStdString().c_str());

    // comparison md5sum target files
    target.append(TARGET_PATH);
    target.append(filename.toStdString().c_str());
    if (this->mMd5hash == getFileMd5Sum(target.toStdString().c_str()).toHex()) {
        this->mMd5hash.clear();
        qDebug() << "file copy completed";
        ret = SUCCESS;
    }
    else {
        qDebug() << "file copy error";
    }

    return ret;
}

void SwUpdater::checkTargetDIR()
{
    QDir targetDir(TARGET_PATH);

    if (!targetDir.exists()) {
        qDebug() << __func__ << "target directory is not exist!";
        targetDir.mkpath(TARGET_PATH);
        qDebug() << __func__ << "target directory is creaded!";
    }
    else {
        qDebug() << __func__ << "target directory is alreay exist!";
    }
}

void SwUpdater::systemReboot()
{
    QString cmd;
    // sync
    cmd.append("sync");
    int ret = system(cmd.toStdString().c_str());
    // sync
    ret = system(cmd.toStdString().c_str());

    // reboot
    cmd.clear();
    cmd.append("sudo reboot");
    ret = system(cmd.toStdString().c_str());

    qDebug() << "cmd result : " << ret;
}
