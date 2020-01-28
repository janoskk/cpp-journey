#ifndef either_is_int_h
#define either_is_int_h

#include <type_traits>

namespace either_is_int_v1 {
template <typename...>
struct either_is_int_s;

template<typename... T1>
using either_is_int = typename either_is_int_s<T1...>::type;

template<>
struct either_is_int_s<> {
    using type = std::false_type;
};

template<typename T, typename... U1>
struct either_is_int_s<T, U1...> {
    using type = std::conditional_t<std::is_same_v<T, int>, std::true_type, typename either_is_int_s<U1...>::type>;
};
};


template<typename... T1>
using either_is_int = std::bool_constant<(std::is_same_v<T1, int> || ...)>;


void test_either_is_int() {
    static_assert(std::is_same_v<either_is_int<>, std::false_type>);

    static_assert(std::is_same_v<either_is_int<float>, std::false_type>);
    static_assert(std::is_same_v<either_is_int<int>, std::true_type>);

    static_assert(std::is_same_v<either_is_int<float, char>, std::false_type>);
    static_assert(std::is_same_v<either_is_int<int, char>, std::true_type>);
    static_assert(std::is_same_v<either_is_int<char, int>, std::true_type>);
    static_assert(std::is_same_v<either_is_int<int, int>, std::true_type>);
    
    static_assert(std::is_same_v<either_is_int<float, char, int>, std::true_type>);
    static_assert(std::is_same_v<either_is_int<float, int, char>, std::true_type>);
    static_assert(std::is_same_v<either_is_int<int, float, char>, std::true_type>);
}

#endif /* either_is_int_h */
