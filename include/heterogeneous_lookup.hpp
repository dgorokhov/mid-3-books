#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;  // Маркер для ассоциативных контейнеров

    template <typename T, typename U>
    constexpr auto operator()(const T &lhs, const U &rhs) const noexcept {
        return std::string_view(lhs) < std::string_view(rhs);
    }
};

// Прозрачный компаратор "равно"
struct TransparentStringEqual {
    using is_transparent = void;

    template <typename T, typename U>
    constexpr auto operator()(const T &lhs, const U &rhs) const noexcept {
        return std::string_view(lhs) == std::string_view(rhs);
    }
};

// Прозрачный хэш-функтор для std::unordered_set / std::unordered_map
struct TransparentStringHash {
    using is_transparent = void;

    size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv); 
    }

    constexpr size_t operator()(const std::string &s) const noexcept {
        return std::hash<std::string_view>{}(s); 
    }

    constexpr size_t operator()(const char *s) const noexcept {
        return std::hash<std::string_view>{}(s);

    }  // namespace bookdb
}
