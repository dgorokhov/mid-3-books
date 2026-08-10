#pragma once

#include <concepts>
#include <iterator>
#include <utility>
#include "book.hpp"

namespace bookdb {

template <typename T>
concept Container = requires(T loc) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;

    // Проверяем обычные методы
    { loc.begin() } -> std::input_or_output_iterator;
    { loc.end() }   -> std::sentinel_for<typename T::iterator>;
    { loc.size() }  -> std::integral;
    
    // Проверяем константные методы (важно для ревью Яндекса!)
    { std::as_const(loc).begin() } -> std::input_or_output_iterator;
    { std::as_const(loc).end() }   -> std::sentinel_for<typename T::const_iterator>;
};

template <typename T>
concept BookIterator = true;

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = true;

template <typename C>
concept BookComparator = true;

}  // namespace bookdb