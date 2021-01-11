#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

class GUI : public  QObject  //QQmlApplicationEngine
{
    Q_OBJECT


public:
    static GUI *get_instance();
    static GUI *__gui;
    QQmlContext *__qml;
    QObject *__obj;
//    QObject* get_root();
    void setModel(QString, QObject *);
    void setProperty(QString, QObject *);
    void setProperty(QString, QString);
    void setProperty(QString, bool);
    void setProperty(QString, int);
    void registPngImageFile(QString &, QString);

    explicit GUI(QObject *parent = 0);

signals:

public slots:

public: // element
};

#endif // GUI_H
