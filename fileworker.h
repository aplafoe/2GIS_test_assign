#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "wordcount.h"

#include <boost/container/static_vector.hpp>

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QTime>

inline constexpr std::size_t REQUIRED_TOP_SIZE = 15u;

struct Rate {
    explicit Rate(const QString& word, std::uint64_t count, std::float_t processed);

    const QString& word;
    std::uint64_t count = 0;
    std::float_t processedPercent = 0;
};

class FileWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileWorker(QObject* parent = nullptr);
public slots:
    void doWork(const QString& fileName);
signals:
    void resultReady(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top);
    void topUpdated(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& top);
    void openError();
private:
    bool compareAndRedraw(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& currentTop) noexcept;
    WordCountContainer container;
    boost::container::static_vector<Rate, REQUIRED_TOP_SIZE> previousTop;
    QTime lastEmit;
};

#endif // FILEWORKER_H
