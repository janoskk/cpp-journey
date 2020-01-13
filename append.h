#ifndef append_h
#define append_h

#include <type_traits>

template <typename...>
struct list;

template <typename...>
struct append_s;

template <typename... T1>
using append = typename append_s<T1...>::type;

// append<>
template<>
struct append_s<> {
    using type = list<>;
};

// append<list<int>>
template<
    template <typename... T1> typename T,
    typename... T1
>
struct append_s<T<T1...>> {
    using type = T<T1...>;
};

// append<list<double>, list<int>, list<int>>
template<
    template <typename... T1> typename T,
    typename... T1,
    template <typename... U1> typename U,
    typename... U1,
    typename... V1
>
struct append_s<T<T1...>, U<U1...>, V1...> {
    using type = append<T<T1..., U1...>, V1...>;
};


void test_append() {
    using append1_input1 = list<int, char>;
    using append1_input2 = std::tuple<double, float>;
    using append1_output = append<append1_input1, append1_input2>;
    using append1_result = list<int, char, double, float>;

    static_assert(std::is_same_v<append1_output, append1_result>);

    using append2_input1 = std::tuple<int, char>;
    using append2_input2 = list<double>;
    using append2_input3 = list<float>;
    using append2_output = append<append2_input1, append2_input2, append2_input3>;
    using append2_result = std::tuple<int, char, double, float>;

    static_assert(std::is_same_v<append2_output, append2_result>);

    using append3_output = append<>;
    using append3_result = list<>;

    static_assert(std::is_same_v<append3_output, append3_result>);
}

#endif /* append_h */
