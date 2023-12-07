#include "wordcount.h"

WordCount::WordCount(const QString& word, std::uint64_t count) : word{word}, count{count} {}

std::size_t QStrHasher::operator()(const QString& v) const noexcept {
    return static_cast<std::size_t>(qHash(v));
}

