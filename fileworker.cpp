#include "fileworker.h"

#include <QDebug>
#include <QThread>

//Rate::Rate(const QString& word, std::uint64_t count, std::uint64_t processed) :
//    word{word}, count{count}, processedPercent {processed}{}

Rate::Rate(const QString& word, std::uint64_t count, std::float_t processed) :
    word{word}, count {count}, processedPercent{processed} {}

FileWorker::FileWorker(QObject *parent)
    : QObject{parent}, lastEmit {QTime::currentTime()} {}

bool FileWorker::compareAndRedraw(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& currentTop) noexcept {
    for (std::size_t i = 0u; i < std::min(currentTop.size(), previousTop.size()); ++i) {
        if (currentTop.at(i).word != previousTop.at(i).word) {
            //qDebug() << "not eq" << currentTop.at(i).word << currentTop.at(i).count << previousTop.at(i).word << previousTop.at(i).count;
            return true;
        }
    }
    previousTop.clear();
    std::copy(currentTop.begin(), currentTop.end(), std::back_inserter(previousTop));
    return false;
}

void FileWorker::doWork(const QString& fileName) {
    qDebug() << "doWork thread" << QThread::currentThreadId();
    QFile file(fileName);

    auto& byWordIndex = container.get<ByWord>();
    auto& byCountIndex = container.get<ByCount>();
    std::uint64_t count = 0;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        boost::container::static_vector<Rate, REQUIRED_TOP_SIZE> currentTop;
        while (!stream.atEnd()) {
            ++count;
            currentTop.clear();
            QString word = stream.readLine();
            if (auto iter = container.find(word); iter != container.end()) {
                auto countIter = container.project<ByCount>(iter);
                byCountIndex.modify_key(countIter, [](std::uint64_t& count) {++count;});
            } else {
                byWordIndex.insert(WordCount{word, 1});
            }
            auto& top = container.get<ByCount>();
            std::transform(top.begin(), std::next(top.begin(), std::min(top.size(), REQUIRED_TOP_SIZE)), std::back_insert_iterator{currentTop},
                [&file] (const WordCount& lhs) {
                               return Rate{lhs.word, lhs.count, static_cast<float>(file.pos()) * 100 / static_cast<float>(file.size())};
            });
            if (compareAndRedraw(currentTop) && std::abs(QTime::currentTime().msec() - lastEmit.msec()) >= 200) {
                lastEmit = QTime::currentTime();
                emit topUpdated(currentTop);
            }
        }
        emit topUpdated(currentTop);
    } else {
        emit openError();
    }
}
