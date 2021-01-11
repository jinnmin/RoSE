#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtNetwork>

#include "websocketthread.h"
//#include "Const.h"
//#include "qfitwindow.h"

#define TIMER_TICKS   10000
#define TEMP_TIMER_TICKS 5000

int currTemperature = 20;
bool startTemplog = false;
bool sendTotalLogFlag = false;
int remainReqCnt = 0;

WebSocketThread::WebSocketThread(QObject *parent) : QThread(parent)
    , m_webSocket(NULL)
    , m_runFlag(true)
    , m_limitedRun(5)
{

    // 관리서버의 주소를 가져온다
//    m_serverURL = "bluecore2012-001-site2.gtempurl.com";
//    m_serverURL = "ws://wsapitest.azurewebsites.net";
    m_serverURL = "ws://testhiroapi.aidsoft.io";

    m_recvServerAlive.start();

    //m_mainWindow = (QFitWindow *)parent;
    //connect(m_mainWindow, &QFitWindow::sendOnlineData,       this, &WebSocketThread::onSendOnlineData);
    //connect(m_mainWindow, &QFitWindow::webUpdateAccept,      this, &WebSocketThread::onUpdateAccept);
    //connect(m_mainWindow, &QFitWindow::webUpdateDeny,        this, &WebSocketThread::onUpdateDeny);
    //connect(m_mainWindow, &QFitWindow::webUpdateProgressing, this, &WebSocketThread::onUpdateProgressing);
    //connect(m_mainWindow, &QFitWindow::webUpdateComplete,    this, &WebSocketThread::onUpdateComplete);
}

void WebSocketThread::close()
{
   qDebug() << " 111 WebSocketThread::close() m_limitedRun: " << m_limitedRun ;
    m_limitedRun = 0;
    m_runFlag = false;
}

void WebSocketThread::rerun()
{
   qDebug() << " 111 WebSocketThread::rerun() m_limitedRun: " << m_limitedRun ;
    m_limitedRun = 10000;
    m_runFlag = true;
}

void WebSocketThread::run()
{
    m_webSocket     = new QWebSocket();

    connect(m_webSocket, &QWebSocket::connected,           this, &WebSocketThread::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected,        this, &WebSocketThread::onDisconnected);
    connect(m_webSocket, &QWebSocket::stateChanged,        this, &WebSocketThread::onStateChanged);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketThread::onTextMessageReceived);

    qRegisterMetaType<QAbstractSocket::SocketState>();
    qRegisterMetaType<QAbstractSocket::SocketError>();

    QElapsedTimer timer;
    timer.start();

    while (m_runFlag)
    {
        qApp->processEvents();
        QThread::msleep(500); // 200


        if((sendTotalLogFlag == false) && (m_webSocket->state() == QAbstractSocket::ConnectedState)) {
            sendTotalLogFlag = true;
            m_runFlag = false;
            sendTotalLog();
        }

        if ((startTemplog == false) && (timer.elapsed() >= TIMER_TICKS))
        {
            timer.restart();

            // server에 alive 메시지를 보낸다.
            sendAliveMessage();

//            qDebug() << "m_recvServerAlive.elapsed(): "<<  m_recvServerAlive.elapsed();

            qDebug() << " 0000 m_webSocket->state() " << m_webSocket->state() ;

            if (m_recvServerAlive.elapsed() > 20000)
            {
                m_webSocket->close();
                m_recvServerAlive.restart();
            }

            if (m_webSocket->state() == QAbstractSocket::UnconnectedState)
            {
                qDebug() << "1111 m_webSocket->state() " << m_webSocket->state() ;
                requestConnect();
                continue;
            }
            else if (m_webSocket->state() != QAbstractSocket::ConnectedState)
                continue;

        }

        if (m_tempTimer.elapsed() >= TEMP_TIMER_TICKS)
        {
            if (m_webSocket->state() == QAbstractSocket::ConnectedState)
            {

                if( remainReqCnt > 0 ) {
                    m_tempTimer.restart();
                    remainReqCnt --;
                    sendReqData();
                }
                else {
                    qDebug() << "**** remainReqCnt: " << remainReqCnt;
                    remainReqCnt = 0;
                }

    //                sendLogs();

            }
        }

    }

    emit finished();

    delete m_webSocket;
}


