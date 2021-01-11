/*	
 * Copyright 2015 Omid Sakhi (www.omidsakhi.com)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _YMODEM_H
#define _YMODEM_H

#include "common.h"
#include <cmath>
#include <QObject>
#include <QSerialPort>
#include <QBuffer>
#include <QFileInfo>
#include <QString>
#include <QRegularExpression>

#define FIRMWARE_FILENAME_SIZE              13      // main_fw_v1001, sono_fw_v1001
#define FIRMWARE_FILENAME_EXTENSION         "bin"   // bin
#define FIRMWARE_FILENAME_EXTENSION_SIZE    4       // .bin
#define FIRMWARE_MAIN               "GT_Main_*.bin"
#define FRIMWARE_SONO               "sono_fw_*.bin"
#define FRIMWARE_RMHP               "GTRM_fw_*.bin"
#define FRIMWARE_RVHP               "GTRV_fw_*.bin"
#define FRIMWARE_RNHP               "GTRN_fw_*.bin"
#define FRIMWARE_RCHP               "GTRC_fw_*.bin"

#define FIRMWARE_MAIN_CONTAINS      "GT_Main"
#define FRIMWARE_SONO_CONTAINS      "sono_fw"
#define FRIMWARE_RMHP_CONTAINS      "GTRM_fw"
#define FRIMWARE_RVHP_CONTAINS      "GTRV_fw"
#define FRIMWARE_RNHP_CONTAINS      "GTRN_fw"
#define FRIMWARE_RCHP_CONTAINS      "GTRC_fw"

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1000
#define DLY_3S 3000
#define DLY_5S 5000
#define DLY_10S 10000
#define MAXRETRANS 25
#define RETRY_COUNT 100

class YMODEM : public QObject
{
    Q_OBJECT
private:
    int mUpdateKinda;
    QString mUpdateFileName;
    bool m_cancel;
    QSerialPort *m_serialPort;
    QString mInsideDirPath;
    int yTransmitRead(unsigned short timeout);
    void yTransmitWrite(const QByteArray &ba );
    void yTransmitWrite( int c );
    void yTransmitFlush();
    unsigned short crc16_ccitt(const unsigned char *buf, int len);
    int yTransmitPacket(const QFileInfo &fileInfo, QIODevice &dev, const quint32 &payloadSize, quint8 sequenceNum = 0,bool firstPacket = true);
    void initPort();
    void checkTargetDIR();

public:
    enum YMODEMResponce {
        MODEM_SUCCESS = 0,
        MODEM_PACKET_FAILED = -1,
        MODEM_NO_SYNC = -2,
        MODEM_REMOTE_CANCEL = -3,
        MODEM_EOT_ERROR = -4,
        MODEM_LOCAL_CANCEL = -5,
        MODEM_SIZE_ERROR = -6,
        MODEM_RESTART = -7,
        MODEM_FILE_NOT_FOUND_ERROR = -8,
        MODEM_FILE_NOT_OPEN_ERROR = -9
    };
    explicit YMODEM(int kinda, QObject *parent = 0);
    int yTransmit(QString filePath);
    int yTransmitClose();
    bool isConnected();
    void cancel();

    QString SearchMainFwUpdateFile(int, QString);
    QString SearchMainFwUpdateFileRms(int, QString);
    QString getDirList(QString);

signals:
    void packet(int,int);
    void progress(int);
    void message(QString);
    void workFinished(bool, int);
    void checkedFile(QString);

public slots:
    void do_work();
    void do_work_rms();
};

#endif // _YMODEM_H
