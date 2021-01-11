#include "sender.h"

Sender::Sender(QSerialPort *serialPort, QObject *parent) :
    QObject(parent),
    mSenderSerial(serialPort),
    mMsg(NULL),
    mEmptyPayloadMsg(NULL)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "Constructor() 30 In";
#endif
    if (NULL == mSenderSerial) {
        qDebug() << __func__ << "mSenderSerial is NULL";
        return;
    }
}

Sender::~Sender()
{
}

void Sender::cmdUiPageInfo(int _pageNo)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "pageinfo : " << _pageNo;
#endif
    quint8 pageInfo = (quint8) _pageNo;

    // writeAndRead(DF_MAIN_BD, REQ_UI_PAGE_INFO, sizeof(pageInfo), (quint8 *)&pageInfo);
}


void Sender::cmdSendToPower(int _cmd, int getSet, int _data1, int _data2, int _data3, int _data4, int _data5, int _data6)
{
    quint8 send_data[6] = {0};
    QString dbgMsg = "";

    send_data[0] = (qint8) _data1;
    send_data[1] = (qint8) _data2;
    send_data[2] = (qint8) _data3;
    send_data[3] = (qint8) _data4;
    send_data[4] = (qint8) _data5;
    send_data[5] = (qint8) _data6;

    writeAndRead(_cmd, getSet, sizeof(send_data), (quint8 *)&send_data);
//     writeAndRead(DF_MAIN_BD, _cmd, sizeof(send_data), (quint8 *)&send_data);
}

#if defined(KZ_UI)
quint16 setSendHpInfo (int hp) {
    quint16 hpInfo =DF_BD;

    switch(hp) {
        case HP_ALEX:
            hpInfo = DF_BD;
            break;
        case HP_NDYAG:
            hpInfo = DF_BD;
            break;

        default:
            break;
    }

    return hpInfo;
}

#endif


void Sender::cmdBuzzerSound(int hp, int mode)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "hp : " << hp << "mode : " << mode;
#endif

    quint8 buzzerMode[3] = {0};
    quint16 hpBd = 0;

    qDebug() << __func__ << "*  cmdBuzzerSound hp : " << hp;

    hpBd = setSendHpInfo(hp);

    buzzerMode[0] = (qint8)((hpBd >> 8) & 0xFF);
    buzzerMode[1] = (qint8)(hpBd & 0xFF);
    buzzerMode[2] = (qint8)mode;

    // writeAndRead(DF_MAIN_BD, REQ_SET_BUZZER_SOUND, sizeof(buzzerMode), (quint8 *)&buzzerMode);

}

void Sender::cmdOperationLevel(int hp, int level)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "hp : " << hp << "level : " << level;
#endif
    quint8 operationLevel[3] = {0};
    quint16 hpBd = 0;

    qDebug() << __func__ << "*  cmdOperationLevel hp : " << hp;

    hpBd = setSendHpInfo(hp);

    operationLevel[0] = (qint8)((hpBd >> 8) & 0xFF);
    operationLevel[1] = (qint8)(hpBd & 0xFF);
    operationLevel[2] = (qint8)level;
    // writeAndRead(DF_MAIN_BD, REQ_SET_LEVEL, sizeof(operationLevel), (quint8 *)&operationLevel);

}

void Sender::cmdOperationBuzzerMdoe(int hp, int buzzerMode)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "hp : " << hp << "buzzerMode : " << buzzerMode;
#endif

    quint8 operationBuzzerMode[3] = {0};
    quint16 hpBd = 0;

    hpBd = setSendHpInfo(hp);

    operationBuzzerMode[0] = (qint8)((hpBd >> 8) & 0xFF);
    operationBuzzerMode[1] = (qint8)(hpBd & 0xFF);
    operationBuzzerMode[2] = (qint8)buzzerMode;
    // writeAndRead(DF_MAIN_BD, REQ_SET_BUZZER_MODE, sizeof(operationBuzzerMode), (quint8 *)&operationBuzzerMode);

}

