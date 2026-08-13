#include <gtest/gtest.h>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statistics.hpp"

using namespace bookdb;
// Тест 1: Проверка инициализации, добавления и стабильности ссылок авторов
TEST(BookDatabaseTest, PushBackOne) {
    BookDatabase db;
    // Добавляем книгу со строкой, которая будет уничтожена сразу после вызова
    ASSERT_EQ(db.size(), 0);
    {
        std::string temporary_author = "J.K. Rowling";
        db.PushBack(Book(temporary_author, "Harry Potter", 1997, Genre::SciFi, 4.9, 1000));
    } // temporary_author выходит из области видимости и уничтожается
    
    ASSERT_EQ(db.size(), 1);
    // Проверяем, что string_view жива и указывает на стабильный адрес внутри базы данных
    EXPECT_EQ(db.GetBooks().front().author, "J.K. Rowling");
    EXPECT_EQ(db.GetAuthors().size(), 1);
}
// Тест 2: Проверка метода EmplaceBack и методов контейнерного API
TEST(BookDatabaseTest, AuthorDontDuplicateANDClear) {
    BookDatabase db;
    db.EmplaceBack("George Orwell", "1984", 1949, "Fiction", 4.8, 500);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, "Fiction", 4.7, 300);
    
    EXPECT_EQ(db.size(), 2);
    EXPECT_FALSE(db.empty());
    
    // Проверяем, что автор "George Orwell" задублировался в памяти только 1 раз
    EXPECT_EQ(db.GetAuthors().size(), 1);
    
    db.Clear();
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);
}
// Тест 3: Проверка std::formatter для Book и концептов
TEST(BookDatabaseTest, FormattingAndConcepts) {
    Book b("Isaac Asimov", "Foundation", 1951, Genre::SciFi, 4.9, 800);
    
    // Проверяем работу форматтера
    std::string formatted = std::format("{}", b);
    EXPECT_NE(formatted.find("Author: Isaac Asimov"), std::string::npos);
    EXPECT_NE(formatted.find("Genre: SciFi"), std::string::npos);
    
    // Проверяем концепт на нашем контейнере по умолчанию
    bool is_valid_container = BookContainerLike<std::vector<Book>>;
    EXPECT_TRUE(is_valid_container);
}


