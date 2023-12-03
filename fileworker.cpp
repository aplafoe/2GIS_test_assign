#include "fileworker.h"

FileWorker::FileWorker(QObject *parent)
    : QObject{parent} {}

bool FileWorker::drawRequired() const noexcept {
    for (std::uint16_t i = 0; i < TOP_SIZE; ++i) {
        if (currentTop[i].word != previousTop[i].word) {
            return false;
        }
    }
    return true;
}

void FileWorker::doWork(const QString& fileName) {
    QFile file(fileName);

    auto& byWordIndex = container.get<ByWord>();
    auto& byCountIndex = container.get<ByCount>();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString word = stream.readLine();
            if (auto iter = container.find(word); iter != container.end()) {
                auto countIter = container.project<ByCount>(iter);
                byCountIndex.modify_key(countIter, [](std::uint64_t& count) {++count;});
            } else {
                byWordIndex.insert(WordCount{word, 1});
            }
        }
    } else {
        emit openError();
    }
}
