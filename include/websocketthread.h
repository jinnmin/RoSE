#ifndef WEBSOCKETTHREAD_H
#define WEBSOCKETTHREAD_H

#include <QThread>
#include <QtWebSockets/qwebsocket.h>
//#include <QtWebSockets/QtWebSockets>
#include <QTime>
#include "/home/wind/trunk/lib/global.h"

class LogItem; // JOON_TEST RMS_TEST

class WebSocketThread : public QThread
{
    Q_OBJECT
public:
    explicit WebSocketThread(QObject *parent = 0);

    void run();

    void close();

    void rerun();

    void setSerialNo(QString serialNo) { m_serialNo = serialNo; }
    void setTotalLogFile(QString totalLogFile) { m_totalLogFile = totalLogFile; }
    bool webSocketTest() { return m_runFlag; }

protected:
    bool sendAliveMessage();
    void requestConnect();
    bool sendLogs();  // JOON_TEST   RMS_TEST
    bool sendError();  // JOON_TEST   RMS_TEST
    bool sendTotalLog();  // JOON_TEST   RMS_TEST
    bool sendReqData();  // JOON_TEST   RMS_TEST
    int checkTemperature();

signals:
    void finished();
    void runRequestData(int period);
    void runUpdateSoftware(const QString &version, const QString &path);
    void runControlReady();
    void runControlStandby();
    void runControlStart(const QString &mode, int freq, int fluence, int spot);
    void runControlStop();
    void outputDebugMessage(const QString &msg);

public slots:
    void reconnect();
    void onConnected();
    void onDisconnected();
    void onStateChanged(QAbstractSocket::SocketState state);
    void onTextMessageReceived(const QString &message);

    void onSendOnlineData(const QString &json);
    void onUpdateAccept();
    void onUpdateDeny();
    void onUpdateProgressing(int progress, const QString &version);
    void onUpdateComplete(const QString &version);

private:
    QWebSocket     *m_webSocket;
    QList<LogItem>  m_logItems;  // JOON_TEST RMS_TEST
    QString         m_serverURL;
    QString         m_serialNo;
    QString         m_totalLogFile;
    bool            m_runFlag;
    int             m_limitedRun;
    QTime           m_recvServerAlive;
    QTime           m_tempTimer;
};

class LogItem
{
public:
    LogItem(const QDateTime &dateTime, int code, const QString &msg,
            float ktp,  float opt1, float opt2, float water, float power, float air,
            float ptc1, float ptc2, float ptc3, float ptc4,  float ptc5,  float ptc6,
            int spot, int bitStatus, int shotCount, int totalShotCount)
        : m_dateTime(dateTime)
        , m_code(code)
        , m_msg(msg)
        , m_ktp(ktp)
        , m_opt1(opt1)
        , m_opt2(opt2)
        , m_water(water)
        , m_power(power)
        , m_air(air)
        , m_ptc1(ptc1)
        , m_ptc2(ptc2)
        , m_ptc3(ptc3)
        , m_ptc4(ptc4)
        , m_ptc5(ptc5)
        , m_ptc6(ptc6)
        , m_spot(spot)
        , m_bitStatus(bitStatus)
        , m_shotCount(shotCount)
        , m_totalShotCount(totalShotCount)
    {

    }

    QDateTime dateTime()       { return m_dateTime; }
    int       code()           { return m_code; }
    QString   message()        { return m_msg; }
    float     ktp()            { return m_ktp; }
    float     optical1()       { return m_opt1; }
    float     optical2()       { return m_opt2; }
    float     water()          { return m_water; }
    float     power()          { return m_power; }
    float     air()            { return m_air; }
    float     ptc1()           { return m_ptc1; }
    float     ptc2()           { return m_ptc2; }
    float     ptc3()           { return m_ptc3; }
    float     ptc4()           { return m_ptc4; }
    float     ptc5()           { return m_ptc5; }
    float     ptc6()           { return m_ptc6; }
    int       spot()           { return m_spot; }
    int       bitStatus()      { return m_bitStatus; }
    int       shotCount()      { return m_shotCount; }
    int       totalShotCount() { return m_totalShotCount; }

private:
    QDateTime m_dateTime;
    int       m_code;
    QString   m_msg;
    float     m_ktp;
    float     m_opt1;
    float     m_opt2;
    float     m_water;
    float     m_power;
    float     m_air;
    float     m_ptc1;
    float     m_ptc2;
    float     m_ptc3;
    float     m_ptc4;
    float     m_ptc5;
    float     m_ptc6;
    int       m_spot;
    int       m_bitStatus;
    int       m_shotCount;
    int       m_totalShotCount;
};


#endif // WEBSOCKETTHREAD_H

