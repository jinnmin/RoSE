#ifndef SENDER_H
#define SENDER_H

#include "common.h"

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QSerialPort *serialPort, QObject *parent = 0);
    ~Sender();

    void cmdUiPageInfo(int);
    void cmdSendToPower(int, int, int, int, int, int, int, int);
    void cmdSetFwDate(int, int, int, int, int, int);
    void cmdGetFwDate();
    void cmdGetTipInfo();
    void cmdSetTipSettings(int, int, int, int, int, int);
    void cmdSetStandbyReady(int, int, int, int, int, int, int, int, int, char, char);
    void cmdUiCartInfo(int, int, int, int, int, int, int);
    void cmdNeedleClean(int);
    void cmdSetAmingBeam(int);
    void cmdUiCurrDate(int, int, int);  // mDateYear, mDateMon, mDateDay
    void cmdBuzzerSound(int, int);
    void cmdOperationLevel(int, int);
    void cmdOperationBuzzerMdoe(int, int);
    void cmdSendOpertaionTime(int, int, int, int, int, int);
    void cmdReqTotalOperationTime(int);
    void cmdSetOperationStartStop(int, int);
    void cmdResetTotalOperationTime(int);
    void cmdSetAckToMCU(quint16 resmsg);
    void cmdConnect();
    void cmdReboot(int);
    void cmdUpdateFw(int);
#if defined(KZ_UI_GET_HP)
    void cmdGetConnectedHP();
    void cmdRepetion(int, int);
#endif
signals:

public slots:

private:
    QSerialPort *mSenderSerial;
    QMutex mMutex;
    struct message *mMsg;
    struct emptyPayloadMsg *mEmptyPayloadMsg;

    void connectToSerial();

    void writeAndRead(quint8, quint8, quint8, quint8 *);  // cmd, len, instuction, data
//    void writeAndRead(quint16, quint16, quint16);

    quint16 crc_sum(quint8 *data, int len);
    void connectDev();
#if defined(KZ_UI_GET_HP)
    void getConnectedHP();
#endif

    int mCurrYear;
    int mCurrMon;
    int mCurrDay;
};
#endif // SENDER_H
