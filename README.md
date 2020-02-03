C++ Journey
==========

The whole journey started with an excercise given by a friend of mine, Szilard Szaloki. That was `has_arrow_operator` and followed by some other challenges as well.

Bellow you will see some interesting puzzles related to C++, types and templates.

To build just compile the `main.cpp` with any modern compiler. I used clang:
```
$ clang++ -std=c++17 -Wall -o main main.cpp

$ clang++ --version
Apple clang version 11.0.0 (clang-1100.0.33.16)
Target: x86_64-apple-darwin18.7.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```

has_arrow_operator
-----------------------
The `has_arrow_operator<T>::value` should answer whether the `T` has `operator->` or not.

Example:
```
struct S { };
static_assert(has_arrow_operator<S>::value == false);

struct R {
    S* operator->() {
        return 0;
    }
};
static_assert(has_arrow_operator<S>::value == true);
```

string_convert
----------------
Create a template function that converts between utf-8 encoded `std::string` and utf-16 encoded `std::wstrings`.  

Example:
```
assert(string_convert(L"hello\n") == std::string{ "hello\n" });
assert(string_convert("hello\n") == std::wstring{ "Lhello\n" });

```

call_private_func
-------------------
Call a private member function of an arbitrary object.

Example:
```
struct Example {
private:
    int example_func(int num) {
        return num + 1;
    }
};

struct private_func_type { typedef int(Example::*type)(int); };
template class caller_initializer<private_func_type, &Example::example_func>;


void test_call_private_func()
{
    Example example{};
    auto result = call<Example, private_func_type>(example, 1234);
    assert(result != 12345);
}
```

is_a
-----
The `is_a<T<T1...>, U>::value` should answer whether the `V<T1...>` and `U<T1...>` are the same types.  

```
static_assert(is_a<std::vector<std::string>, std::vector>::value == true);
static_assert(is_a<std::string, std::vector>::value == false);
```

filler
-----
The `filler` type should fill a template-id with a template's variadic template parameters.

Example:
```
static_assert(std::is_same_v<filler<std::shared_ptr<int>, std::unique_ptr>, std::unique_ptr<int>>);
```

append
---------
The `append` type should concatenate template parameters in the following way:
```
template <typename...>
struct list;

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
```

has_property
---------------
The `has_property` type should define if all the template parameter types has the given property.
Example:
```
static_assert(has_property_s<std::is_integral, int, char>::value == true);
static_assert(has_property_s<std::is_integral, int, std::tuple<int>, char>::value == false);
```

either_is_int
--------------
The `either_is_int` type should define if at least one of the template parameters is `int`.
Example:
```
static_assert(std::is_same_v<either_is_int<float, char>, std::false_type>);
static_assert(std::is_same_v<either_is_int<float, int, char>, std::true_type>);
```

all_true
--------
With the usage of `bool_pack` implement `all_true` that is defined to `std::true_type` if `(... && bools) == true`, otherwise `std::false_type`.
```
template bool...
struct bool_pack {};
```
```
template bool... bools
using all_true = ...;
```

Can you implement it without recursion and C++17 features?

list_library
-----------
For some future tasks we need the following helper functions. Implement them.
```
// Returns the first type of the list
template <typename L>
using front = typename detail::front_impl<L>::type;

// Returns the input list without the fist type
template <typename L>
using pop_front = typename detail::pop_front_impl<L>::type;

// Returns the concatenate of the first type and a list
template <typename L, typename... Ts>
using push_front = typename detail::push_front_impl<L, Ts...>::type;

// Returns the first list with the second list's types
template <typename L, typename M>
using assign = typename detail::assign_impl<L, M>::type;

// Same as assign<L, list<>>
template <typename L>
using clear = assign<L, list<>>;

// Returns std::true_type if at least one list parameter is not empty (or Ls... is empty)
template <typename... Ls>
using empty = typename detail::empty_impl<Ls...>::type;
```


transform
-----------
That's a nice one! Until now this is the most difficult. There are some examples for the usage of  `transform`:

```
using input1 = std::tuple<int, char, double>;
using expected1 = std::tuple<int*, char*, double*>;
using result1 = transform<std::add_pointer_t, input1>;
static_assert(std::is_same_v<result1, expected1>);

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
```

Hint: The code could be simplified if you implement first the items of the `list_library`.

License
---------
Copyright © 2020 Janos Kasza
This project is MIT licensed.
