#pragma once
#include <algorithm>

template<typename T>
constexpr T const& max(T const& a, T const& b)
{
    return (a < b) ? b : a;
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
    return *std::max_element()
}