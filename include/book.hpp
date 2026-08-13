#pragma once

#include <format>
#include <stdexcept>
#include <string_view>
#include <array>
namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

// constexpr преобразование строк в Genre
constexpr Genre GenreFromString(std::string_view s) {
    if (s == "Fiction")    return Genre::Fiction;
    if (s == "NonFiction") return Genre::NonFiction;
    if (s == "SciFi")      return Genre::SciFi;
    if (s == "Biography")  return Genre::Biography;
    if (s == "Mystery")    return Genre::Mystery;
    return Genre::Unknown;
}

constexpr std::string_view StringFromGenre(Genre g) {
    static constexpr std::array<std::string_view, 6> genreStrings
        {"Fiction",   "NonFiction", "SciFi",
        "Biography", "Mystery",    "Unknown"};
    return genreStrings[static_cast<int>(g)];
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    // Ваш код для конструкторов здесь
        // 1. Конструктор, принимающий Genre как enum
    constexpr Book(std::string_view auth, std::string t, int y, Genre g, double r, int rc)
        : author(auth)
        , title(std::move(t))
        , year(y)
        , genre(g)
        , rating(r)
        , read_count(rc) {}

    // 2. Конструктор, принимающий Genre в виде строки (std::string_view)
    constexpr Book(std::string_view auth, std::string t, int y, std::string_view g_str, double r, int rc)
        : author(auth)
        , title(std::move(t))
        , year(y)
        , genre(GenreFromString(g_str))
        , rating(r)
        , read_count(rc) {}
};

}  // namespace bookdb

namespace std {

template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;
        genre_str = StringFromGenre(g);
        return format_to(fc.out(), "{}", genre_str);
    }
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Реализация std::formatter для класса Book
template <>
struct formatter<bookdb::Book, char> {

    template <typename FormatContext>
    auto format(const bookdb::Book& b, FormatContext &fc) const {
        // Форматируем книгу в удобном и понятном виде
        return format_to(fc.out(), 
            "Author: {}, Title: {}, Year: {}, Genre: {}, Rating: {:.1f}, Read Count: {}", 
            b.author, b.title, b.year, b.genre, b.rating, b.read_count);
    }
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin(); 
    }

};

}  // namespace std
