#include "useriface.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQuick/QQuickView>

#include <QQmlComponent>
#include <QQmlContext>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QDebug>


static QObject *UserInstance(QQmlEngine *, QJSEngine *);

#if defined(KZ_UI)
//#include <QGuiApplication>.
#include <QFontDatabase>

void set_font()
{
    QStringList flist; flist.clear();
    QFontDatabase font_family;
    flist += ":/images/res/font/NotoSans-Regular.ttf";
    foreach(QString fname, flist) {
        QString::number(font_family.addApplicationFont(fname));
    }
    QFont ft("Noto Sans", QFont::Thin);
    ft.setPixelSize(10);
    QGuiApplication::setFont(ft);
}
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    UserIface *userif = new UserIface;

    qmlRegisterSingletonType<UserIface>("useriface", 1, 0, "UserIface", UserInstance);

    QObject::connect(&app, SIGNAL(aboutToQuit()), userif, SLOT(closing()));

#if defined(KZ_UI)
    set_font();
#endif
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/RoSE.qml")));
    return app.exec();
}

static QObject *UserInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    qDebug() << "Creating";
    UserIface *userif = UserIface::getInstance();

    return userif;
}
