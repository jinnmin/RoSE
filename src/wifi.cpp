#include "wifi.h"
#include "useriface.h"
#include "../lib/global.h"
#include <QProcess>
#include <QFile>
#include <QNetworkInterface>

#if defined(WIFI_TEST)
UserIface::Wifi(QObject *parent) : QObject(parent)
{
    m_scan_list.clear();
}
#endif

QString UserIface::get_address()
{
    QList<QNetworkInterface> itf_list = QNetworkInterface::allInterfaces();

    foreach(QNetworkInterface itf, itf_list)
    {
#if defined(KZ_UI_DEBUG)
        qDebug() << __func__ << itf;
#endif
//        log1<<itf;
        if(itf.name() == "wlan0") {
            if(itf.addressEntries().count() < 1) continue;
            if(itf.flags() & QNetworkInterface::IsRunning)
            {
                return QString("%1^ (CONNECTED)").arg(itf.addressEntries().first().ip().toString());
            }
            else
            {
                return QString("%1^ (DISCONNECTED)").arg(itf.addressEntries().first().ip().toString());
            }
        }
        else
        {
            continue;
        }
    }

    return QString("NO^ DEVICE");
}

int UserIface::scan()
{
    QStringList senario;
    senario.clear();
    senario.append("sudo wpa_cli scan");
    senario.append("sudo wpa_cli scan_results");

    m_scan_list.clear();

#if defined(KZ_UI_DEBUG)
        qDebug() << __func__  << " in Scan function";
#endif

    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);

    for(int i=0; i<senario.count();)
    {
        p.start(senario[i]);
        if(p.waitForFinished())
        {
            if(i == 0) { i++; continue; }
            QByteArray r = p.readAll();
            QString s(r);
            m_scan_list = s.split("\n", QString::SkipEmptyParts);
            if(m_scan_list.count() <= 2)
            {
                QThread::msleep(500);
                continue;
            }
            m_scan_list.removeFirst();
            m_scan_list.removeFirst();
            for(int j=0; j<m_scan_list.count(); j++)
            {
                 m_scan_list[j].replace("\t", "^");
            }

#if defined(KZ_UI_DEBUG)
        qDebug() << __func__ << m_scan_list ;
#endif

            emit resScanResult(m_scan_list.count());
            return m_scan_list.count();
        }
        else
        {
//            log1<<"CMD FAILED: "<<m_scan_list[i];
            emit resScanResult(0);
            return 0;
        }
    }
}

QString UserIface::get_scan_list_at(int index)
{
    if(index < 0 || m_scan_list.count() <= index)
        return QString();
    return m_scan_list[index];
}

void UserIface::clear_conf()
{
     log2 << "/home/app/script/wifi.sh CLEAR" << endl;
    QProcess::execute("sudo /home/app/script/wifi.sh CLEAR");
}

int UserIface::conn(QString ssid, QString passwd)
{
    if(passwd == "")
    {
        QProcess::execute(QString("sudo /home/app/script/wifi.sh SET_NOAUTH \"%1\" \"%2\"")
                          .arg(ssid).arg(passwd));
    }
    else
    {
        QProcess::execute(QString("sudo /home/app/script/wifi.sh SET_AUTH \"%1\" \"%2\"")
                          .arg(ssid).arg(passwd));
    }
    return 0;

    /*
country=GB
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1

network={
    ssid="rnd_sw"
    key_mgmt=WPA-PSK
    psk="sw123123!"
}
     */
//    if(index < 0 || m_scan_list.count() <= index)
//        return -99;
    QFile f("/etc/wpa_supplicant/wpa_supplicant.conf");
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Unbuffered) == false)
    {
        log3<<"WIFI^FILE_ERROR";
        return -99;
    }
    QTextStream s(&f);
    s<<"country=GB"<<endl;
    s<<"ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev"<<endl;
    s<<"update_config=1"<<endl;
    s<<"network={"<<endl;
    s<<"ssid=\"";
    s<<ssid;
    s<<"\""<<endl;
    s<<"key_mgmt=WPA-PSK"<<endl;
    s<<"psk=\"";
    s<<passwd;
    s<<"\""<<endl;
    s<<"}"<<endl;
    f.close();
    QProcess::execute("ifdown wlan0");
    QProcess::execute("ifup wlan0");

//    QList<QHostAddress> a = QNetworkInterface::allAddresses();
//    foreach(QHostAddress b, a)
//    {
//        b.
//    }

    return 0;
//    QStringList target = m_scan_list[index].split("^");
//    QString ssid = target[4];

//    QProcess p;
//    p.setProcessChannelMode(QProcess::MergedChannels);

//    QStringList senario;
//    senario.clear();
//    senario.append(QString("sudo /usr/local/app/scripts/wifi.sh %1 %2").arg(ssid).arg("sw123123!"));
//    p.start(senario[0]);
//    if(p.waitForFinished())
//    {
//        QByteArray r = p.readAll();
//        QString s(r);
//        log1<<senario[0];
//        log1<<s;
//        return 0;
//    }
//    return -1;
}
