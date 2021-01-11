#ifndef COMMON_H
#define COMMON_H

#include "hspsprotocol.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QString>
#include <QDataStream>
#include <QDebug>
#include <QtEndian>
#include <QMutex>
#include <QFileSystemWatcher>
#include <QDir>
#include <QDirIterator>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QQueue>
#include <QProcess> // 2018-05-28 JJHWANG Logo Change

#define SERIAL_DEV       "/dev/serial0"

#define DECTOBCD(x) ((((x/1000)%10)<<12)|(((x/100)%10)<<8)|\
                    (((x/10)%10)<<4)|(x%10))
#define BCDTODEC(x) ((((x & 0xF000) >> 12) * 1000) + (((x & 0xF00) >> 8) * 100)\
                    + (((x & 0xF0) >> 4) * 10) + (x & 0x0F))

/*
 * 년 : 9999 / (60 * 60 * 24 * 365)
 * 월 : 년의 나머지 / (60 * 60 * 24 * 12)
 * 일 : 월의 나머지 / (60 * 60 * 24)
 * 시 : 일의 나머지 / (60 * 60)
 * 분 : 시의 나머지 / (60)
 * 초 : 분의 나머지
 */
#define TM_YEAR(x)      x / (60 * 60 * 24 * 365);
#define TM_MON(x)       (x % (60 * 60 * 24 * 365)) / (60 * 60 * 24 * 12);
#define TM_DAY(x)       ((x % (60 * 60 * 24 * 365)) % (60 * 60 * 24 * 12)) / (60 * 60 * 24);
#define TM_HOUR(x)      ((x % (60 * 60 * 24 * 365)) % (60 * 60 * 24 * 12) % (60 * 60 * 24)) / (60 * 60);
#define TM_MIN(x)       ((((x % (60 * 60 * 24 * 365)) % (60 * 60 * 24 * 12)) % (60 * 60 * 24)) % (60 * 60)) / 60;
#define TM_SEC(x)       ((((x % (60 * 60 * 24 * 365)) % (60 * 60 * 24 * 12)) % (60 * 60 * 24)) % (60 * 60)) % 60;

#define FAIL                0
#define SUCCESS             1
#define ERR_DIR_NOT_FOUND   2
#define ERR_FILE_NOT_FOUND   3

//#define USB_PATH    "/media/pi"
#define USB_PATH    "/home/app/usb"
#define TARGET_PATH "/home/app/bin/"
#define TARGET_LOG_PATH "/home/pi/hironic/log/"
#define HIRONIC "hironic"
#define LOG "log"
#define DB_FILE_DIR "/home/pi/hironic/log/"
#define DB_FILE_NAME "tb_patient_total.csv"
#define S3_FILE_DIR "s3://hironicstor/LOG/GENTLO/"

struct message {
    quint16 mStx;
    quint8 deviceId;

    quint8 msgCode;

    quint8 msgLen;
    quint8 msgInstruction;

    quint8 *msgPayload;
    quint16 mCrc;
    quint16 mEtx;
};

struct emptyPayloadMsg {
    quint16 mStx;
    quint8 deviceId;

    quint8 msgCode;

    quint8 msgLen;
    quint8 msgInstruction;

    quint8 *msgPayload;
    quint16 mCrc;
    quint16 mEtx;
};

#endif // COMMON_H
