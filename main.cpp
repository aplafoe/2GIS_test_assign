#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QTextStream>

#include <boost/container/static_vector.hpp>

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

    boost::container::static_vector<QString, 15> vec;

    auto& top = container.get<ByCount>();

    std::transform(top.begin(), std::next(top.begin(), 15), std::back_insert_iterator{vec}, [](const WordCount& lhs) {return lhs.word;});

    for (const auto & it : vec) {
        qDebug() << it;
    }

    /*auto end = byCount.begin();
    std::advance(end, std::min(15, static_cast<int>(byCount.size())));

    int i = 0;
    for (auto it = byCount.begin(); it != end; ++it) {
        topWordsArray[i++] = &(it->word);
    }

    for (const auto& wordPtr : topWordsArray) {
        qDebug() << *wordPtr << ": " << container.find(*wordPtr)->count << '\n';
    }*/


    /*QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/FilesWordRating/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);*/

    return app.exec();
}
