#pragma once

#include <algorithm>
#include <bitset>
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <random>
#include <string_view>
#include <memory>

#include "book_database.hpp"
#include "concepts.hpp"


namespace bookdb {

/*
=Функция buildAuthorHistogramFlat должна создавать гистограмму количества книг по авторам, используя один из доступных flat-контейнеров.
=Функция calculateGenreRatings должна возвращать средний рейтинг книг по жанрам, используя один или несколько из доступных flat-контейнеров.
=Функция calculateAverageRating должна использовать стандартные алгоритмы для вычисления среднего рейтинга всех книг в библиотеке.
=Функция sampleRandomBooks должна выбирать указанное количество книг из библиотеки и возвращать их в виде std::vector<std::reference_wrapper<const Book>>.
=Функция getTopNBy должна выбирать из библиотеки указанное количество книг c наивысшим рейтингом и возвращать их 
	в виде std::vector<std::reference_wrapper<const Book>>. Это единственная функция, которой разрешено изменять переданный контейнер.
=Все функции, за исключением последней, не должны модифицировать входные параметры.
*/    

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {

    // ключ ==  string_view == авторов базы данных
    //AuthorHistogramView result;
    std::vector<std::pair<std::string_view, size_t>> result;
    // Заранее резервируем память под всех авторов для скорости
    result.reserve(cont.GetAuthors().size());
    for (const auto& author : cont.GetAuthors()) {
        result.emplace_back(author, 0);
    }
    // Проходим по всем книгам базы данных
    for (const auto& book : cont.GetBooks()) {
        auto it = std::find_if(result.begin(), result.end(), 
            [&book](const auto& pair) { return pair.first == book.author; });
        
        if (it != result.end()) {
            it->second++; // Увеличиваем счетчик книг автора
        }
    }
    std::sort(result.begin(), result.end(), [&comp](const auto& lhs, const auto& rhs) {
        return comp(lhs.first, rhs.first);
    });
    return result;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last) {
    // 1. Создаем промежуточную мапу: Ключ — Genre, Значение — пара {сумма_рейтингов, количество_книг}
    std::unordered_map<Genre, std::pair<double, int>> hist;
    
    for (auto it = first; it != last; ++it) {
        // Доступ через точку, так как hist[...] возвращает ссылку на пару, а не указатель
        hist[it->genre].first += it->rating;
        hist[it->genre].second++;
    }

    // 2. Переносим данные в плоский вектор для вычисления среднего и последующей сортировки
    std::vector<std::pair<Genre, double>> genre_rt;
    genre_rt.reserve(hist.size());

    for (const auto& [genre, info] : hist) {
        double avg_rating = (info.second > 0) ? (info.first / info.second) : 0.0;
        genre_rt.emplace_back(genre, avg_rating);
    }

    // 3. Сортируем вектор по рейтингу (например, от большего к меньшему)
    std::sort(genre_rt.begin(), genre_rt.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second > rhs.second; 
    });

    return genre_rt;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books) {
    double sum{};
    for (const auto& book : books) {
        sum += book.rating;
    }
    return (books.size() ? sum/books.size() : 0);
}


template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num) {

    using namespace std;
    //на всякй случай если объект громадный - в куче
    //резервируем место под результат.
    auto random_books = make_unique<vector<std::reference_wrapper<const bookdb::Book>>>() ;
    num = (std::min(cont.size(), num));
    random_books->reserve(num);

    // инициализация // Вихрь Мерсенна
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, num );
    //битовый массив для хранения какой объект уже выделен
    size_t bits_count{};
    std::vector<bool> bits(num, false);

    if (num > cont.size()/2)  { 
    //если вернуь надо больше половины то быстрее и  главное надержнее  пометить те
    //записи кторые будут отброшены т.к. случ. распределение не гарантирует прохд по всем записям    
        num = cont.size() - num; 
        while (bits_count < num) {
            auto bitnum = distrib(gen);
            if (!bits[bitnum]) continue;
            bits[bitnum] = true;
            ++bits_count;
        }
        for (auto i = 0; i < cont.size(); ++i)
            //вернуть сброшенные или все (если cont.size==num)
            if (!bits[i]) random_books->push_back(std::cref(cont[i]));
    }
    else {
        while (bits_count < num) {
            auto bitnum = distrib(gen);
            if (bits[bitnum]) continue;
            bits[bitnum] = true;
            ++bits_count;
            random_books->push_back(std::cref(cont[bitnum]));
        } 
    }
    return std::move(random_books); 

}
  
template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t count, Comparator comp) {

    std::sort(cont.begin(), cont.end(), [&comp](const auto& lhs, const auto& rhs) {
            return comp(lhs.first, rhs.first);
    });
    count = std::min(count, cont.GetBooks().size());
    std::vector<std::reference_wrapper<const bookdb::Book>> result;
    for (size_t i = 0; i < count; ++i) {
        result.emplace_back(std::cref(cont.GetBooks()[i]));
    
    return result;  //RVO used
} 



}  // namespace bookdb
}