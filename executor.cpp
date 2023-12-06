#include "executor.h"
#include <QDebug>

Executor::Executor(QObject *parent)
    : QObject{parent}
{
    FileWorker* worker = new FileWorker;
    worker->moveToThread(&thread);
    connect(&thread, &QThread::finished, worker, &FileWorker::deleteLater);
    connect(this, &Executor::start, worker, &FileWorker::doWork);
    connect(worker, &FileWorker::topUpdated, this, &Executor::receiveTop);
    connect(worker, &FileWorker::resultReady, this, &Executor::receiveTop);
    thread.start();
}

void Executor::receiveFileName(const QString& fileName) {
    qDebug() << fileName;
    emit start(fileName);
}

void Executor::receiveTop(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top) {
    QVariantList message;
    std::transform(top.begin(), top.end(), std::back_insert_iterator{message},
        [](const Rate& lhs) {
            QVariantMap map;
            map["count"] = lhs.count;
            map["word"] = lhs.word;
            map["percent"] = lhs.processedPercent;
            return map;
    });
    emit sendTop(message);
}

Executor::~Executor() {
    thread.quit();
    thread.wait();
}
