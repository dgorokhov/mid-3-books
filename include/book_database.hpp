#pragma once

#include "concepts.hpp"
#include <format>
#include <initializer_list>
#include <string>
#include <string_view>
#include <set>
#include <vector>
#include <unordered_set>

#include "book.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

    
template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Псевдонимы типов для итераторов и контейнера
    using value_type      = Book;
    using reference       = Book&;
    using const_reference = const Book&;
    using iterator        = typename BookContainer::iterator;
    using const_iterator  = typename BookContainer::const_iterator;
    using size_type       = typename BookContainer::size_type;

    // стабильный хэш-сет для уникальных авторов с поддержкой гетерогенного поиска
    //flat_set, flat_map не подходят
    using AuthorContainer = 
        std::unordered_set<std::string, TransparentStringHash, TransparentStringEqual>;

    BookDatabase() = default;

    // Конструктор из initializer_list
    BookDatabase(std::initializer_list<Book> init) {
        for (const auto& book : init)
            PushBack(book);
    }

    void Clear() noexcept {
        books_.clear();
        authors_.clear();
    }

    // API стандартных контейнеров
    iterator begin() noexcept { return books_.begin(); }
    iterator end() noexcept { return books_.end(); }
    const_iterator begin() const noexcept { return books_.begin(); }
    const_iterator end() const noexcept { return books_.end(); }
    const_iterator cbegin() noexcept { return books_.cbegin(); }
    const_iterator cend()  noexcept { return books_.cend(); }

    size_type size() const noexcept { return books_.size(); }
    bool empty() const noexcept { return books_.empty(); }

    //const BookContainer& GetBooks() const noexcept { return books_; }
    

    // Добавление элементов
    void PushBack(const Book& book) {
        // 1. Вставляем автора в стабильный сет. Если он уже есть, вернется существующий.
        auto [it, inserted] = authors_.insert(std::string(book.author));
        // 2. Создаем копию книги, но ее string_view жестко привязываем к строке внутри authors_
        Book clean_book = book;
        clean_book.author = *it; 
        books_.push_back(std::move(clean_book));
    }

    void PushBack(Book&& book) {
        auto [it, inserted] = authors_.insert(std::string(book.author));
        book.author = *it;
        books_.push_back(std::move(book));
    }

    template <typename... Args>
     requires std::constructible_from<Book, Args...>
        reference EmplaceBack(Args&&... args) {
        // Конструируем временную книгу для извлечения автора
        Book tmp(std::forward<Args>(args)...);
        
        auto [it, inserted] = authors_.insert(std::string(tmp.author));
        tmp.author = *it;
        
        books_.push_back(std::move(tmp));
        return books_.back();
    }

    std::span<const Book> GetBooks() const noexcept {
        // Конструируем span прямо из внутреннего контейнера.
        // std::span автоматически подхватит begin() и size() вашего вектора.
        return std::span<const Book>(books_.data(), books_.size());
    }
    std::span<Book> GetBooks() noexcept {
        return std::span<Book>(books_.data(), books_.size());
    }
    const AuthorContainer& GetAuthors() const noexcept { return authors_; }
    AuthorContainer& GetAuthors() noexcept { return authors_; }

    
    template <typename Predicate>
    requires BookPredicate<Predicate>
    void FindBooks(Predicate pred) {
        // Код поиска
    }

    template <typename Comparator>
    requires BookComparator<Comparator>
    void SortBooks(Comparator comp) {
        // Код сортировки
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

struct AuthorHistogramView {
    const std::vector<std::pair<std::string_view, size_t>>& data;
};

struct GenreHistogramView {
    const std::vector<std::pair<bookdb::Genre, double>>& data;
};

}  // namespace bookdb

namespace std {


template <>
struct formatter<bookdb::AuthorHistogramView, char> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const bookdb::AuthorHistogramView &view, FormatContext &fc) const {
        auto out = fc.out(); // Сохраняем начальный итератор вывода
        
        format_to(fc.out(), "Authors in BookDatabase : (size = {}):\n\n", view.data.size());
        for (const auto& [author, count] : view.data) {
            // Перезаписываем итератор вывода при каждом шаге
            out = format_to(out, "  {} : {}\n", author, count);
        } 
        return out;
    }
};


template <>
struct formatter<bookdb::GenreHistogramView, char> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const bookdb::GenreHistogramView &view, FormatContext &fc) const {
        auto out = fc.out(); // Сохраняем начальный итератор вывода
        
        format_to(fc.out(), "Genres in BookDatabase : (size = {}):\n\n", view.data.size());
        for (const auto& [genre, popularity] : view.data) {
            // Перезаписываем итератор вывода при каждом шаге
            out = format_to(out, "  {} : {}\n", bookdb::StringFromGenre(genre), popularity);
        } 
        return out;
    }
};
    

template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}):\n", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }
};
  // namespace std

}

