#ifndef tuple_cat_h
#define tuple_cat_h

#include <type_traits>
#include <tuple>


template<typename Tuple_A, std::size_t N_A, typename Tuple_B, std::size_t N_B>
struct TupleExpander {
    template <typename... Args>
    static auto expand(const Tuple_A& t_A, const Tuple_B& t_B, Args... args)
    {
        if constexpr (N_B >= 1) {
            return TupleExpander<Tuple_A, N_A, Tuple_B, N_B-1>::expand(t_A, t_B, std::get<N_B-1>(t_B), args...);
        }
        else if constexpr (N_A > 1 && N_B == 0) {
            return TupleExpander<Tuple_A, N_A-1, Tuple_B, N_B>::expand(t_A, t_B, std::get<N_A-1>(t_A), args...);
        }
        else if constexpr (N_A == 1 && N_B == 0) {
            return std::make_tuple(std::get<0>(t_A), args...);
        }
        else {
            return std::tuple{};
        }
    }
};

template<typename... Args_A, typename... Args_B>
auto expand(const std::tuple<Args_A...>& t_A, const std::tuple<Args_B...>& t_B)
{
    return TupleExpander<decltype(t_A), sizeof...(Args_A), decltype(t_B), sizeof...(Args_B)>::expand(t_A, t_B);
}

// my_tuple_cat
auto my_tuple_cat() {
    return std::tuple{};
}

template <typename... Args_A>
auto my_tuple_cat(std::tuple<Args_A...> t_A) {
    return t_A;
}

template <typename... Args_A, typename... Args_B, typename... Others>
auto my_tuple_cat(std::tuple<Args_A...> t_A, std::tuple<Args_B...> t_B, Others... others) {
    if constexpr (sizeof...(Others) == 0) {
        return expand(t_A, t_B);
    }
    else {
        return my_tuple_cat(expand(t_A, t_B), others...);
    }
}

 
// helper function to print a tuple of any size - Source: https://en.cppreference.com/w/cpp/utility/tuple/tuple_cat
template<typename Tuple, std::size_t N>
struct TuplePrinter {
    static void print(const Tuple& t)
    {
        TuplePrinter<Tuple, N-1>::print(t);
        std::cout << ", " << std::get<N-1>(t);
    }
};
 
template<typename Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(const Tuple& t)
    {
        std::cout << std::get<0>(t);
    }
};
 
template<typename... Args, std::enable_if_t<sizeof...(Args) == 0, int> = 0>
void print(const std::tuple<Args...>& t)
{
    std::cout << "()\n";
}

template<typename... Args, std::enable_if_t<sizeof...(Args) != 0, int> = 0>
void print(const std::tuple<Args...>& t)
{
    std::cout << "(";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
    std::cout << ")\n";
}
// end helper function


void test_tuple_cat() {
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
    
    auto input3_1 = std::make_tuple(1, "foo", 2, "bar");
    auto input3_2 = std::make_tuple("asd", 3.14);
    auto input3_3 = std::make_tuple("asdfg", 3.1415);
    auto expected3 = std::tuple_cat(input3_1, input3_2, input3_3);
    auto result3 = my_tuple_cat(input3_1, input3_2, input3_3);
    
    static_assert(std::is_same_v<decltype(expected3), decltype(result3)>);
    print(expected3);
    print(result3);
}

#endif /* tuple_cat_h */
