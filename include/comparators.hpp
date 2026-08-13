#pragma once

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb::comp {
// Сравнение по автору (гетерогенное)
struct LessByAuthor {
    using is_transparent = void;

    constexpr bool operator()(const Book& lhs, const Book& rhs) const noexcept {
        return lhs.author < rhs.author;
    }
    constexpr bool operator()(const Book& lhs, std::string_view rhs_author) const noexcept {
        return lhs.author < rhs_author;
    }
    constexpr bool operator()(std::string_view lhs_author, const Book& rhs) const noexcept {
        return lhs_author < rhs.author;
    }
};
// Сравнение по названию (гетерогенное)
struct LessByTitle {

    using is_transparent = void;

    constexpr bool operator()(const Book& lhs, const Book& rhs) const noexcept {
        return lhs.title < rhs.title;
    }
    constexpr bool operator()(const Book& lhs, std::string_view rhs_title) const noexcept {
        return lhs.title < rhs_title;
    }
    constexpr bool operator()(std::string_view lhs_title, const Book& rhs) const noexcept {
        return lhs_title < rhs.title;
    }
};
// Сравнение по году (от старых к новым)
struct LessByYear {
    constexpr bool operator()(const Book& lhs, const Book& rhs) const noexcept {
        return lhs.year < rhs.year;
    }
};
// Сравнение по рейтингу (от высшего к низшему)
struct LessByRating {
    constexpr bool operator()(const Book& lhs, const Book& rhs) const noexcept {
        return lhs.rating < rhs.rating; // Обратите внимание: > для сортировки по убыванию
    }
};


}  // namespace bookdb::comp