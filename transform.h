#ifndef transform_h
#define transform_h

#include <type_traits>
#include <tuple>
#include "append.h"
#include "either_is_int.h"
#include "filler.h"
#include "list_library.h"


// transform
namespace detail {
template <template <typename...> typename, typename...>
struct transform_impl;

template <template <typename...> typename F, typename... Ls>
using transform = typename transform_impl<F, empty<Ls...>, Ls...>::type;

template <template <typename...> typename F, typename... Ls>
struct transform_impl<F, std::true_type, Ls...> {
    using type = list<>;
};

template <template <typename...> typename F, typename... Ls>
struct transform_impl<F, std::false_type, Ls...> {
    using first = F<front<Ls>...>;
    using first_packed = assign<front<list<Ls...>>, list<first>>; // replace list<first> to L<first>
    using type = append<first_packed, transform<F, pop_front<Ls>...>>;
};
}; /* detail */

template <template <typename...> typename F, typename... Ls>
using transform = typename detail::transform<F, Ls...>;


/*
 * As Szilard pointed out, one could define transform_impl's recursion like this to be simpler:
 *
 
 template <template <typename...> typename F, typename... Ls>
 struct transform_impl<F, std::false_type, Ls...> {
 private:
     using first = F<front<Ls>...>;
     using rest = transform<F, pop_front<Ls>...>;
 public:
     using type = push_front<rest, first>;
 };
 
 *
 * Great! ;-)
 */


void test_transform() {
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
    
    using input4_1 = list<int, char, double>;
    using input4_2 = std::tuple<int*, char*, double*>;
    using input4_3 = std::tuple<int**, char**, double**>;
    using input4_4 = std::tuple<int***, char***, double***>;
    using expected4 = list<std::true_type, std::false_type, std::false_type>;
    using result4 = transform<either_is_int, input4_1, input4_2, input4_3, input4_4>;
    static_assert(std::is_same_v<result4, expected4>);
    
    static_assert(std::is_same_v<transform<std::pair>, list<>>);
}

#endif /* transform_h */