void Sender::cmdSendOpertaionTime(int hp, int ttmin, int ttsec, int tipRemainHH, int tipRemainMM, int tipRemainSS)
{

    quint8 oeprationTime[7] = {0};
    quint16 hpBd = 0;

    qDebug() << __func__ << "*  cmdSendOpertaionTime hp : " << hp;

    hpBd = setSendHpInfo(hp);

    oeprationTime[0] = (qint8)((hpBd >> 8) & 0xFF);
    oeprationTime[1] = (qint8)(hpBd & 0xFF);
    oeprationTime[2] = (qint8)ttmin;
    oeprationTime[3] = (qint8)ttsec;
    oeprationTime[4] = (qint8)tipRemainHH;
    oeprationTime[5] = (qint8)tipRemainMM;
    oeprationTime[6] = (qint8)tipRemainSS;
    // writeAndRead(DF_MAIN_BD, REQ_SEND_OPERATION_TIME, sizeof(oeprationTime), (quint8 *)&oeprationTime);

}

void Sender::cmdReqTotalOperationTime(int hp)
{
    qDebug() << __func__ << "hp : " << hp;
    quint8 hpInfo = (quint8) hp;
    // writeAndRead(DF_MAIN_BD, REQ_TOTAL_OPERATION_TIME, sizeof(hpInfo), (quint8 *)&hpInfo);
}

void Sender::cmdSetOperationStartStop(int hp, int startStop)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "hp : " << hp << "startStop : " << startStop;
#endif

    quint8 operationStartStop[3] = {0};
    quint16 hpBd = 0;

    qDebug() << __func__ << "*  cmdSetOperationStartStop hp : " << hp;

    hpBd = setSendHpInfo(hp);

    operationStartStop[0] = (qint8)((hpBd >> 8) & 0xFF);
    operationStartStop[1] = (qint8)(hpBd & 0xFF);
    operationStartStop[2] = (qint8)startStop;
    // writeAndRead(DF_MAIN_BD, REQ_SET_LEVEL, sizeof(operationStartStop), (quint8 *)&operationStartStop);

}

void Sender::cmdResetTotalOperationTime(int hp)
{
    qDebug() << __func__ << "hp : " << hp;
    quint8 hpInfo = (quint8) hp;
//     writeAndRead(DF_MAIN_BD, REQ_RESET_TOTAL_OPERATION_TIME, sizeof(hpInfo), (quint8 *)&hpInfo);

}

void Sender::cmdSetAckToMCU(quint16 resmsg)
{
    quint8 hpInfo[2] = {0};
    quint16 hpBd = DF_BD;

    hpInfo[0] = (qint8)((hpBd >> 8) & 0xFF);
    hpInfo[1] = (qint8)(hpBd & 0xFF);
    qDebug() << __func__  << endl;
    // writeAndRead(DF_MAIN_BD, resmsg, sizeof(hpInfo), (quint8 *)&hpInfo);
}

void Sender::cmdConnect()
{
    connectToSerial();
}

#if defined(KZ_UI_GET_HP)
void Sender::cmdGetConnectedHP()
{
     qDebug() << __func__  << endl;
    getConnectedHP();
}
#endif


void Sender::cmdReboot(int currBd)
{
    quint8 hpInfo[2] = {0};
    quint16 hpBd = DF_BD;

    hpInfo[0] = (qint8)((hpBd >> 8) & 0xFF);
    hpInfo[1] = (qint8)(hpBd & 0xFF);
    qDebug() << __func__  << endl;
    // writeAndRead(DF_MAIN_BD, REQ_REBOOT, sizeof(hpInfo), (quint8 *)&hpInfo);
}

void Sender::cmdUpdateFw(int type)
{
    qDebug() << __func__ << "type : " << hex << type;
    quint8 updateFw[2] = {0};

    updateFw[0] = ((type >> 8) & 0xFF);
    updateFw[1] = (type & 0xFF);

    // writeAndRead(DF_MAIN_BD, RES_FW_UPDATE, sizeof(updateFw), (quint8 *)&updateFw);

}

