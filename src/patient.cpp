#include "patient.h"

Patient::Patient(QObject *parent) : QAbstractListModel(parent)
{
    m_rolename[P_ID] = "__person_id";
    m_rolename[P_NUM] = "__person_num"; // SKW 2019-04-16 num(시술기록번호), feedback 추가
    m_rolename[P_REG_DATE] = "__person_reg_Date";
    m_rolename[P_LAST_DATE] = "__person_last_date";
    m_rolename[P_FIRST_NAME] = "__person_first_name";
    m_rolename[P_LAST_NAME] = "__person_last_name";
    m_rolename[P_GENDER] = "__person_gender";
    m_rolename[P_BIRTH] = "__person_birth";
    m_rolename[P_HEIGHT] = "__person_height";
    m_rolename[P_WEIGHT] = "__person_weight";

    // SKW 2019-02-13 treatment history
    m_rolename[P_HP] = "__person_person_hp";
    m_rolename[P_FACE] = "__person_face";
    m_rolename[P_RMPININFO] = "__person_rmPinInfo";
    m_rolename[P_PSOPMODE] = "__person_psOpMode";
    m_rolename[P_POWER] = "__person_power";
    m_rolename[P_REPETITION] = "__person_repetition";
    m_rolename[P_DEPTH] = "__person_depth";
    m_rolename[P_ONTIME] = "__person_ontime";
    m_rolename[P_DELAY] = "__person_delay";
    m_rolename[P_ALARM] = "__person_alarm";
    m_rolename[P_ALARM_MAX] = "__person_alarm_max";
    m_rolename[P_USEDSHOT] = "__person_usedShot";     // SKW 2019-02-25 shotCount
    m_rolename[P_USEDTIME] = "__person_usedTime";     // SKW 2019-02-25 shotCount
    m_rolename[P_FEEDBACK] = "__person_usedTime";     // SKW 2019-04-16 num(시술기록번호), feedback 추가
}

QHash<int, QByteArray> Patient::roleNames() const
{
    return m_rolename;
}

int Patient::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_patient_list.count();
}

patient_element * Patient::get_patient_slot()
{
    patient_element *tmp;
    if(m_patient_pool.count() == 0) {
        m_patient_pool.append(new patient_element());
    }
    tmp = m_patient_pool[0];
    m_patient_pool.removeAt(0);
    return tmp;
}

void Patient::release_patient_slot(patient_element *obj)
{
    if(obj == NULL) return;
    m_patient_pool.append(obj);
}

void Patient::clear()
{
    if(m_patient_list.count() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_patient_list.count() - 1);
        foreach(patient_element *p, m_patient_list) {
            release_patient_slot(p);
        }
        m_patient_list.clear();
        endRemoveRows();
    }
}

QVariant Patient::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 ||
        index.row() >= m_patient_list.count()) { return QVariant(); }

    patient_element *p = m_patient_list[index.row()];
    switch(role) {
    case P_ID: return p->id;
    case P_NUM: return p->num; // SKW 2019-04-16 num(시술기록번호), feedback 추가
    case P_REG_DATE: return p->reg_date;
    case P_LAST_DATE: return p->last_date;
    case P_FIRST_NAME: return p->first_name;
    case P_LAST_NAME: return p->last_name;
    case P_GENDER: return p->gender == 0?
                    QString("FEMALE") : QString("MALE");
    case P_BIRTH: return p->birth;
    case P_HEIGHT: return p->height;
    case P_WEIGHT: return p->weight;

        // SKW 2019-02-13 treatment history
    case P_HP: return p->hp;
    case P_FACE: return p->face;
    case P_RMPININFO: return p->rmPinInfo;
    case P_PSOPMODE: return p->psOpMode;
    case P_POWER: return p->power;
    case P_REPETITION: return p->repetition;
    case P_DEPTH: return p->depth;
    case P_ONTIME: return p->ontime;
    case P_DELAY: return p->delay;
    case P_ALARM: return p->alarm;
    case P_ALARM_MAX: return p->alarm_max;
    case P_USEDSHOT: return p->usedShot; // SKW 2019-02-25 shotCount
    case P_USEDTIME: return p->usedTime; // SKW 2019-02-25 shotCount
    case P_FEEDBACK: return p->feedback; // SKW 2019-04-16 num(시술기록번호), feedback 추가
    }

    return QVariant();
}

void Patient::patient_listup(QSqlQuery *qry)
{
    if(qry == NULL) {
        qDebug() <<"Query Result is null.";
        return;
    }
    clear();
    while(qry->next())
    {
        patient_element *p = get_patient_slot();
        m_patient_list.append(p);
        p->id = qry->record().value("id").toInt();
        p->num = qry->record().value("num").toInt(); // SKW 2019-04-16 num(시술기록번호), feedback 추가
        p->gender = qry->record().value("gender").toInt();
        p->reg_date = qry->record().value("reg_date").toString();
        p->last_date = qry->record().value("last_date").toString();
        p->first_name = qry->record().value("first_name").toString();
        p->last_name = qry->record().value("last_name").toString();
        p->birth = qry->record().value("birth").toString();
        p->height = qry->record().value("height").toString();
        p->weight = qry->record().value("weight").toString();

        // SKW 2019-02-13 treatment history
        p->hp = qry->record().value("hp").toString();
        p->face = qry->record().value("face").toString();
        p->rmPinInfo = qry->record().value("rmPinInfo").toString();
        p->psOpMode = qry->record().value("psOpMode").toString();
        p->power = qry->record().value("power").toInt();
        p->repetition = qry->record().value("repetition").toInt();
        p->depth = qry->record().value("depth").toInt();
        p->ontime = qry->record().value("ontime").toInt();
        p->delay = qry->record().value("delay").toInt();
        p->alarm = qry->record().value("alarm").toInt();
        p->alarm_max = qry->record().value("alarm_max").toInt();
        p->usedShot = qry->record().value("usedShot").toInt();   //SKW 2019-02-25  used shot count
        p->usedTime = qry->record().value("usedTime").toInt();   //SKW 2019-02-25  used shot count
        p->feedback = qry->record().value("feedback").toInt();   // SKW 2019-04-16 num(시술기록번호), feedback 추가
    }
    int count = m_patient_list.count();
    qDebug() << __func__ << " count : " << count;
    if(count > 0) {
        beginInsertRows(QModelIndex(), 0, count-1); // parameter is index baundary
        endInsertRows();
    }
}
