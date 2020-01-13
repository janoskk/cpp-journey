#ifndef is_a_h
#define is_a_h

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <type_traits>

template <
	typename,
	template <typename...> typename
>
struct is_a {
	static constexpr bool value = false;
};

template<
	template <typename... T1> typename T,
	template <typename...> typename U,
	typename... T1
>
struct is_a<T<T1...>, U> {
	template <
		template <typename... Y1> typename Y,
		template <typename...> typename X,
		typename... Y1
	>
    static auto check(int const*) -> typename std::bool_constant< std::is_same_v< X<Y1...>, Y<Y1...> > >;
    
	template <
		template <typename...> typename,
		template <typename...> typename,
		typename...
	>
    static auto check(...) -> std::false_type;
    
	static constexpr bool value = decltype(check<T, U, T1...>(0))::value;
};

/*
 * As Szilard pointed out, it could be solved by pattern matching instead of SFINAE:
 *
 * template <typename, template <typename...> typename>
 * struct is_a : std::false_type {};
 *
 * template <template <typename...> typename T, typename... Ts>
 * struct is_a<T<Ts...>, T> : std::true_type {};
 *
 * Simpler a little bit ;-))))
 */


// for test
class TestX { };

template<class T>
class TestY {
	int y;
	T t;
};

template<class T>
class TestZ {
	float z;
	T tt;
};


void test_is_a()
{
	static_assert(is_a< std::vector<std::string>, std::list >::value == false, "list should not be a vector");
	static_assert(is_a< std::vector<TestY<TestX>>, std::vector >::value == true, "vector<Test<>> should be a vector");

	static_assert(is_a< TestY<TestX>, TestZ >::value == false, "Y<X> should not be a Z");
	static_assert(is_a< TestY<TestX>, TestY >::value == true, "Y<X> should be an X");
	
	static_assert(is_a< std::string, std::vector >::value == false, "string should not be a vector");
	static_assert(is_a< std::vector<std::string>, std::vector >::value == true, "vector<string> should be a vector");

	static_assert(is_a< int, std::vector >::value == false, "int should not be a vector");
	static_assert(is_a< std::vector<int>, std::vector >::value == true, "vector<int> should be a vector");
}

#endif /* is_a_h */
