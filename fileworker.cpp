#include "fileworker.h"

#include <QDebug>

Rate::Rate(const QString& word, std::uint64_t count) : word{word}, count{count} {}

FileWorker::FileWorker(QObject *parent)
    : QObject{parent} {}

bool FileWorker::compareAndRedraw(const boost::container::static_vector<Rate, REQUIRED_TOP_SIZE>& currentTop) noexcept {
    for (std::size_t i = 0u; i < std::min(currentTop.size(), previousTop.size()); ++i) {
        if (currentTop.at(i).word != previousTop.at(i).word) {
            return true;
        }
    }
    previousTop.clear();
    std::copy(currentTop.begin(), currentTop.end(), std::back_inserter(previousTop));
    return false;
}

void FileWorker::doWork(const QString& fileName) {
    QFile file(fileName);

    auto& byWordIndex = container.get<ByWord>();
    auto& byCountIndex = container.get<ByCount>();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            boost::container::static_vector<Rate, REQUIRED_TOP_SIZE> currentTop;
            QString word = stream.readLine();
            if (auto iter = container.find(word); iter != container.end()) {
                auto countIter = container.project<ByCount>(iter);
                byCountIndex.modify_key(countIter, [](std::uint64_t& count) {++count;});
            } else {
                byWordIndex.insert(WordCount{word, 1});
            }
            auto& top = container.get<ByCount>();
            qDebug() << top.size();
            std::transform(top.begin(), std::next(top.begin(), std::min(top.size(), REQUIRED_TOP_SIZE)), std::back_insert_iterator{currentTop},
                           [](const WordCount& lhs) {
                                return Rate{lhs.word, lhs.count};
                            });
            if (compareAndRedraw(currentTop)) {
                qDebug() << "Signal emitted!";
                emit topUpdated(currentTop);
            }
        }
    } else {
        emit openError();
    }
}