void Sender::cmdNeedleClean(int mode)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "mode : " << mode;
#endif
    quint8 needleClean[3] = {0,0,0};

    needleClean[0] = ((RM_BD >> 8) & 0xFF);
    needleClean[1] = (RM_BD & 0xFF);
    needleClean[2] = (quint8) mode;

//    writeAndRead(RM_BD, REQ_RM_NEEDLE_CLEAN, sizeof(needleClean), (quint8 *)&needleClean);
}

void Sender::cmdSetAmingBeam(int level)
{
#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "level : " << level;
#endif
    quint8 aimingBeamLevel[3] = {0,0,0};

    aimingBeamLevel[0] = ((DF_BD >> 8) & 0xFF);
    aimingBeamLevel[1] = (DF_BD & 0xFF);
    aimingBeamLevel[2] = (quint8) level;

//    writeAndRead(DF_BD, REQ_SET_AIMING_BEAM, sizeof(aimingBeamLevel), (quint8 *)&aimingBeamLevel);
}

void Sender::cmdSetFwDate(int yy, int mm, int dd, int h, int m, int s)
{
    quint8 date[6] = {0};
    date[0] = (qint8)yy;
    date[1] = (qint8)mm;
    date[2] = (qint8)dd;
    date[3] = (qint8)h;
    date[4] = (qint8)m;
    date[5] = (qint8)s;

    // writeAndRead(DF_MAIN_BD, REQ_FW_SET_DATE, sizeof(date), (quint8 *)&date);

}

void Sender::cmdGetFwDate()
{
    quint8 hpInfo[2] = {0};
    quint16 hpBd = DF_BD;

    hpInfo[0] = (qint8)((hpBd >> 8) & 0xFF);
    hpInfo[1] = (qint8)(hpBd & 0xFF);
    qDebug() << __func__  << endl;
    // writeAndRead(DF_MAIN_BD, REQ_DATE, sizeof(hpInfo), (quint8 *)&hpInfo);

}


void Sender::cmdGetTipInfo()
{
    quint8 hpInfo[2] = {0};
    quint16 hpBd = DF_BD;

    hpInfo[0] = (qint8)((hpBd >> 8) & 0xFF);
    hpInfo[1] = (qint8)(hpBd & 0xFF);
    qDebug() << __func__  << endl;
    // writeAndRead(DF_MAIN_BD, RES_PS_TIP_INFO, sizeof(hpInfo), (quint8 *)&hpInfo);

}

void Sender::cmdSetTipSettings(int sn, int yy, int mm, int dd, int h, int m)
{
    quint8 tipSettings[11] = {0};
    tipSettings[0] = (qint8)'P';
    tipSettings[1] = (qint8)'S';
    tipSettings[2] = (qint8)'T';
    tipSettings[3] = (qint8)DECTOBCD(yy);
    tipSettings[4] = (qint8)DECTOBCD(mm);
    tipSettings[5] = (qint8)DECTOBCD(dd);
    tipSettings[6] = (qint8)((sn >> 8) & 0xFF);
    tipSettings[7] = (qint8)(sn & 0xFF);
    tipSettings[8] = (qint8)DECTOBCD(h);
    tipSettings[9] = (qint8)DECTOBCD(m);
    tipSettings[10] = (qint8)0x00;

    // writeAndRead(DF_MAIN_BD,REQ_PS_TIP_SETTING, sizeof(tipSettings), (quint8 *)&tipSettings);

}

