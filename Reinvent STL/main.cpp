#include <iostream>
#include "Utility.hpp"

namespace details
{
    template <typename T>
    struct is_instance_of_impl
    {
    };
} // namespace details

template <typename T, typename U>
constexpr auto operator*(T &&object, details::is_instance_of_impl<U> query)
{
    return std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(object)>>, U>;
}

#define is_instance_of(Type) \
    *details::is_instance_of_impl<Type> {}

int main()
{
    std::cout << make_index_sequence<100>::sum() << '\n';
    //std::cout << make_index_sequence<1000000>::sum() << '\n';
}