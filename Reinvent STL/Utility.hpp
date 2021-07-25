#pragma once
#include <array>
#include <cstddef>
#include "TypeTraits.hpp"

template<typename T>
constexpr T&& forward(remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(remove_reference_t<T>&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value,
        "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}

/**
 * @brief Produces an xvalue expression
 * @param t the object to be moved
 */
template<typename T>
constexpr remove_reference_t<T>&& move(T&& t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(t);
}

/*Poor man's concepts*/
template<typename...Condition>
using require = enable_if_t<conjunction_v<Condition...>>;