void Sender::cmdSetStandbyReady(int hp_info,int pulseDuration,int  psuVol,int  pulseRate,int  preSpray,int  ccdDelay,int  postSpray,int  aimingBean,int  status, char spotSize, char ccdOnOff)
{
    quint16 hpBd = DF_BD;

#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << " ***  hp_info : " << hp_info << "cmdSetStandbyReady  status: " << status;
#endif

    quint8 standbyData[17] = {0};

    standbyData[0] = (qint8)((hpBd >> 8) & 0xFF);
    standbyData[1] = (qint8)(hpBd & 0xFF);
    standbyData[2] = (qint8)status;
    standbyData[3] = (qint8)((psuVol >> 8) & 0xFF);
    standbyData[4] = (qint8)(psuVol & 0xFF);
    standbyData[5] = (qint8) spotSize;
    standbyData[6] = (qint8)((pulseDuration >> 8) & 0xFF);
    standbyData[7] = (qint8)(pulseDuration & 0xFF);
    standbyData[8] = (qint8) pulseRate;
    standbyData[9] = (qint8)((preSpray >> 8) & 0xFF);
    standbyData[10]=  (qint8)(preSpray & 0xFF);
    standbyData[11] = (qint8)((ccdDelay >> 8) & 0xFF);
    standbyData[12]=  (qint8)(ccdDelay & 0xFF);
    standbyData[13] = (qint8)((postSpray >> 8)  & 0xFF);
    standbyData[14] = (qint8)(postSpray & 0xFF);
    standbyData[15] = (qint8)(ccdOnOff & 0xFF);
    standbyData[16] = (qint8)(aimingBean & 0xFF);

    // writeAndRead(DF_MAIN_BD, REQ_SET_STANDBY_READY, sizeof(standbyData), (quint8 *)&standbyData);

}


#if defined(KZ_UI)

void Sender::cmdUiCurrDate(int yy, int mm, int dd)
{
    mCurrYear = yy;
    mCurrMon = mm;
    mCurrDay =dd;
}

