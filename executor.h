#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "fileworker.h"

#include <boost/container/static_vector.hpp>

#include <QObject>
#include <QThread>
#include <QVariantList>
#include <QVariantMap>

class Executor : public QObject
{
    Q_OBJECT
public:
    explicit Executor(QObject *parent = nullptr);
    ~Executor();
public slots:
    void receiveTop(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top);
    void receiveFileName(const QString& fileName);
signals:
    void start(const QString& fileName);
    void sendTop(const QVariantList& top);
private:
    QThread thread;
};

#endif // EXECUTOR_H
