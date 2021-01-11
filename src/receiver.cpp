#include "receiver.h"

/* Mecro Function  ---------------------------------------------------------------------------------------------------*/
#define Q_HI_LOGO(...)     qDebug() << '[' << QString("%1 : %2").arg(__func__,30,QChar(' ')).arg( __LINE__, 4, 10, QChar(' ')) << ']' ##__VA_ARGS__

Receiver::Receiver(QSerialPort *serialPort, QObject *parent) :
    QObject(parent),
    mPacketStatus(false),
    mReceiverSerial(serialPort)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "Constructor 1 In()";
#endif
    if (NULL == mReceiverSerial) {
        qDebug() << __func__ << "mReceiverSerial is NULL";
        return;
    }
    else {
        /*
         * DirectConnection : synchronous 처리
         * QueuedConnection : asynchronous 처리
         */
        // connect(mReceiverSerial, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::QueuedConnection);
        connect(mReceiverSerial, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
        // Auto Connection.
        // connect(mReceiverSerial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }
}

Receiver::~Receiver()
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "2 In()";
#endif
}

void Receiver::onReadyRead()
{
    qDebug() << __func__;
    readData();
    makePacket();
}

void Receiver::readData()
{
    QByteArray recvData;

    qDebug() << __func__ << "1. bytesAvailable: " << mReceiverSerial->bytesAvailable();
    if (NULL != mReceiverSerial) {
        if(mReceiverSerial->isOpen()) {
            if (mReceiverSerial->isReadable()) {
                if (mReceiverSerial->bytesAvailable()) {
                    recvData = mReceiverSerial->readAll();
#if defined(KZ_UI_DEBUG)
//                      qDebug() << __func__ << "1. Recv Data : " << recvData;
//                      qDebug() << __func__ << "Recv Data Size : " << recvData.size();
#endif
                    mRecvQueue.enqueue(recvData);
                }
            }
        }
    }
}

void Receiver::makePacket() {
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "3 In()";
#endif

    // 수신된 데이터 처리를 완료할 때까지 Locking.
    QMutexLocker mLocker(&mMutex);
    unsigned long recvSize;

    // 수신된 데이터를 Queue로부터 임시버퍼에 인입한다.
    if (!mRecvQueue.isEmpty()) {
        checkRecvBArr.append(mRecvQueue.dequeue());
    }
    recvSize = checkRecvBArr.size();
    qDebug() << __func__ << "mPacketStatus: " << mPacketStatus << recvSize;

    // 예외처리 코드. 수신된 패킷이 있으나 데이터 처리 중 임시 버퍼를 비워버릴 경우
    // 이후 진행되는 코드에서 비워진 ByteArray를 참조하여 잘못된 참조를 일으킨다.
    // 이 예외를 방지하기 위하여 본 함수를 호출되었을 때 MutexLocker를 설정하여, 데이터 처리가 완료되기 전까지 대기할 수 있도록 한다.
    if (!recvSize) {
        qDebug() << __func__ << "Packet size has 0. This is serious error." << "Return !!!";
        return;
    }



    if(recvSize >= MSG_PROTOCOL_SIZE) {
//        mPacketStatus = PACKET_DONE;
        qDebug() << __func__ << "in mPacketStatus: " << mPacketStatus << PACKET_DONE;

#if defined(KZ_UI_DEBUG)
//      qDebug() << __func__ << "1. Recv recvBArr : " << recvBArr;
//      qDebug() << __func__ << "2. checkRecvBArr : " << checkRecvBArr;
#endif
//        packetChecker(recvBArr);  // JOON_TEST RoSE
        packetChecker(checkRecvBArr);  // JOON_TEST RoSE
        checkRecvBArr.clear();

    }

#if defined(KZ_UI_DEBUG)
//          qDebug() << __func__ << "3. Recv recvBArr : " << recvBArr;
//          qDebug() << __func__ << "4. checkRecvBArr : " << checkRecvBArr;
#endif
}

