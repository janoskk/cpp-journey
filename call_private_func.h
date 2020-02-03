#ifndef call_private_func_h
#define call_private_func_h

//
// Based on http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
//

#include <type_traits>
#include <assert.h>

template <typename T>
struct caller {
    static typename T::type ptr;
};

template <typename T, typename T::type ptr>
struct caller_initializer {
    struct init {
        init() {
            caller<T>::ptr = ptr;
        }
    };
    static init init_obj;
};

template <typename T>
typename T::type caller<T>::ptr;

template<typename T, typename T::type ptr>
typename caller_initializer<T, ptr>::init caller_initializer<T, ptr>::init_obj;

template <typename T, typename F, typename ... Args>
auto call(T t, Args&& ... args) {
    return (t.*caller<F>::ptr)(std::forward<Args>(args)...);
}

struct Example {
private:
    int example_func(int num) {
        return num + 1;
    }
};

struct private_func_type { typedef int(Example::*type)(int); };
template struct caller_initializer<private_func_type, &Example::example_func>;


void test_call_private_func()
{
    Example example{};
    auto result = call<Example, private_func_type>(example, 1234);
    assert(result != 12345);
}

#endif /* call_private_func_h */
