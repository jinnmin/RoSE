#include "gui.h"
#include "useriface.h"

GUI *GUI::__gui = NULL;
GUI* GUI::get_instance()
{
    if(__gui == NULL)
    {
        __gui = new GUI();
        Q_ASSERT(__gui != NULL);
//        __gui->__qml = __gui->rootContext();
//        Q_ASSERT(__gui->__qml != NULL);
    }
    return __gui;
}

/*
QObject *GUI::get_root()
{
    if(this->__obj == NULL) __obj = rootObjects().first();

    return __obj;
}
*/

GUI::GUI(QObject *parent) : QObject(parent) // QQmlApplicationEngine(parent)
{
    __qml = NULL;
    __obj = NULL;
}

/**
 * @author Hyomin Yang
 * @date 2016-11-21
 * @brief GUI::setProperty
 * @param name 변수 이름 (QString)
 * @param val 값
 */
void GUI::setProperty(QString name, QString val)
{
//    LOG()<<"SET UI ENV: "<<name<<" - "<<val;
    QString __name = "__" + name;
    __qml->setContextProperty(__name, val);
}

/**
 * @author Hyomin Yang
 * @date 2016-11-21
 * @brief GUI::setProperty
 * @param name 변수 이름 (bool)
 * @param val 값
 */
void GUI::setProperty(QString name, bool val)
{
//    LOG()<<"SET UI ENV: "<<name<<" - "<<val;
    QString __name = "__" + name;
    __qml->setContextProperty(__name, val);
}

/**
 * @author Hyomin Yang
 * @date 2016-11-21
 * @brief GUI::setProperty
 * @param name 변수 이름 (int)
 * @param val 값
 */
void GUI::setProperty(QString name, int val)
{
//    LOG()<<"SET UI ENV: "<<name<<" - "<<QString::number(val);
    QString __name = "__" + name;
    __qml->setContextProperty(__name, val);
}

/**
 * @author Hyomin Yang
 * @date 2016-11-21
 * @brief GUI::setProperty
 * @param name 변수 이름 (QObject)
 * @param val 값
 */
void GUI::setProperty(QString name, QObject *val)
{
//    LOG()<<"SET UI ENV: "<<name<<" - "<<QString::number(val);
    QString __name = "__" + name;
    __qml->setContextProperty(__name, val);
}

/**
 * @author Hyomin Yang
 * @date 2016-11-21
 * @brief GUI::setProperty
 * @param name 변수 이름 (QObject)
 * @param val 값
 */
void GUI::setModel(QString name, QObject *val)
{
//    LOG()<<"SET UI ENV: "<<name<<" - "<<QString::number(val);
    __qml->setContextProperty(name, val);
}

/**
 * @author Hyomin Yang
 * @date 2016-11-21
 * @brief GUI::registPngImageFile
 * 해당 이미지 파일의 경로와 이름을 등록 한다
 * @param base_path 이미지 파일 경로
 * @param name UI 에서 사용할 이미지 이름
 */
void GUI::registPngImageFile(QString &base_path, QString name)
{
    setProperty(name, base_path + "/" + name + ".png");
}