void Receiver::remainPacketChecker(unsigned long recvSize)
{
    quint16 msgLen = 0;
    unsigned long parsingDataSize = 0;
    qDebug() << __func__ << "In(unsigned long), recvSize : " << recvSize;

    // 루프를 돌면서 수신된 패킷에 START_BIT(0x1b01)이 있는지 체크한다.
    for (int loop = 0; loop < (int)recvSize; loop++) {

#if !defined(PH_PROTOCOL)
        if (checkRecvBArr.at(loop) == START_BIT) {
        // 메시지 길이를 확인. (프로토콜 상 index 3, 4를 확인하면 메시지 길이를 확인 할 수 있다)
                msgLen = MSG_PROTOCOL_SIZE;
//qDebug() << __func__ << "JOON_TEST 2. msgLen: " << msgLen;
                // 임시 버퍼의 데이터를 실제 데이터 처리를 위한 버퍼에 인입한다.
                recvBArr = checkRecvBArr.mid(loop, msgLen);
Q_HI_LOGO() <<  "recvBArr: " << recvBArr;

                parsingDataSize = recvBArr.size();

                // 실제 데이터를 체크하고 UI 업데이트를 할 수 있도록 SIGNAL 송신.
//qDebug() << __func__ << "JOON_TEST before 2. packetChecker, size : " << parsingDataSize;
                packetChecker(recvBArr);

                {
                    checkRecvBArr.clear();
//                    break;
                }

                /*
                // 한번에 여러 패킷을 받았을 경우를 위한 설비로 위에서 처음 처리한 패킷 사이즈가
                // 수신된 전체 패킷 사이즈보다 작을 경우 다음 패킷을 처리한다.
                if (parsingDataSize + loop < recvSize) {
                    // loop를 위에서 처음 처리한 패킷 이후로 건너뛰기 위하여 loop 값 변경한다.
                    // 이어지는 continue 구문에서 loop++이 진행되므로 하나 작은 값으로 loop 값을 변경.
                    loop = (parsingDataSize + loop) -1;
                    continue;
                }
                // 수신된 패킷을 모두 처리하였을 경우 임시 버퍼를 비우고 loop를 탈출한다.
                else {
                    checkRecvBArr.clear();
                    break;
                }
                */
        }
#else
        // START_BIT(0x1b01)가 있을 경우.
        if (checkRecvBArr.at(loop) == 0x1B) {
            if (checkRecvBArr.at(loop+1) == 0x01) {

                // 메시지 길이를 확인. (프로토콜 상 index 3, 4를 확인하면 메시지 길이를 확인 할 수 있다)
                msgLen = ((checkRecvBArr.at(loop+2) << 8) | checkRecvBArr.at(loop+3));
qDebug() << __func__ << "JOON_TEST 2. msgLen: " << msgLen;
                // 임시 버퍼의 데이터를 실제 데이터 처리를 위한 버퍼에 인입한다.
                recvBArr = checkRecvBArr.mid(loop, msgLen + MSG_HEADER_SIZE);
                parsingDataSize = recvBArr.size();

                // 실제 데이터를 체크하고 UI 업데이트를 할 수 있도록 SIGNAL 송신.
qDebug() << __func__ << "JOON_TEST before 2. packetChecker";
                packetChecker(recvBArr);

                // 한번에 여러 패킷을 받았을 경우를 위한 설비로 위에서 처음 처리한 패킷 사이즈가
                // 수신된 전체 패킷 사이즈보다 작을 경우 다음 패킷을 처리한다.
                if (parsingDataSize + loop < recvSize) {
                    // loop를 위에서 처음 처리한 패킷 이후로 건너뛰기 위하여 loop 값 변경한다.
                    // 이어지는 continue 구문에서 loop++이 진행되므로 하나 작은 값으로 loop 값을 변경.
                    loop = (parsingDataSize + loop) -1;
                    continue;
                }
                // 수신된 패킷을 모두 처리하였을 경우 임시 버퍼를 비우고 loop를 탈출한다.
                else {
                    checkRecvBArr.clear();
                    break;
                }
            }
        }
#endif
    }
}