void Sender::cmdUiCartInfo(int hp_info, int setGet, int countryCode, int sn, int currCnt, int totalCnt, int tipType)
{
    quint8 cartInfoData[36] = {0};
    quint16 hpBd = 0;

    quint16 cartNation_Code = countryCode;
    quint16 cartFreq = 0;
    quint8 cartType[5][4] = {
        {'R','M','4','9'},
        {'R','M','2','5'},
        {'I','S','4','9'},
        {'I','S','2','5'},
        {'P','S','T','0'},
    };
    quint8 cartPowerCal = 0;
    quint32 cartCurrentCount = currCnt;
    quint32 cartMaxCount = totalCnt;
    quint32 cartCurrentCountBack = 0;
    quint8 cartWriteNum = 1;

#if defined(KZ_UI_DEBUG)
    qDebug() << __func__ << "*  cmdUiCartInfo hp : " << hp_info;
    qDebug() << __func__ << "* &***&&&**** cmdUiCartInfo setGet : " << setGet;
    qDebug() << __func__ << "* &***&&&**** cartMaxCount : " << cartMaxCount;
    qDebug() << __func__ << " ***  mCurrYear : " << mCurrYear << " mCurrMon: " << mCurrMon;

#endif

    hpBd = setSendHpInfo(hp_info);

    cartInfoData[0] = (qint8)setGet;  // 0: get , 1: set
    cartInfoData[1] = (qint8)((hpBd >> 8) & 0xFF);
    cartInfoData[2] = (qint8)(hpBd & 0xFF);
    cartInfoData[3] = (qint8)0x00;  // connection

    if(hp_info == 2) {
        cartInfoData[4] = (qint8)'R';
        cartInfoData[5] = (qint8)'M';
        cartInfoData[6] = (qint8)'C';
    }
    else {
        cartInfoData[4] = (qint8)'P';
        cartInfoData[5] = (qint8)'S';
        cartInfoData[6] = (qint8)'T';
    }
    cartInfoData[7] = (qint8)DECTOBCD(mCurrYear);
    cartInfoData[8] = (qint8)DECTOBCD(mCurrMon);
    cartInfoData[9] = (qint8)DECTOBCD(mCurrDay);
    cartInfoData[10] = (qint8)((cartNation_Code >> 8) & 0xFF);
    cartInfoData[11] = (qint8)(cartNation_Code & 0xFF);
    cartInfoData[12] = (qint8)((sn >> 24) & 0xFF);
    cartInfoData[13] = (qint8)((sn >> 16) & 0xFF);
    cartInfoData[14] = (qint8)((sn >> 8) & 0xFF);
    cartInfoData[15] = (qint8)(sn & 0xFF);
    cartInfoData[16] = (qint8)((cartFreq >> 8) & 0xFF);
    cartInfoData[17] = (qint8)(cartFreq & 0xFF);
    if(hp_info == 2) {
        cartInfoData[18] = (qint8)cartType[tipType][0];
        cartInfoData[19] = (qint8)cartType[tipType][1];
        cartInfoData[20] = (qint8)cartType[tipType][2];
        cartInfoData[21] = (qint8)cartType[tipType][3];
        cartInfoData[22] = (qint8)cartPowerCal;
        cartInfoData[23] = (qint8)((cartCurrentCount >> 24) & 0xFF);
        cartInfoData[24] = (qint8)((cartCurrentCount >> 16) & 0xFF);
        cartInfoData[25] = (qint8)((cartCurrentCount >> 8) & 0xFF);
        cartInfoData[26] = (qint8)(cartCurrentCount & 0xFF);
        cartInfoData[27] = (qint8)((cartMaxCount >> 24) & 0xFF);
        cartInfoData[28] = (qint8)((cartMaxCount >> 16) & 0xFF);
        cartInfoData[29] = (qint8)((cartMaxCount >> 8) & 0xFF);
        cartInfoData[30] = (qint8)(cartMaxCount & 0xFF);
        cartInfoData[31] = (qint8)((cartCurrentCountBack >> 24) & 0xFF);
        cartInfoData[32] = (qint8)((cartCurrentCountBack >> 16) & 0xFF);
        cartInfoData[33] = (qint8)((cartCurrentCountBack >> 8) & 0xFF);
        cartInfoData[34] = (qint8)(cartCurrentCountBack & 0xFF);
    }
    else {
        cartInfoData[18] = (qint8)cartType[tipType][0];
        cartInfoData[19] = (qint8)cartType[tipType][1];
        cartInfoData[20] = (qint8)cartType[tipType][2];
        cartInfoData[21] = (qint8) 0;
        cartInfoData[22] = (qint8)cartPowerCal;

        cartCurrentCount *= 30;

        cartInfoData[23] = (qint8) 0x00;
        cartInfoData[24] = (qint8) (cartCurrentCount/60);
        cartInfoData[25] = (qint8) (cartCurrentCount%60);
        cartInfoData[26] = (qint8) 0x00;
        cartInfoData[27] = (qint8) 0x00;
        cartInfoData[28] = (qint8) (cartCurrentCount/60);
        cartInfoData[29] = (qint8) (cartCurrentCount%60);
        cartInfoData[30] = (qint8) 0x00;
        cartInfoData[31] = (qint8) 0x00;
        cartInfoData[32] = (qint8) (cartCurrentCount/60);
        cartInfoData[33] = (qint8) (cartCurrentCount%60);
        cartInfoData[34] = (qint8) 0x00;
    }


    cartInfoData[35] = (qint8) cartWriteNum;

    // writeAndRead(DF_MAIN_BD, REQ_SET_GET_CART_INFO, sizeof(cartInfoData), (quint8 *)&cartInfoData);
}
#endif

void Sender::connectToSerial()
{
qDebug() << __func__ << "mSenderSerial: ", mSenderSerial;
    if (NULL != mSenderSerial) {
        if (mSenderSerial->isOpen())  {
            qDebug() << __func__ << "mSerial is already opened ? ";
            mSenderSerial->close();
        }

        if (mSenderSerial->open(QIODevice::ReadWrite)) {
//            connectDev(); // JOON_TEST LZ
            qDebug() << __func__ << "connectDev() ";
        }
        else {
            qDebug() << __func__ << "failed to open serial port!";
        }
    }
}

