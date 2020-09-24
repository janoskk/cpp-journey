#ifndef tuple_cat_2_h
#define tuple_cat_2_h

#include <type_traits>
#include <tuple>
#include "list_library.h"
#include "index_sequence.h"

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

namespace detail
{
template <size_t N, typename... Us>
struct double_index
{
    using first = list<>;
    using second = list<>;
};

template <size_t N, typename T, typename... Us>
struct double_index<N, T, Us...>
{
    using others = double_index<N+1, Us...>;
    using first =  append<from_sequence<std::make_index_sequence<std::tuple_size_v<T>>>, typename others::first>;
    using second = append<from_sequence<make_constant_sequence<std::tuple_size_v<T>, N>>, typename others::second>;
};

template <typename return_t, typename... Ts, typename... Us, typename Tuples>
return_t tuple_cat_helper(list<Ts...>&&, list<Us...>&&, Tuples&& tuples)
{
    // static_assert(std::is_same_v<decltype(tuples), int>);
    return return_t{std::get<Ts::value>(std::get<Us::value>(std::forward<Tuples>(tuples)))...};
}
} /* detail */

template <typename... Ts, typename return_t = append<std::tuple<>, std::remove_cv_t<std::remove_reference_t<Ts>>...> >
return_t my_tuple_cat_2(Ts&&... ts)
{
    // static_assert(std::is_same_v<return_t, int>);

    using doubles = detail::double_index<0, std::remove_cv_t<std::remove_reference_t<Ts>>...>;
    // static_assert(std::is_same_v<typename doubles::first, int>);
    // static_assert(std::is_same_v<typename doubles::second, int>);

    auto ts_union = std::forward_as_tuple(std::forward<Ts>(ts)...);
    // static_assert(std::is_same_v<decltype(ts_union), int>);
    
    return detail::tuple_cat_helper<return_t>(typename doubles::first{}, typename doubles::second{}, std::move(ts_union));
}


void test_tuple_cat_2() {
    static_assert(std::is_same_v<decltype(std::tuple_cat()), decltype(my_tuple_cat_2())>);

    auto expected0 = std::tuple_cat(std::tuple{}, std::tuple{});
    auto result0 = my_tuple_cat_2(std::tuple{}, std::tuple{});
    
    static_assert(std::is_same_v<decltype(expected0), decltype(result0)>);

    auto const input1_1 = std::make_tuple(1, "foo", 2, "bar");
    auto expected1 = std::tuple_cat(input1_1);
    auto result1 = my_tuple_cat_2(input1_1);
    
    static_assert(std::is_same_v<decltype(expected1), decltype(result1)>);

    auto input2_1 = std::make_tuple(1, "foo", 2, "bar");
    auto input2_2 = std::make_tuple("asd", 3.14);
    auto expected2 = std::tuple_cat(input2_1, input2_2);
    auto result2 = my_tuple_cat_2(input2_1, input2_2);
    
    static_assert(std::is_same_v<decltype(expected2), decltype(result2)>);
    
    auto input3_1 = std::make_tuple(1, "foo", 2, "bar", "xxx");
    auto input3_2 = std::make_tuple("asd", 3.14);
    auto input3_3 = std::make_tuple("asdfg", 3.1415, 5);
    auto expected3 = std::tuple_cat(input3_1, input3_2, input3_3);
    auto result3 = my_tuple_cat_2(input3_1, input3_2, input3_3);
    
    static_assert(std::is_same_v<decltype(expected3), decltype(result3)>);
    
    int i = 123; double d = 3.14;
    std::tuple<int&> input_4_1{i};
    std::tuple<double const&> input_4_2{d};
    auto expected4 = std::tuple_cat(input_4_1, input_4_2);
    auto result4 = my_tuple_cat_2(input_4_1, input_4_2);
    
    static_assert(std::is_same_v<decltype(expected4), decltype(result4)>);
}

#endif /* tuple_cat_2_h */
