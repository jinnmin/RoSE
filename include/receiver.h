#ifndef RECEIVER_H
#define RECEIVER_H

#include "common.h"

enum {
    PACKET_DONE = 0,
    PACKET_READING
};

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QSerialPort *serialPort, QObject *parent = 0);
    ~Receiver();

signals:
    void receivedData(QByteArray);

public slots:
    void onReadyRead();

private:
    int mPacketStatus;
    QSerialPort *mReceiverSerial;
    QByteArray checkRecvBArr;
    QByteArray recvBArr;
    QQueue<QByteArray> mRecvQueue;
    QMutex mMutex;
    void readData();
    void makePacket();
    void packetChecker(QByteArray);
    void remainPacketChecker(unsigned long);
    void remainPacketChecker(QByteArray);
};

#endif // RECEIVER_H