void Receiver::remainPacketChecker(QByteArray _recvData)
{
    qDebug() << __func__ << "In(QByteArray)";
    quint16 msgLen = 0;
    unsigned long parsingDataSize = 0;
    QByteArray processableData;
    unsigned long recvSize = _recvData.size();

    // 루프를 돌면서 수신된 패킷에 START_BIT(0x1b01)이 있는지 체크한다.
    for (int loop = 0; loop < (int)recvSize; loop++) {
        // START_BIT(0x1b01)가 있을 경우.
        if (_recvData.at(loop) == 0x1B) {
            if (_recvData.at(loop+1) == 0x01) {

                // 메시지 길이를 확인. (프로토콜 상 index 3, 4를 확인하면 메시지 길이를 확인 할 수 있다)
                msgLen = ((_recvData.at(loop+2) << 8) | _recvData.at(loop+3));

                // 임시 버퍼의 데이터를 실제 데이터 처리를 위한 버퍼에 인입한다.
                processableData = _recvData.mid(loop, msgLen + MSG_HEADER_SIZE);
                parsingDataSize = processableData.size();

                // 실제 데이터를 체크하고 UI 업데이트를 할 수 있도록 SIGNAL 송신.
                packetChecker(processableData);

                // 한번에 여러 패킷을 받았을 경우를 위한 설비로 위에서 처음 처리한 패킷 사이즈가
                // 수신된 전체 패킷 사이즈보다 작을 경우 다음 패킷을 처리한다.
                if (parsingDataSize + loop < recvSize) {
                    // loop를 위에서 처음 처리한 패킷 이후로 건너뛰기 위하여 loop 값 변경한다.
                    // 이어지는 continue 구문에서 loop++이 진행되므로 하나 작은 값으로 loop 값을 변경.
                    loop = (parsingDataSize + loop) - 1;
                    processableData.clear();
                    continue;
                }
            }
        }
    }
}

void Receiver::packetChecker(QByteArray _recvData)
{
    quint16 etx = 0;
    quint16 fullPacketEtx = 0;
    int i;
    unsigned long recvSize = _recvData.size();
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "4 In(), recvData Size : " << recvSize;
#endif

    // 수신된 패킷이 프로토콜의 전체 패킷 사이즈와 같거나 이상이면 정상 패킷으로 간주한다.
    if (recvSize >= MSG_PROTOCOL_SIZE)
    {

        etx = (_recvData.at(MSG_PROTOCOL_SIZE - 4) << 8) | (_recvData.at(MSG_PROTOCOL_SIZE - 3));
        fullPacketEtx = (_recvData.at(MSG_PROTOCOL_SIZE - 2) << 8) | (_recvData.at(MSG_PROTOCOL_SIZE - 1));

        if(etx != CRC_BIT) {
            Q_HI_LOGO() << " &*&*&*&*&*&*&*&*&**&* In(), etx : " << hex << etx << " recvSize: "  << recvSize;
            etx = CRC_BIT;
        }

        // JOON_TEST
        fullPacketEtx = USER_STOP_BIT;


        if (CRC_BIT == etx && ((STOP_BIT == fullPacketEtx) || (USER_STOP_BIT == fullPacketEtx))) {
            // 추가로 받은 패킷에 여러 패킷이 혼재되어 있을 경우 패킷을 나누어 처리하기 위해 remainPacketChecker()에서 처리될 수 있도록 한다.
            Q_HI_LOGO() << "  In(), fullPacketEtx : " << hex << fullPacketEtx << " recvSize: "  << recvSize;

            mPacketStatus = PACKET_DONE;
            recvBArr.clear();
            checkRecvBArr.clear();

            // 현재 패킷 처리 상태를 DONE으로 처리한다.
            mPacketStatus = PACKET_DONE;

            // UI를 처리하도록 SIGNAL 송신한다.
            emit receivedData(_recvData);

        }
        else {
            qDebug() << __func__ << "this data has no stop bit!";
            // 수신된 패킷이 STOP_BIT가 없을 경우 패킷 처리 상태를 READING으로 변경하여 계속 수신할 수 있도록 한다.
            if (PACKET_READING != mPacketStatus) {
                mPacketStatus = PACKET_READING;
            }
        }
    }
    else {
        qDebug() << __func__ << "need to read next packet";
        // 수신된 패킷이 기본 프로토콜 전체 사이즈보다 작을 경우 다음 패킷을 추가로 받을 수 있도록
        // 패킷 처리 상태를 READING으로 변경한다.
        mPacketStatus = PACKET_READING;
    }
}