void Sender::writeAndRead(quint8 msgCode, quint8 getSet, quint8 msgLen, quint8 *msgData)
{
/*
 * PARAMETER DATA SIZE (BYTE)
 * +----------------------------------------------------------------------------------------------------------------------------------------------+
 * | START_BIT || (HEADER)DeviceID  || (HEADER)CMD        || (HEADER) LEN     || (HEADER) INSTRUCTION ||(DATA) CODE  || (DATA) CRC_BIT | STOP_BIT |
 * +----------------------------------------------------------------------------------------------------------------------------------------------+
 * | 2BYTE     || 1BYTE             || 1BYTE              || 1BYTE            || 1BYTE                ||6BYTE        || 2BYTE          | 2BYTE    |
 * +----------------------------------------------------------------------------------------------------------------------------------------------+
 * DATA_BYTE = 2 + 1 + 1 + 1 + 1 + 6 + 2 + 2 => 16 Bytes

 */
    int wb = 0;
    int wcnt = 4;
    QByteArray sendData;
    quint8 tempCmd;
    quint8 deviceId = 1;

    sendData.resize(MSG_PROTOCOL_SIZE);


#if defined(KZ_UI_DEBUG)
    qDebug() << " 0 msgCode : " << hex << msgCode;
    qDebug() << __func__ << "2. msgLen : " << msgLen;
#endif // end KZ_UI_DEBUG

    tempCmd = msgCode;
    QDataStream stream(&sendData, QIODevice::WriteOnly);

    stream << START_BIT << deviceId << tempCmd << msgLen << getSet;

    for(int loop = 0 ; loop < MSG_DEFAULT_SIZE; loop++)
    {
        stream << msgData[loop];
        qDebug() << "1. msgData[" << loop << "] : " << msgData[loop];
    }

//    if((msgCode == REQ_KEEP_ARRIVE) || (msgCode ==  REQ_GET_VERSION))
//    {
//        stream << CRC_BIT << USER_STOP_BIT;
//    }
//    else
    {
        stream << CRC_BIT << STOP_BIT;
    }

qDebug() << "1. sendData : " << sendData << "sendData.size()" << sendData.size();


    if( sendData.size() != MSG_PROTOCOL_SIZE)
    {
        return;
    }

    if( mSenderSerial->isWritable() )
    {
        // Write to the serial port and wait for up to 100 ms until the recording is done.
        qDebug() << "1. sendData Size : " << sendData.size();
        do
        {
            wb = mSenderSerial->write( sendData );
            mSenderSerial->waitForBytesWritten( 100 );
            wcnt--;
        } while( wb != sendData.size() && wcnt >= 0 );
    }
}

quint16 Sender::crc_sum(quint8 *data, int len)
{
    quint16 sum = 0;
    quint16 tmp = 0;

    for(;;) {
        if(len == 1) {
            tmp = (quint16) *data;
            sum ^= tmp;
            break;
        } else if (len == 0) {
            break;
        } else {
            sum ^= *((quint16 *)data);
            data += 2;
            len -= 2;
        }
    }
    return sum;
}

void Sender::connectDev()
{
    quint8 hpInfo[2] = {0};
    quint16 hpBd = DF_BD;
    qDebug() << __func__ << "*  Selected hp : " << hpBd;

    hpInfo[0] = (qint8)((hpBd >> 8) & 0xFF);
    hpInfo[1] = (qint8)(hpBd & 0xFF);

    // writeAndRead(DF_MAIN_BD, KEEP_ALIVE, sizeof(hpInfo), (quint8 *)&hpInfo);

}

#if defined(KZ_UI) // 2018-09-20

void Sender::getConnectedHP()
{
    quint8 hpInfo[2] = {0};
    quint16 hpBd = DF_BD;

    hpInfo[0] = (qint8)((hpBd >> 8) & 0xFF);
    hpInfo[1] = (qint8)(hpBd & 0xFF);
    qDebug() << __func__  << endl;
    // writeAndRead(DF_MAIN_BD, REQ_GET_HP_CONN_STATE, sizeof(hpInfo), (quint8 *)&hpInfo);

}

#endif
