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

License
---------
Copyright © 2020 Szilard Szaloki & Janos Kasza
This project is MIT licensed.
