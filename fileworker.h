#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "wordcount.h"

#include <boost/container/static_vector.hpp>

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QThread>
#include <QUrl>

inline constexpr std::uint16_t TOP_SIZE = 15u;

struct Rate {
    QString& word;
    std::uint64_t count = 0;
};

class FileWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileWorker(QObject* parent = nullptr);
    bool drawRequired() const noexcept;
public slots:
    void doWork(const QString& fileName);
signals:
    void resultReady(const boost::container::static_vector<Rate, TOP_SIZE>& top);
    void openError();
private:
    WordCountContainer container;
    boost::container::static_vector<WordCount, TOP_SIZE> previousTop;
    boost::container::static_vector<Rate, TOP_SIZE> currentTop;
};

#endif // FILEWORKER_H
