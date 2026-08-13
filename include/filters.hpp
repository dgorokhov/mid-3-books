#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "concepts.hpp"
#include "book_database.hpp"

namespace bookdb {


// Функция возвращает ЛЯМБДУ (которая и является объектом-предикатом)
constexpr inline auto YearBetween(int from, int to) {
    return [from, to](const Book& book) -> bool {
        return book.year >= from && book.year <= to;
    };
}

constexpr inline auto RatingAbove(double min_rating) {
    return [min_rating](const Book& book) -> bool {
        return (book.rating >= min_rating);
    };
}

constexpr inline auto GenreIs(Genre genre) {
    return [genre](const Book& book) -> bool {
        return (book.genre == genre);
    };
}

// Фильтрация по любому кастомному предикату, удовлетворяющему концепту BookPredicate
template <BookContainerLike T, BookPredicate Predicate>
auto filterBooks(const BookDatabase<T>& db, Predicate pred) {
    std::vector<std::reference_wrapper<const Book>> filtered;
    for (const auto& book : db.GetBooks()) {
        if (pred(book)) {
            filtered.push_back(std::cref(book));
        }
    }
    return filtered;
}   
}// namespace bookdb