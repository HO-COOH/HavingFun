/*Description: Binding arbitrary amount of iterators and looping them together
*/
#include <iostream>
#include <array>
#include <tuple>
#include <vector>
#include <list>

template<size_t I = 0, typename... Iters >
void my_advance(std::tuple<Iters...>& iters)
{
    ++std::get<I>(iters);
    if constexpr (I + 1 != sizeof...(Iters))
        my_advance<I + 1>(iters);
}

template<size_t I = 0, typename... Iters >
auto&& operator++(std::tuple<Iters...>& iters)
{
    ++std::get<I>(iters);
    if constexpr (I + 1 != sizeof...(Iters))
        my_advance<I + 1>(iters);
    return iters;
}

template<size_t I = 0, typename Func, typename... Iters >
void my_visit(std::tuple<Iters...>& iters, Func f)
{
    f(std::get<I>(iters));
    if constexpr (I + 1 != sizeof...(Iters))
        my_visit<I + 1>(iters, f);
}

int main()
{
    std::vector a{ 1,2,3,4,5 };
    std::array b{ 6,7,8,9,10 };
    std::list c{ -1,-2,-3,-4,-5 };

    for (auto starts = std::tuple{ a.cbegin(), b.cbegin(), c.cbegin() };
        starts != std::tuple{ a.cend(), b.cend(), c.cend() };
        ++starts
        )
    {
        my_visit(starts, [](const auto& element) {std::cout << *element << " "; });
        std::cout << '\n';
    } 
}