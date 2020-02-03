#ifndef filler_h
#define filler_h

#include <type_traits>
#include <memory>

template<
    typename T,
    template <typename...> typename
>
struct filler_s {
    static_assert(sizeof(T) == 0, "Should be used for template type");
};

template<
    template <typename... T1> typename T,
    template <typename...> typename U,
    typename... T1
>
struct filler_s<T<T1...>, U> {
    using type = U<T1...>;
};

template<
    typename T,
    template <typename...> typename U
>
using filler = typename filler_s<T, U>::type;


void test_filler() {
    static_assert(std::is_same_v<filler<std::shared_ptr<int>, std::unique_ptr>, std::unique_ptr<int>>);
}

#endif /* filler_h */
