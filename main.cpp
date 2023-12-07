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

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/FilesWordRating/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    Executor ex;
    engine.rootContext()->setContextProperty("cppBackend", &ex);
    engine.load(url);

    auto root = engine.rootObjects();
    auto* fileDialogItem = root.first()->findChild<QObject*>("metaFileDialog");
    auto* signalsReceiver = root.first()->findChild<QObject*>("metaReceiver");

    QObject::connect(fileDialogItem, SIGNAL(fileDialogAccepted(QString)), &ex, SLOT(setFileName(QString)));
    QObject::connect(&ex, &Executor::sendTop, signalsReceiver , [&signalsReceiver ](const QVariantList& list) {
        QMetaObject::invokeMethod(signalsReceiver, "drawChart", Q_ARG(QVariant, QVariant::fromValue(list))); });
    QObject::connect(&ex, &Executor::sendError, signalsReceiver , [&signalsReceiver ](const QString& error) {
        QMetaObject::invokeMethod(signalsReceiver, "showError", Q_ARG(QString, error)); });

    return app.exec();
}
