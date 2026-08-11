#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "concepts.hpp"
#include "book_database.hpp"

namespace bookdb {

// Фильтрация по любому кастомному предикату, удовлетворяющему концепту BookPredicate
template <BookContainerLike T, BookPredicate Predicate>
auto FilterBooks(const BookDatabase<T>& db, Predicate pred) {
    std::vector<std::reference_wrapper<const Book>> filtered;
    for (const auto& book : db.GetBooks()) {
        if (pred(book)) {
            filtered.push_back(std::cref(book));
        }
    }
    return filtered;
}   
}// namespace bookdb