#ifndef WIFI_H
#define WIFI_H

#include <QObject>
#include <QThread>

class Wifi : public QObject
{
    Q_OBJECT
public:
    explicit Wifi(QObject *parent = 0);

#if defined (WIFI_TEST)
    QStringList m_scan_list;
public slots:
    Q_INVOKABLE void clear_conf();
    Q_INVOKABLE int scan();
    Q_INVOKABLE QString get_scan_list_at(int);
    Q_INVOKABLE int conn(QString, QString);
    Q_INVOKABLE QString get_address();
#endif
};

#endif // WIFI_H
