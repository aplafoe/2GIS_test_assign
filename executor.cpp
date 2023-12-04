#include "executor.h"

Executor::Executor(QObject *parent)
    : QObject{parent}
{
    FileWorker* worker = new FileWorker;
    worker->moveToThread(&thread);
    connect(&thread, &QThread::finished, worker, &FileWorker::deleteLater);
    connect(this, &Executor::start, worker, &FileWorker::doWork);
    connect(worker, &FileWorker::topUpdated, this, &Executor::sendTop);
    thread.start();
}

void Executor::receiveFileName(const QString& fileName) {
    emit start(fileName);
}

void Executor::receiveTop(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top) {
    emit sendTop(top);
}

Executor::~Executor() {
    thread.quit();
    thread.wait();
}
