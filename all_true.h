#ifndef all_true_h
#define all_true_h

#include <type_traits>

//
// First version - using inheritance and checking whether the `has_false` member is defined or not.
//
namespace detail_v1 {
template <bool...>
struct bool_pack;

template <>
struct bool_pack<> {};

template <bool... Bools>
struct bool_pack<false, Bools...>
{
    void has_false();
};

template <bool... Bools>
struct bool_pack<true, Bools...> : public bool_pack<Bools...> { };

template <typename T>
struct has_false
{
    template <typename U>
    static auto check(U const*) -> decltype(&U::has_false, std::false_type{});
    
    template <typename>
    static auto check(...) -> std::true_type;
    
    static constexpr bool value = decltype(check<T>(0))::value;
};

template <bool... bools>
struct all_true_impl {
    
    using type = std::bool_constant<has_false<bool_pack<bools...>>::value>;
};
}; /* detail_v1 */

template <bool... bools>
using all_true_v1 = typename detail_v1::all_true_impl<bools...>::type;


//
// Second version - I found a way to generate a bool_pack that is all true and has the same length
// (without recursion and C++17 features)
//
namespace detail {
template <bool...>
struct bool_pack;
}; /* detail */

template <bool... bools>
using all_true = typename std::is_same<detail::bool_pack<true || bools...>, detail::bool_pack<bools...>>::type;


void test_all_true() {
    static_assert(std::is_same_v<all_true<true>, std::true_type>);
    static_assert(std::is_same_v<all_true<true, true>, std::true_type>);
    static_assert(std::is_same_v<all_true<true, true, true>, std::true_type>);
    static_assert(std::is_same_v<all_true<true, true, true, true>, std::true_type>);

    static_assert(std::is_same_v<all_true<false>, std::false_type>);
    static_assert(std::is_same_v<all_true<false>, std::false_type>);
    static_assert(std::is_same_v<all_true<true, false>, std::false_type>);
    static_assert(std::is_same_v<all_true<false, true, true>, std::false_type>);
    static_assert(std::is_same_v<all_true<true, false, true>, std::false_type>);
    static_assert(std::is_same_v<all_true<true, true, false>, std::false_type>);
}

#endif /* all_true_h */
