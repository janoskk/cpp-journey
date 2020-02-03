#ifndef string_convert_h
#define string_convert_h

#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <type_traits>


template <typename T>
auto string_convert(const T& str)
{
    if constexpr (std::is_same_v<T, std::string>) {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(str);
    }
    else if constexpr (std::is_convertible_v<T, const char *>) {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(str);
    }
    else if constexpr (std::is_same_v<T, std::wstring>) {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.to_bytes(str);
    }
    else if constexpr (std::is_convertible_v<T, const wchar_t *>) {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.to_bytes(str);
    }
    else {
        static_assert(sizeof(T) == 0, "Use only with string types");
    }
}


void test_string_convert()
{
    {
        const std::wstring hello{L"hello\n"};
        assert(string_convert(hello) == std::string{ "hello\n" } && "Conversion error" );
    }
    {
        assert(string_convert(L"hello\n") == std::string{ "hello\n" } && "Conversion error" );
    }
    {
        assert(string_convert(std::wstring{L"hello\n"}) == std::string{ "hello\n" } && "Conversion error" );
    }
    {
        const wchar_t hello[] = L"hello\n";
        assert(string_convert(hello) == std::string{ "hello\n" } && "Conversion error" );
    }
    
    {
        std::string const world{ "world\n" };
        assert(string_convert(world) == std::wstring{ L"world\n" } && "Conversion error" );
    }
    {
        std::string world{"world\n"};
        assert(string_convert(world) == std::wstring{ L"world\n" } && "Conversion error" );
    }
    {
        assert(string_convert("world\n") == std::wstring{ L"world\n" } && "Conversion error" );
    }
    {
        assert(string_convert(std::string{"world\n"}) == std::wstring{ L"world\n" } && "Conversion error" );
    }
    {
        char world[] = "world\n";
        assert(string_convert(world) == std::wstring{ L"world\n" } && "Conversion error" );
    }
    
    {
        // std::cout << string_convert(123);
    }
}

#endif /* string_convert_h */
