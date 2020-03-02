#include <iostream>
struct S{
};

void func(S&)
{
    puts("S&");
}
void func(S&&)
{
    puts("S&&");
}
namespace My
{
    template<typename T>
    struct identity
    {
        using type = T;
    };
    
    template<typename T>
    T&& forward(typename identity<T>::type&& param)
    {
        return static_cast<typename identity<T>::type &&>(param);
    }
}
namespace My2
{
    template<typename T>
    struct identity
    {
        using type = T;
    };
    
    template<typename T>
    T& forward(typename identity<T>::type& param)
    {
        return static_cast<typename identity<T>::type &>(param);
    }
    template<typename T>
    T&& forward(typename identity<T>::type&& param)
    {
        return static_cast<typename identity<T>::type &&>(param);
    }
}

int main()
{
    S lvalue;
    func(My::forward<S&>(lvalue));  //need to explicitly specify whether & or && 
    func(My::forward<S&&>(S{}));
    
    func(My2::forward<S>(lvalue));  //don't need to explicitly specify whether & or && 
    func(My2::forward<S>(S{}));
}