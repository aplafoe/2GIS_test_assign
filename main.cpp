#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QTextStream>

#include "wordcount.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QFile file("file.txt");

    WordCountContainer container;

    auto& byWordIndex = container.get<ByWord>();
    auto& byCountIndex = container.get<ByCount>();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString s = stream.readLine();
            if (auto iter = container.find(s); iter != container.end()) {
                auto countIter = container.project<ByCount>(iter);
                byCountIndex.modify_key(countIter, [](std::uint64_t& count) {++count;});
            } else {
                byWordIndex.insert(WordCount{s, 1});
            }
        }
    } else {
        qDebug() << "Error";
    }

    std::array<const QString*, 15> topWordsArray;

    auto& byCount = container.get<ByCount>();
    auto end = byCount.begin();
    std::advance(end, std::min(15, static_cast<int>(byCount.size())));

    int i = 0;
    for (auto it = byCount.begin(); it != end; ++it) {
        topWordsArray[i++] = &(it->word);
    }

    for (const auto& wordPtr : topWordsArray) {
        qDebug() << *wordPtr << ": " << container.find(*wordPtr)->count << '\n';
    }


    /*QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/FilesWordRating/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);*/

    return app.exec();
}
