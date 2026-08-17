#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "concepts.hpp"
#include "book_database.hpp"

namespace bookdb {


constexpr auto YearAbove(int year) {
    return [year](const Book& b) { return b.year > year; };
}

// Функция возвращает ЛЯМБДУ (которая и является объектом-предикатом)
constexpr auto YearBetween(int from, int to) {
    return [from, to](const Book& book) -> bool {
        return book.year >= from && book.year <= to;
    };
}

constexpr auto RatingAbove(double min_rating) {
    return [min_rating](const Book& book) -> bool {
        return (book.rating >= min_rating);
    };
}

constexpr auto GenreIs(Genre genre) {
    return [genre](const Book& book) -> bool {
        return (book.genre == genre);
    };
}

// Фильтрация по любому кастомному предикату, удовлетворяющему концепту BookPredicate
template <BookContainerLike T, BookPredicate Predicate>
constexpr auto filterBooks(const BookDatabase<T>& db, Predicate pred) {
    std::vector<std::reference_wrapper<const Book>> filtered;
    for (const auto& book : db.GetBooks()) {
        if (pred(book)) {
            filtered.push_back(std::cref(book));
        }
    }
    return filtered;
}   

template <BookPredicate... Preds>
constexpr auto all_of(Preds... preds) {
    // Возвращаем лямбду, которая принимает книгу
    return [=](const Book& book) -> bool {
        return (preds(book) && ...);
    };
}

template <BookPredicate... Preds>
auto any_of(Preds... preds) {
    return [=](const Book& book) -> bool {
        return (preds(book) || ...);
    };
}


}// namespace bookdb