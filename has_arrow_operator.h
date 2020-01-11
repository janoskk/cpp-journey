#ifndef has_arrow_operator_h
#define has_arrow_operator_h

#include <type_traits>

template <typename T>
struct has_arrow_operator__not_ok
{
    template <typename U, U* (U::*)()>
    struct check;
    
    template <typename U>
    static std::true_type test(check<U, &U::operator->> *);
    
    template <typename U>
    static std::false_type test(...);
    
    static constexpr const bool value = decltype(test<T>(0))::value;
};

template <typename T>
struct has_arrow_operator__still_not_ok
{
    template <typename U, typename V>
    static U* check(U (V::*)());

    template <typename U, typename V>
    static U* check(U (V::*)() const);
    
    template <typename U>
    static std::true_type test(decltype(check(&U::operator->)));
    
    template <typename U>
    static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(0))::value;
};

template <typename T>
struct has_arrow_operator
{
    template <typename U>
    static auto check(U const*) -> decltype(&U::operator->, std::true_type{});
    
    template <typename>
    static auto check(...) -> std::false_type;
    
    static constexpr bool value = decltype(check<T>(0))::value;
};


void test_has_arrow_operator()
{
    {
        struct S {
        };
        static_assert(has_arrow_operator<S>::value == false, "Should NOT be implemented");
    }
    {
        struct S {
            S* operator->() {
                return 0;
            }
        };
        static_assert(has_arrow_operator<S>::value == true, "Should be implemented");
    }
    {
        struct S {
            virtual S* operator->() = 0;
        };
        struct R : public S {
            virtual S* operator->() override {
                return 0;
            };
        };
        static_assert(has_arrow_operator<S>::value == true, "Should be implemented");
        static_assert(has_arrow_operator<R>::value == true, "Should be implemented");
    }
    {
        struct T {
            int* operator->() {
                return &i;
            }
            int i{ 10 };
        };
        struct S {
            T* operator->() {
                return &t;
            }
            T t;
        };
        static_assert(has_arrow_operator<T>::value == true, "Should be implemented");
        static_assert(has_arrow_operator<S>::value == true, "Should be implemented");
    }
    {
        struct S { S* operator->() const; };
        static_assert(has_arrow_operator<S>::value == true, "Should be implemented");
    }
    {
        struct U { int i; };
        struct T {
            U* operator->() { return &u; };
            U u;
        };
        static_assert(has_arrow_operator<T>::value == true, "Should be implemented");
        struct S {
            T operator->() {
                return t;
            }
            T t;
        };
        static_assert(has_arrow_operator<S>::value == true, "Should be implemented");
    }
}


#endif /* has_arrow_operator_h */
