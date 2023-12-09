#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "wordcount.h"

#include <boost/container/static_vector.hpp>

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QTextStream>
#include <QTime>

inline constexpr std::size_t REQUIRED_TOP_SIZE = 15u;

enum class ThreadState {
    Processing,
    Paused,
    NoWork
};

struct Rate {
    explicit Rate(const QString& word, std::uint64_t count, std::float_t processed);

    std::uint64_t count = 0;
    std::float_t processedPercent = 0;
    const QString& word;
};

class FileWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileWorker(QObject* parent = nullptr);
    void pauseThread() noexcept;
    void resumeThread() noexcept;
    void cancelReading() noexcept;
public slots:
    void doWork(const QString& fileName);
signals:
    void openError(const QString& error);
    void resultReady(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top);
    void topUpdated(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top);
private:
    bool compareAndRedraw(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& currentTop) noexcept;

    WordCountContainer container;
    ThreadState currentState;
    QMutex mutex;
    QTime lastEmit;
    boost::container::static_vector<Rate, REQUIRED_TOP_SIZE> previousTop;
};

#endif // FILEWORKER_H
