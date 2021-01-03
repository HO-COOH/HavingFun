#include "TypeTraits.hpp"
#include <iostream>




void func(int){}

int main()
{
    common_type_t<int, float, long, double> a;
    std::cout<<typeid(a).name()<<'\n';
}