bool WebSocketThread::sendAliveMessage()
{

    qDebug() <<  "[ m_serialNo:  "<<  m_serialNo << "]"  ;

    if (m_webSocket && m_webSocket->isValid() && m_webSocket->state() == QAbstractSocket::ConnectedState)
    {
        QString msg = "{ \"CMD\":\"NPING\", \"SERIAL\":\"" + m_serialNo + "\", \"KEY\":\"HIKEY001\" }";

        int sent = m_webSocket->sendTextMessage(msg);

        qDebug() << "[" + QTime::currentTime().toString() + "] " + msg + " m_limitedRun: " + m_limitedRun;

        m_limitedRun --;

        if(m_limitedRun == 0)
            m_runFlag = false;

        emit outputDebugMessage("Alive(" + msg + ")");
        return (msg.length() == sent);
    }
    else
    {
        return false;
    }
}

void WebSocketThread::requestConnect()
{
//    QString url = QString("ws://%1/WebsockServer.ashx?type=nd&id=%2&model=%3&ver=%4").
//            arg(m_serverURL).arg(m_serialNo).arg("HLaser").arg("0.0.1");
//            arg(m_serverURL).arg(m_serialNo).arg(MODEL_NAME).arg(VERSION_SHORT);

//    QString url;
//    url.sprintf("ws://%s/WebsockServer.ashx?type=d&id=%s&model=QFit&ver=%s",
//                m_serverURL.toStdString().c_str(),
//                m_serialNo.toStdString().c_str(),
//                VERSION_SHORT);
//    QString url = QString("ws://wsapitest.azurewebsites.net");
    QString url = QString("ws://testhiroapi.aidsoft.io");

    qDebug() << "Connecting..." << url;
    emit outputDebugMessage("Connecting..." + url);

    m_webSocket->open(QUrl(url));
}

bool WebSocketThread::sendLogs()
{

    if (m_webSocket && m_webSocket->isValid() && m_webSocket->state() == QAbstractSocket::ConnectedState)
    {
        QString msg = "{ \"CMD\":\"NPING\", \"SERIAL\":\"" + m_serialNo + "\", \"KEY\":\"HIKEY001\" }";
        int sent = m_webSocket->sendTextMessage(msg);

        qDebug() << "[" + QTime::currentTime().toString() + "] " + msg;
        emit outputDebugMessage("Alive(" + msg + ")");

        return (msg.length() == sent);
    }
    else
    {
        return false;
    }
}

bool WebSocketThread::sendError()
{
    QDateTime dt = QDateTime::currentDateTime();

    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            QVariantMap data;
            data["CMD"]    = "NERR_DATA";
            data["SERIAL"]    = m_serialNo;
            data["TIME"]    =  dt.toString("yyyy-MM-dd hh:mm:ss");
            data["ERRCODE"] = 222;
            data["ERRORMSG"] = "RM HP DISCONNECTED";
            data["KEY"]    = "HIKEY001";

            QJsonDocument doc(QJsonObject::fromVariantMap(data));
            QString json(doc.toJson(QJsonDocument::Compact));

            qDebug() << json;
            m_webSocket->sendTextMessage(json);

            emit outputDebugMessage("ERROR_SEND");

        }
    }
    catch (int exception)
    {
        qDebug() << "Exception on onUpdateDeny() : " << exception;
    }

    return true;
}

