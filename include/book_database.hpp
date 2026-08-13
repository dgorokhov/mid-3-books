#pragma once

#include "concepts.hpp"
#include <format>
#include <initializer_list>
#include <string>
#include <string_view>
#include <set>
#include <vector>
#include <unordered_set>
#include <stdexcept>

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

    // Выбираем стабильный хэш-сет для уникальных авторов с поддержкой гетерогенного поиска
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
    const_iterator cbegin() const noexcept { return books_.cbegin(); }
    const_iterator cend() const noexcept { return books_.cend(); }

    size_type size() const noexcept { return books_.size(); }
    bool empty() const noexcept { return books_.empty(); }

    // Безопасный доступ к внутреннему состоянию
    const BookContainer& GetBooks() const noexcept { return books_; }
    const AuthorContainer& GetAuthors() const noexcept { return authors_; }

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
    // 1. Метод поиска: включает BookPredicate
    template <typename Predicate>
    requires BookPredicate<Predicate>
    void FindBooks(Predicate pred) {
        // Код поиска
    }

    // 2. Метод сортировки: включает BookComparator
    template <typename Comparator>
    requires BookComparator<Comparator>
    void SortBooks(Comparator comp) {
        // Код сортировки
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
    
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

template <>
struct formatter<std::vector<std::pair<std::string_view, size_t>>> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const std::vector<std::pair<std::string_view, size_t>> &hist, FormatContext &fc) const {
        for (const auto& pair : hist) {
           format_to(fc.out(), "Author {} : {}\n", pair.first, pair.second);
        } 
    }
};

template <>
struct formatter<std::vector<std::pair<bookdb::Genre, double>>> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const std::vector<std::pair<bookdb::Genre, double>> &hist, FormatContext &fc) const {
        for (const auto& pair : hist) {
           format_to(fc.out(), "Author {} : {}\n", pair.first, pair.second);
        } 
    }
};

}
/*namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        /*
        Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        (size/begin/...)

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

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

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
*/
