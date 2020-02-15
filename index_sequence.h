#ifndef index_sequence_h
#define index_sequence_h

#include <type_traits>
#include <stddef.h>
#include "list_library.h"

template <size_t...>
struct index_sequence;

// make_index_sequence
namespace detail {
template<size_t i, size_t ... js>
struct make_index_sequence_impl {
    using type = typename make_index_sequence_impl<i-1, i-1, js...>::type;
};

template<size_t... js>
struct make_index_sequence_impl<0, js...> {
    using type = index_sequence<js...>;
};
}; /* detail */

template<size_t i>
using make_index_sequence = typename detail::make_index_sequence_impl<i>::type;


//
// First version - from_sequence using recursion
//
namespace v1 {
namespace detail {
template <typename>
struct from_sequence_impl;

template <template <typename TT, TT...> typename T, typename TT, TT Value, TT... Values>
struct from_sequence_impl<T<TT, Value, Values...>> {
    
    using type = append<list<std::integral_constant<TT, Value>>, typename from_sequence_impl<T<TT, Values...>>::type>;
};

template <template <typename TT, TT...> typename T, typename TT>
struct from_sequence_impl<T<TT>> {
    using type = list<>;
};
}; /* detail */

template<typename T>
using from_sequence = typename detail::from_sequence_impl<T>::type;
}; /* v1 */


//
// Second version - from_sequence using pack expansion
//
namespace detail {
template <typename>
struct from_sequence_impl;

template <template <typename TT, TT...> typename T, typename TT, TT... Values>
struct from_sequence_impl<T<TT, Values...>> {
    using type = list<std::integral_constant<TT, Values>...>;
};

template <template <typename TT, TT...> typename T, typename TT>
struct from_sequence_impl<T<TT>> {
    using type = list<>;
};
}; /* detail */

template<typename T>
using from_sequence = typename detail::from_sequence_impl<T>::type;


void test_index_sequence() {
    static_assert(std::is_same_v<make_index_sequence<0>, index_sequence<>>);
    static_assert(std::is_same_v<make_index_sequence<5>, index_sequence<0, 1, 2, 3, 4>>);
    
    static_assert(
        std::is_same_v<
            from_sequence<std::index_sequence<0, 1, 2>>,
            list<std::integral_constant<std::size_t, 0>, std::integral_constant<std::size_t, 1>, std::integral_constant<std::size_t, 2>>
        >
    );
    static_assert(
        std::is_same_v<
            from_sequence<std::integer_sequence<int, 0, 1, 2>>,
            list<std::integral_constant<int, 0>, std::integral_constant<int, 1>, std::integral_constant<int, 2>>
        >
    );
}

#endif /* index_sequence_h */