bool WebSocketThread::sendTotalLog()
{
    QDateTime dt = QDateTime::currentDateTime();

    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            QVariantMap data;
            data["CMD"]    = "NREQ_LOG";
            data["SERIAL"]    = m_serialNo;
            data["TIME"]    =  dt.toString("yyyy-MM-dd hh:mm:ss");
            data["FILE"] = m_totalLogFile;
            data["KEY"]    = "HIKEY001";

            QJsonDocument doc(QJsonObject::fromVariantMap(data));
            QString json(doc.toJson(QJsonDocument::Compact));

            qDebug() << json;
            m_webSocket->sendTextMessage(json);

            emit outputDebugMessage("TOTAL_LOG_SEND");

        }
    }
    catch (int exception)
    {
        qDebug() << "Exception on onUpdateDeny() : " << exception;
    }

    return true;
}

int WebSocketThread::checkTemperature(){
    //DATA
    float check_temperature;
    int curr_temp;


    QString cmd = "/home/pi/hironic/voltcheck.sh";
    QProcess process;
    process.start("sh", QStringList() << "-c" << cmd);
    process.waitForFinished();

    QString output(process.readAllStandardOutput());

    QStringList output2 = output.split( QRegExp("[\n]") );

    check_temperature = output2[0].toFloat();
    curr_temp = (int) check_temperature;

    return curr_temp;

}

bool WebSocketThread::sendReqData()
{
    QDateTime dt = QDateTime::currentDateTime();

    currTemperature = checkTemperature();

    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            QVariantMap data;
            data["CMD"]    = "NREQ_DATA";
            data["RET"]    = 0;
            data["SERIAL"]    = m_serialNo;
            data["TIME"]    = dt.toString("yyyy-MM-dd hh:mm:ss");
            data["HP"] = "RM_Auto";
            data["FACE"] = "Nose";
            data["RMPININFO"] = "RM HP 49Pin";
            data["PSOPMODE"] = "0";
            data["POWER"] = 70;
            data["REPETITION"] = 20;
            data["DEPTH"] = 20;
            data["ONTIME"] = 50;
            data["DELAY"] = 150;
            data["USEDSHOT"] = 250;
            data["USEDTIME"] = 0;
            data["TEMPERATURE"] = currTemperature;
            data["KEY"]    = "HIKEY001";

            QJsonDocument doc(QJsonObject::fromVariantMap(data));
            QString json(doc.toJson(QJsonDocument::Compact));

            qDebug() << json;
            m_webSocket->sendTextMessage(json);

            emit outputDebugMessage("ERROR_SEND");

        }

    }
    catch (int exception)
    {
        qDebug() << "Exception on sendReqData() : " << exception;
    }

    return true;
}

void WebSocketThread::reconnect()
{

}

void WebSocketThread::onConnected()
{
    qDebug() << "WebSocket connected...";
    emit outputDebugMessage("WebSocket connected...");
}

void WebSocketThread::onDisconnected()
{
    qDebug() << "WebSocket disconnected...";
    emit outputDebugMessage("WebSocket disconnected...");
}

void WebSocketThread::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "WebSocket state changed..." << state;
    emit outputDebugMessage("WebSocket state changed..." + state);
}

void WebSocketThread::onTextMessageReceived(const QString &message)
{
    qDebug() << "WebSocket text message received..." << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (doc.object().empty())
    {
        qDebug() << "Invalid JSON format!!";
        return;
    }

    const   QJsonObject &obj = doc.object();
    QString cmd = obj["CMD"].toString();


    qDebug() << "WebSocket text message received... cmd: " << cmd;

    if (cmd == "NREQ_DATA")
    {
        if (obj.contains("PERIOD")) {
            remainReqCnt = obj["PERIOD"].toInt();

        qDebug() << "WebSocket text message received... remainReqCnt: " << remainReqCnt;
            remainReqCnt = (remainReqCnt*1000)/TEMP_TIMER_TICKS;
            qDebug() << "NERR_DATA  m_tempTimer.start() remainReqCnt: " << remainReqCnt ;
            m_tempTimer.start();
        }
//        emit outputDebugMessage(" NERR_DATA Connected with server.(NERR_DATA)");
    }
    else if (cmd == "NUPDATE")
    {
        if (obj.contains("VERSION") && obj.contains("PATH"))
            emit runUpdateSoftware(obj["VERSION"].toString(), obj["PATH"].toString());
    }
    else if (cmd == "NCTRL_READY")
    {
        emit runControlReady();
    }
    else if (cmd == "NCTRL_STANDBY")
    {
        emit runControlStandby();
    }
    else if (cmd == "NCTRL_START")
    {
        if (obj.contains("MODE") && obj.contains("FREQUENCY") && obj.contains("FLUENCE") && obj.contains("SPOT"))
            emit runControlStart(obj["MODE"].toString(), obj["FREQUENCY"].toInt(), obj["FLUENCE"].toInt(), obj["SPOT"].toInt());
    }
    else if (cmd == "NCTRL_STOP")
    {
        emit runControlStop();
    }
    else if (cmd == "NPING")
    {
        m_recvServerAlive.restart();
        emit outputDebugMessage("NPING  Connected with server.(PING)");
    }
    else if (cmd == "NERR_DATA")
    {
//        remainReqCnt = (30*1000)/TEMP_TIMER_TICKS;
        qDebug() << "NERR_DATA  m_tempTimer.start() cmd: " << cmd ;
//        m_tempTimer.start();
        emit outputDebugMessage(" NERR_DATA Connected with server.(NERR_DATA)");
    }

}

