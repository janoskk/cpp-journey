#ifndef has_property_h
#define has_property_h

#include <type_traits>

template <template <typename...> typename, typename...>
struct has_property_s;

template<template <typename...> typename T, typename... U>
inline constexpr bool has_property = has_property_s<T, U...>::value;

template<template <typename...> typename T>
struct has_property_s<T> : std::true_type {};

template<template <typename...> typename T, typename U, typename... V1>
struct has_property_s<T, U, V1...> {
    static constexpr bool value = T<U>::value && has_property<T, V1...>;
};


void test_has_property() {
    static_assert(has_property<std::is_integral, int> == true);
    static_assert(has_property<std::is_integral, int, char> == true);
    static_assert(has_property<std::is_floating_point, double, double, const float> == true);

    static_assert(has_property<std::is_integral, std::tuple<int>> == false);
    static_assert(has_property<std::is_integral, int, std::tuple<int>> == false);
    static_assert(has_property<std::is_integral, std::tuple<int>, int> == false);
    static_assert(has_property<std::is_integral, int, std::tuple<int>, char> == false);
}

#endif /* has_property_h */
