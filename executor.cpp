#include "executor.h"

Executor::Executor(QObject *parent)
    : QObject{parent}
{
    worker = new FileWorker;
    worker->moveToThread(&thread);
    connect(&thread, &QThread::finished, worker, &FileWorker::deleteLater);
    connect(this, &Executor::start, worker, &FileWorker::doWork);
    connect(worker, &FileWorker::topUpdated, this, &Executor::handleTopUpdate);
    connect(worker, &FileWorker::resultReady, this, &Executor::handleTopUpdate);
    connect(worker, &FileWorker::openError, this, &Executor::sendError);
    thread.start();
}

void Executor::setFileName(const QString& fileName) {
    emit start(fileName);
}

void Executor::handleTopUpdate(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top) {
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

void Executor::handleError(const QString& error) {
    emit sendError(error);
}

Executor::~Executor() {
    thread.quit();
    thread.wait();
}
