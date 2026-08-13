#include <vector>

#include <gtest/gtest.h>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statistics.hpp"


using namespace bookdb;
////////////////////////////////////////////////////////////////////////
// BookDatabase musts
////////////////////////////////////////////////////////////////////////

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

TEST(BookDatabaseTest, AuthorDontDuplicateANDClear) {
    BookDatabase db;
    db.EmplaceBack("George Orwell", "1984", 1949, "Fiction", 4.8, 500);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, "Fiction", 4.7, 300);
    
    EXPECT_EQ(db.size(), 2);
    EXPECT_FALSE(db.empty());
    
    EXPECT_EQ(db.GetAuthors().size(), 1);
    db.Clear();
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);
}


TEST(BookDatabaseTest, FormattingAndConcepts) {
    Book b("Isaac Asimov", "Foundation", 1951, Genre::SciFi, 4.9, 800);
    std::string formatted = std::format("{}", b);
    EXPECT_NE(formatted.find("Author: Isaac Asimov"), std::string::npos);
    EXPECT_NE(formatted.find("Genre: SciFi"), std::string::npos);
    bool is_valid_container = BookContainerLike<std::vector<Book>>;
    EXPECT_TRUE(is_valid_container);
}



////////////////////////////////////////////////////////////////////////////////////
// SampleRandomBooks
////////////////////////////////////////////////////////////////////////////////////

TEST(SampleRandomBooksTest, Reqsted11EmptyDatabase) {
    BookDatabase<std::vector<Book>> db;
    // Вызываем выборку 11 книг из абсолютно пустой базы
    auto result = sampleRandomBooks(db, 11);
    
    // Ожидаем, что результат будет пустым, программа не упадет и не уйдет в бесконечный цикл
    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.size(), 0);
}


TEST(SampleRandomBooksTest, MoreThanSize) {
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("Author 1", "Book 1", 2001, Genre::Fiction, 4.5, 10);
    db.EmplaceBack("Author 2", "Book 2", 2002, Genre::SciFi, 4.6, 20);
    db.EmplaceBack("Author 3", "Book 3", 2003, Genre::Mystery, 4.7, 30);
    
    ASSERT_EQ(db.size(), 3);
    auto result = sampleRandomBooks(db, 10);
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].get().year, 2001);
}

TEST(SampleRandomBooksTest, Get99from100) {
    BookDatabase<std::vector<Book>> db;
    for (int i = 0; i < 100; ++i) {
        db.EmplaceBack("Author","Title " + std::to_string(i), 
            1900 + i,Genre::Fiction, 4.0, i
        );
    }
    ASSERT_EQ(db.size(), 100);
    auto result = sampleRandomBooks(db, 99);
    EXPECT_EQ(result.size(), 99);
}
