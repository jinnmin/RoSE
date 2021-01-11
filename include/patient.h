#ifndef PATIENT_H
#define PATIENT_H

#include <QObject>
#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include "/home/wind/trunk/lib/global.h"

struct patient_element
{
    int id;
    int num; // SKW 2019-04-16 num(시술기록번호), feedback 추가
    QString reg_date;
    QString last_date;
    QString first_name;
    QString last_name;
    int gender;
    QString birth;
    QString height;
    QString weight;

    // SKW 2019-02-13 treatment history // SKW 2019-02-19 treatment history
    QString hp;
    QString face;
    QString rmPinInfo;
    QString psOpMode;
    int power;
    int repetition;
    int depth;
    int ontime;
    int delay;
    int alarm;
    int alarm_max;
    int usedShot; //SKW 2019-02-25 usedShotCount
    int usedTime; //SKW 2019-02-25 usedShotCount
    int feedback; // SKW 2019-04-16 num(시술기록번호), feedback 추가
};



class Patient : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(PERSON)
public:
    explicit Patient(QObject *parent = 0);
    enum PERSON {
        P_ID = Qt::UserRole + 1,
        P_NUM, // SKW 2019-04-16 num(시술기록번호), feedback 추가
        P_REG_DATE,
        P_LAST_DATE,
        P_FIRST_NAME,
        P_LAST_NAME,
        P_GENDER,
        P_BIRTH,
        P_HEIGHT,
        P_WEIGHT,

        // SKW 2019-02-13 treatment history
        P_HP,
        P_FACE,
        P_RMPININFO,
        P_PSOPMODE,
        P_POWER,
        P_REPETITION,
        P_DEPTH,
        P_ONTIME,
        P_DELAY,
        P_ALARM,
        P_ALARM_MAX,
        P_USEDSHOT,    // SKW 2019-02-25 usedShotCount
        P_USEDTIME,      // SKW 2019-02-25 usedShotCount
        P_FEEDBACK     // SKW 2019-04-16 num(시술기록번호), feedback 추가
    };
    QHash<int, QByteArray> m_rolename;
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void clear();

    void release_patient_slot(patient_element *);
    patient_element* get_patient_slot();
    QList<patient_element *> m_patient_list;
    QList<patient_element *> m_patient_pool;


signals:

public slots:
    void patient_listup(QSqlQuery *);
};


#endif // PATIENT_H
