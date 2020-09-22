#ifndef tuple_cat_2_h
#define tuple_cat_2_h

#include <type_traits>
#include <tuple>
#include "list_library.h"
#include "index_sequence.h"
#include "tuple_cat.h" // for the tuple printer


template <size_t...>
struct index_sequence;

// make_constant_sequence
namespace detail {
template<size_t i, size_t j, size_t... js>
struct make_constant_sequence_impl {
    using type = typename make_constant_sequence_impl<i-1, j, j, js...>::type;
};

template<size_t j, size_t... js>
struct make_constant_sequence_impl<0, j, js...> {
    using type = index_sequence<js...>;
};
}; /* detail */

template<size_t i, size_t j>
using make_constant_sequence = typename detail::make_constant_sequence_impl<i, j>::type;



template <typename T, std::size_t N>
auto repeat(T t)
{
    return repeat(t, t, N-1);
}

struct end_of_list;


auto my_tuple_cat_2() /* -> decltype(std::tuple()) */
{
    return std::tuple{};
}

template <typename T>
auto my_tuple_cat_2(T t)
{
    return t;
}


template <size_t N, typename T, typename... Us>
struct double_index
{
    using others = double_index<N+1, Us...>;
    using first =  append<from_sequence<std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>>, typename others::first>;
    using second = append<from_sequence<make_constant_sequence<std::tuple_size_v<std::decay_t<T>>, N>>, typename others::second>;
};

template <size_t N, typename T>
struct double_index<N, T, end_of_list>
{
    using first = from_sequence<std::make_index_sequence<std::tuple_size_v<T>>>;
    using second = from_sequence<make_constant_sequence<std::tuple_size_v<T>, N>>;
};

template <typename return_type, typename... Us>
auto foo(Us... us)
{
    
}

template <typename R, typename... Ts, typename... Us, typename Tuples>
R tuple_cat_helper(list<Ts...>, list<Us...>, Tuples tuples)
{
    return R{std::get<Ts::value>(std::get<Us::value>(tuples))...};
}

template <typename... Ts>
auto my_tuple_cat_2(Ts... ts)
{
    using doubles = double_index<0, Ts..., end_of_list>;
    //static_assert(std::is_same_v<typename doubles::first, int>);
    //static_assert(std::is_same_v<typename doubles::second, int>);

    auto ts_union = std::forward_as_tuple(ts...);
    //static_assert(std::is_same_v<decltype(ts_union), int>);
    
    using return_type = append<Ts...>;
    // static_assert(std::is_same_v<return_type, int>);
    
    return tuple_cat_helper<return_type>(typename doubles::first(), typename doubles::second(), ts_union);
}



#define print(x)
#define my_tuple_cat my_tuple_cat_2

void test_tuple_cat_2() {
    static_assert(std::is_same_v<decltype(std::tuple_cat()), decltype(my_tuple_cat())>);
    
    auto expected0 = std::tuple_cat(std::tuple{}, std::tuple{});
    auto result0 = my_tuple_cat(std::tuple{}, std::tuple{});
    
    static_assert(std::is_same_v<decltype(expected0), decltype(result0)>);
    print(expected0);
    print(result0);
    
    auto input1_1 = std::make_tuple(1, "foo", 2, "bar");
    auto expected1 = std::tuple_cat(input1_1);
    auto result1 = my_tuple_cat(input1_1);
    
    static_assert(std::is_same_v<decltype(expected1), decltype(result1)>);
    print(expected1);
    print(result1);

    auto input2_1 = std::make_tuple(1, "foo", 2, "bar");
    auto input2_2 = std::make_tuple("asd", 3.14);
    auto expected2 = std::tuple_cat(input2_1, input2_2);
    auto result2 = my_tuple_cat(input2_1, input2_2);
    
    static_assert(std::is_same_v<decltype(expected2), decltype(result2)>);
    print(expected2);
    print(result2);
    
    auto input3_1 = std::make_tuple(1, "foo", 2, "bar", "xxx");
    auto input3_2 = std::make_tuple("asd", 3.14);
    auto input3_3 = std::make_tuple("asdfg", 3.1415, 5);
    auto expected3 = std::tuple_cat(input3_1, input3_2, input3_3);
    auto result3 = my_tuple_cat(input3_1, input3_2, input3_3);
    
    static_assert(std::is_same_v<decltype(expected3), decltype(result3)>);
    print(expected3);
    print(result3);
}

#endif /* tuple_cat_2_h */
