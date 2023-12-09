#include "executor.h"

Executor::Executor(QObject *parent)
    : QObject{parent}
{
    worker = new FileWorker;
    connect(&thread, &QThread::finished, worker, &FileWorker::deleteLater);
    connect(this, &Executor::start, worker, &FileWorker::doWork);
    connect(worker, &FileWorker::topUpdated, this, &Executor::handleTopUpdate);
    connect(worker, &FileWorker::resultReady, this, &Executor::handleTopUpdate);
    connect(worker, &FileWorker::openError, this, &Executor::sendError);
}

void Executor::cancelReading() {
    worker->cancelReading();
}

void Executor::handleError(const QString& error) {
    emit sendError(error);
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

void Executor::setFileName(const QString& fileName) {
    worker->resumeThread();
    worker->moveToThread(&thread);
    thread.start();
    emit start(fileName);
}

void Executor::togglePause(bool flag) {
    if (flag) {
        worker->pauseThread();
    } else {
        worker->resumeThread();
    }
}

Executor::~Executor() {
    thread.quit();
    thread.wait();
}
