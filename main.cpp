#include <QCoreApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QTextStream>
#include <QQmlContext>
#include <QObject>
#include <QQuickItem>
#include <QObject>
#include <boost/container/static_vector.hpp>

#include "executor.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Executor ex;

    const QUrl url(u"qrc:/FilesWordRating/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("cppBackend", &ex);
    engine.load(url);

    auto root = engine.rootObjects();

    auto* r = root.first()->findChild<QObject*>("metaFileDialog");

    auto* re = root.first()->findChild<QObject*>("metaReceiver");

    QObject::connect(r, SIGNAL(fileDialogAccepted(QString)), &ex, SLOT(receiveFileName(QString)));

    QObject::connect(&ex, &Executor::sendTop, re, [&re](const QVariantList& list) {
        QMetaObject::invokeMethod(re, "myQmlFunction", Q_ARG(QVariant, QVariant::fromValue(list))); });

    return app.exec();
}
