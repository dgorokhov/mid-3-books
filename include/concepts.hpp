#pragma once

#include <concepts>
#include <iterator>
#include <utility>
#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T loc) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;

    // Проверяем обычные методы
    { loc.begin() } -> std::input_or_output_iterator;
    { loc.end() }   -> std::sentinel_for<typename T::iterator>;
    { loc.size() }  -> std::integral;
    
    { std::as_const(loc).begin() } -> std::input_or_output_iterator;
    { std::as_const(loc).end() }   -> std::sentinel_for<typename T::const_iterator>;
};

template <typename T>
concept BookIterator = std::forward_iterator<T> && 
    std::convertible_to<typename std::iterator_traits<T>::value_type, Book>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book&>;

template <typename C>
concept BookComparator = std::relation<C, const Book&, const Book&>;

}  // namespace bookdb