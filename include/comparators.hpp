#pragma once

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb::comp {
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

struct LessByYear {
    constexpr bool operator()(const Book& lhs, const Book& rhs) const noexcept {
        return lhs.year < rhs.year;
    }
};

struct LessByRating {
    constexpr bool operator()(const Book& lhs, const Book& rhs) const noexcept {
        return lhs.rating < rhs.rating; 
    }
};

struct GreaterByRating {
    constexpr bool operator()(const Book& lhs, const Book& rhs) const noexcept {
        return lhs.rating > rhs.rating; 
    }
};

}  // namespace bookdb::comp