#ifndef transform_h
#define transform_h

#include <type_traits>
#include <tuple>
#include <initializer_list>
#include "append.h"
#include "either_is_int.h"
#include "filler.h"

//
// Some helper types:
// - front        Returns the first type of the list
// - pop_front    Returns the input list without the fist type
// - push_front   Returns the concatenate of the first type and a list
// - assign       Returns the first list with the second list's types
// - clear        Same as assign<L, list<>>
// - (is_)empty   Returns std::true_type if at least one list parameter is not empty
//


// front
namespace detail {
template <typename>
struct front_impl;

template <template <typename ...> typename L, typename T, typename... Ts>
struct front_impl<L<T, Ts...>> {
    using type = T;
};
}; /* detail */

template <typename L>
using front = typename detail::front_impl<L>::type;


// pop_front
namespace detail {
template <typename>
struct pop_front_impl;

template <template <typename...> typename L, typename T, typename... Ts>
struct pop_front_impl<L<T, Ts...>> {
    using type = L<Ts...>;
};
}; /* detail */

template <typename L>
using pop_front = typename detail::pop_front_impl<L>::type;


// push_front
namespace detail {
template <typename, typename...>
struct push_front_impl;

template <template <typename...> typename L, typename... Us, typename... Ts>
struct push_front_impl<L<Ts...>, Us...> {
    using type = L<Us..., Ts...>;
};
}; /* detail */

template <typename L, typename... Ts>
using push_front = typename detail::push_front_impl<L, Ts...>::type;


// assign
namespace detail {
template <typename, typename>
struct assign_impl;

template <template <typename...> typename L, template <typename...> typename M, typename... Ts, typename... Us>
struct assign_impl<L<Ts...>, M<Us...>> {
    using type = L<Us...>;
};
}; /* detail */

template <typename L, typename M>
using assign = typename detail::assign_impl<L, M>::type;


// clear
template <typename L>
using clear = assign<L, list<>>;


// empty or is_empty
namespace detail {
template <typename>
struct empty_item_impl;

template <template <typename...> typename T, typename... Ts>
struct empty_item_impl<T<Ts...>> {
    using type = std::bool_constant<sizeof...(Ts) == 0>;
};
}; /* detail */

template <typename... Ls>
using is_empty = std::bool_constant<(std::is_same_v<typename detail::empty_item_impl<Ls>::type, std::true_type> && ...)>;

template <typename... Ls>
using empty = is_empty<Ls...>;

static_assert(std::is_same_v<empty<>, std::true_type>);
static_assert(std::is_same_v<empty<list<>>, std::true_type>);
static_assert(std::is_same_v<empty<list<>, list<>>, std::true_type>);
static_assert(std::is_same_v<empty<list<int>>, std::false_type>);
static_assert(std::is_same_v<empty<list<>, list<int>>, std::false_type>);


// TODO: implement transform


void test_transform() {
#if 0
    using foo_1 = std::tuple<int, char, double>;
    using foo_2 = std::tuple<int*, char*, double*>;
    using foo_3 = std::tuple<int**, char**, double**>;
    using foo_4 = std::tuple<int***, char***, double***>;

    
    using input1 = std::tuple<int, char, double>;
    using expected1 = std::tuple<int*, char*, double*>;
    using result1 = transform<std::add_pointer_t, input1>;
    
    static_assert(std::is_same_v<result1, expected1>); // ERROR
    

    
    using input2_1 = std::tuple<int, char, double>;
    using input2_2 = std::tuple<char, double, int>;
    using expected2 = std::tuple<std::pair<int, char>, std::pair<char, double>, std::pair<double, int>>;
    using result2 = transform<std::pair, input2_1, input2_2>;
    
    static_assert(std::is_same_v<result2, expected2>);
    
    using input3_1 = list<int, char, double>;
    using input3_2 = std::tuple<char, double, void>;
    using input3_3 = std::tuple<double, int, char>;
    using expected3 = list<std::true_type, std::true_type, std::false_type>;
    using result3 = transform<either_is_int, input3_1, input3_2, input3_3>;
    
    static_assert(std::is_same_v<result3, expected3>);
    
#endif
}

#endif /* transform_h */
