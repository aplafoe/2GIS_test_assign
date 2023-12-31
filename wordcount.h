#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <QHash>
#include <QString>

struct WordCount {
    explicit WordCount(const QString& word, std::uint64_t count);

    std::uint64_t count = 0;
    QString word;
};

struct QStrHasher {
    std::size_t operator()(const QString& v) const noexcept;
};

struct ByCount {};
struct ByWord {};

using WordCountContainer = boost::multi_index::multi_index_container<
    WordCount,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<ByWord>, boost::multi_index::key<&WordCount::word>, QStrHasher>,
                boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<ByCount>, boost::multi_index::key<&WordCount::count>, std::greater<std::uint64_t>
            >
        >
    >;

#endif // WORDCOUNT_H
