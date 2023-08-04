
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"

#ifdef Android_Platform
#include <QtCore/private/qandroidextras_p.h>
#endif

int main(int argc, char *argv[])
{

#ifdef Android_Platform
//    // 添加读取权限，Android 6.0 版本后需要申请，不然的话，啥都看不到。
//    QtAndroidPrivate::requestPermission(QtAndroidPrivate::PermissionType::Storage);  // ok
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend",new BackEnd(&app));




    const QUrl url(u"qrc:/telegram/qml/Main.qml"_qs);
    const QStringList Locations = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    const QUrl Downloads =
        QUrl::fromLocalFile(Locations.isEmpty() ?
                                app.applicationDirPath() :
                                Locations.front());
    engine.rootContext()->setContextProperty("path_downloads", Downloads);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);


//    const QUrl url(u"qrc:/template/Main.qml"_qs);


    return app.exec();
}
