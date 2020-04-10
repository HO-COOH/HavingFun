#include "Noisy_Class.hpp"
#include <tuple>

auto return_moved_tuple()
{
    MyClass obj1;
    MyClass obj2;
    return std::tuple{std::move(obj1), std::move(obj2)};
}

auto return_copied_tuple()
{
    MyClass obj1;
    MyClass obj2;
    return std::tuple{obj1, obj2};
}

int main()
{

    auto [o1, o2]=return_moved_tuple();     //moved
    auto [o3, o4]=return_copied_tuple();    //copied
    puts("Main exit");
}