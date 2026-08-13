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

    // Правильная проверка: типы из begin/end должны приводиться к итераторам контейнера
    { loc.begin() }                -> std::convertible_to<typename T::iterator>;
    { loc.end() }                  -> std::convertible_to<typename T::iterator>;
    { std::as_const(loc).begin() } -> std::convertible_to<typename T::const_iterator>;
    { std::as_const(loc).end() }   -> std::convertible_to<typename T::const_iterator>;
    
    // Проверка метода очистки
    { loc.clear() } -> std::same_as<void>;
    
    // Проверка, что внутри лежат строго Книги
    requires std::same_as<typename T::value_type, Book>;
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