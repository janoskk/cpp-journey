#ifndef index_sequence_h
#define index_sequence_h

#include <type_traits>
#include <stddef.h>
#include "list_library.h"

template <size_t...>
struct index_sequence;

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


void test_index_sequence() {
    static_assert(std::is_same_v<make_index_sequence<0>, index_sequence<>>);
    static_assert(std::is_same_v<make_index_sequence<5>, index_sequence<0, 1, 2, 3, 4>>);
}

#endif /* index_sequence_h */
