#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QTextStream>

#include <boost/container/static_vector.hpp>

#include "fileworker.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    FileWorker f;
    f.doWork("file.txt");


    /*QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/FilesWordRating/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);*/

    return app.exec();
}
