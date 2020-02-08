#ifndef list_library_h
#define list_library_h

#include "append.h"

//
// Some helper types:
// - front        L<Ts...> -> T                  // Returns the first type of the list
// - pop_front    L<Ts...> -> L<Xs...>           // Returns the input list without the fist type
// - push_front   L<Ts...>, Us... -> L<Xs...>    // Returns the concatenate of the first type and a list
// - push_back    L<Ts...>, Us... -> L<Xs...>    // Returns the concatenate of the list and the first type
// - assign       L<Ts...>, L<Us...> -> L<Xs...> // Returns the first list with the second list's types
// - clear        L<Ts...> -> L<Xs...>           // Same as assign<L, list<>>
// - (is_)empty   Ls... -> bool_constant         // Returns std::true_type if at least one list parameter is not empty
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
template <typename...>
struct push_front_impl;

template <template <typename...> typename L, typename... Us, typename... Ts>
struct push_front_impl<L<Ts...>, Us...> {
    using type = L<Us..., Ts...>;
};
}; /* detail */

template <typename... Ts>
using push_front = typename detail::push_front_impl<Ts...>::type;


// push_back
namespace detail {
template <typename, typename...>
struct push_back_impl;

template <template <typename...> typename L, typename... Us, typename... Ts>
struct push_back_impl<L<Ts...>, Us...> {
    using type = L<Ts..., Us...>;
};
}; /* detail */

template <typename L, typename... Ts>
using push_back = typename detail::push_back_impl<L, Ts...>::type;


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


#endif /* list_library_h */