void WebSocketThread::onSendOnlineData(const QString &json)
{
    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            m_webSocket->sendTextMessage(json);
        }
    }
    catch (int exception)
    {
    }
}

void WebSocketThread::onUpdateAccept()
{
    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            QVariantMap data;
            data["CMD"]    = "NUPDATE";
            data["RET"]    = 0;
            data["SERIAL"] = "000000";
            data["KEY"]    = "HIKEY001";

            QJsonDocument doc(QJsonObject::fromVariantMap(data));
            QString json(doc.toJson(QJsonDocument::Compact));

            m_webSocket->sendTextMessage(json);
        }
    }
    catch (int exception)
    {
        qDebug() << "Exception on onUpdateAccept() : " << exception;
    }
}

void WebSocketThread::onUpdateDeny()
{
    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            QVariantMap data;
            data["CMD"]    = "NUPDATE";
            data["RET"]    = -1;
            data["SERIAL"] = "000000";
            data["KEY"]    = "HIKEY001";

            QJsonDocument doc(QJsonObject::fromVariantMap(data));
            QString json(doc.toJson(QJsonDocument::Compact));

            qDebug() << json;
            m_webSocket->sendTextMessage(json);
        }
    }
    catch (int exception)
    {
        qDebug() << "Exception on onUpdateDeny() : " << exception;
    }
}

void WebSocketThread::onUpdateProgressing(int progress, const QString &version)
{
    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            if (progress < 0)
                progress = 0;
            else if (progress > 100)
                progress = 100;

            QVariantMap data;
            data["CMD"]     = "NUPDATE";
            data["RET"]     = progress;
            data["SERIAL"]  = "000000";
            data["VERSION"] = version;
            data["KEY"]     = "HIKEY001";

            QJsonDocument doc(QJsonObject::fromVariantMap(data));
            QString json(doc.toJson(QJsonDocument::Compact));

//            qDebug() << json;
            m_webSocket->sendTextMessage(json);
        }
    }
    catch (int exception)
    {
//        qDebug() << "Exception on onUpdateProgressing() : " << exception;
    }
}

void WebSocketThread::onUpdateComplete(const QString &version)
{
    try
    {
        if (m_webSocket->state() == QAbstractSocket::ConnectedState)
        {
            QVariantMap data;
            data["CMD"]     = "NUPDATE";
            data["RET"]     = 100;
            data["SERIAL"]  = "000000";
            data["VERSION"] = version;
            data["KEY"]     = "HIKEY001";

            QJsonDocument doc(QJsonObject::fromVariantMap(data));
            QString json(doc.toJson(QJsonDocument::Compact));

//            qDebug() << json;
            m_webSocket->sendTextMessage(json);
        }
    }
    catch (int exception)
    {
//        qDebug() << "Exception on onUpdateComplete() : " << exception;
    }
}

