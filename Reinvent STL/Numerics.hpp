#pragma once
#include <algorithm>

#include "TypeTraits.hpp"

template<typename T>
constexpr T const& max(T const& a, T const& b)
{
    return (a < b) ? b : a;
}

template<typename T>
constexpr T const& min(T const& a, T const& b)
{
    return (a < b) ? a : b;
}

template<typename T, typename Compare>
constexpr T const& max(T const& a, T const& b, Compare comp)
{
    return (comp(a, b)) ? b : a;
}

template<typename T>
constexpr T max(std::initializer_list<T> ilist)
{
    return *std::max_element(ilist.begin(), ilist.end());
}

template<typename T, typename Compare>
constexpr T max(std::initializer_list<T> ilist, Compare comp)
{
    return *std::max_element(ilist.begin(), ilist.end(), std::move(comp));
}

/**
 * @brief Computes the absolute value of an integer number.
 * The behavior is undefined if the result cannot be represented by the return type.
 * @tparam T Type of the input value
 * @tparam ReturnType Type of the return value
 * @param value Input value
 * @return |value|
 */
template<typename T, typename ReturnType = T>
constexpr ReturnType abs_constexpr(T value)
{
    static_assert(sizeof(ReturnType) >= sizeof(T), "ReturnType must be at least as wide as the input type");
    return value < 0 ? -static_cast<ReturnType>(value) : static_cast<ReturnType>(value);
}


namespace details
{
    template<typename UnsignedType>
    constexpr auto gcd(UnsignedType uint1, UnsignedType uint2)
    {
        static_assert(is_unsigned_v<UnsignedType>);
        /*using the euclidean algorithm*/
        auto a = max(uint1, uint2);
        auto b = min(uint1, uint2);
        while(b!=0)
            a = std::exchange(b, a % b);
        return a;
    }
}

/**
 * @brief Computes the greatest common divisor of the integers m and n.
 * @tparam Int1 Type of the first integer
 * @tparam Int2 Type of the second integer
 * @param int1 integer values
 * @param int2 integer values
 * @return If both m and n are zero, returns zero. Otherwise, returns the greatest common divisor of |m| and |n|.
 */
template<typename Int1, typename Int2>
constexpr common_type_t<Int1, Int2> gcd(Int1 int1, Int2 int2)
{
    static_assert(is_integral_v<Int1> && is_integral_v<Int2>, "Argument of gcd() must be integers");
    static_assert(Int1(1) != Int1(2) && Int2(1) != Int2(2), "Argument of gcd() must not be bool");

    using common_type_unsigned = make_unsigned_t<common_type_t<Int1, Int2>>;
    return details::gcd(abs_constexpr<Int1, common_type_unsigned>(int1), abs_constexpr<Int2, common_type_unsigned>(int2));
}