
#include <algorithm>
#include <print> // Обязательно добавляем для std::print

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statistics.hpp"

using namespace bookdb;

int main(int argc, const char** argv) {
    // Create a book database
    BookDatabase<std::vector<Book>> db;

    // Add some books (исправлен порядок: сначала автор, потом название)
    db.EmplaceBack("George Orwell", "1984", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Harper Lee", "To Kill a Mockingbird", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("J.D. Salinger", "The Catcher in the Rye", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Charlotte Brontë", "Jane Eyre", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("William Golding", "Lord of the Flies", 1954, Genre::Fiction, 4.2, 89);
    
    std::print("Books: {}\n\n", db);

    // Sorts
    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    std::print("Books sorted by author: {}", db);

    std::print ("\n\n==================\n");
    std::sort(db.begin(), db.end(), comp::LessByRating{});
    std::print("Books sorted by popularity: {}", db);

    std::print ("\n\n==================\n");
    auto avrRating = calculateAverageRating(db);
    std::print("Average books rating in library: {}\n", avrRating);

   // Author histogram
    std::print ("\n\n==================\n");
    auto histogram = buildAuthorHistogramFlat(db);
    // Оборачиваем в AuthorHistogramView для кастомного форматирования
    std::print("Author histogram:\n{}", bookdb::AuthorHistogramView{histogram});

    // Ratings
    std::print ("\n\n==================\n");
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    std ::print("\n\nAverage ratings by genres: {}\n", bookdb::GenreHistogramView{.data=genreRatings});

    
    // Filters
    auto filtered = filterBooks(db, any_of(YearBetween(1960, 1999), RatingAbove(4.6)));
    std::print("\n\nBooks from the 20th century with rating ≥ 4.4:\n");
    std::for_each(filtered.cbegin(), filtered.cend(), [](const auto &v) { std::print("{}\n", v.get()); });

    auto filtered2 = filterBooks(db, RatingAbove(4.5));
    std::print("\n\nBooks from the 20th century with rating ≥ 4.5:\n");
    std::for_each(filtered2.cbegin(), filtered2.cend(), [](const auto &v) { std::print("{}\n", v.get()); });

    // Top 3 books
    // getTopNBy отбирает по любому компаратору
    auto topBooks = getTopNBy(db, 3, comp::LessByRating{});
    std::print("\n\nTop 3 books by rating:\n");
    std::for_each(topBooks.cbegin(), topBooks.cend(), [](const auto &v) { std::print("{}\n", v.get()); });

    auto orwellBookIt = std::find_if(db.begin(), db.end(), 
                            [](const auto &v) 
                            { return v.author == "George Orwell"; 
                            });
    if (orwellBookIt != db.end()) {
        std::print("\n\nTransparent lookup by authors. Found Orwell's book: {}\n", *orwellBookIt);
    }

    return 0;
}


