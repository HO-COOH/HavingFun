#pragma once
#include "../Constant.hpp"

namespace details {
    template<template<typename> typename>
    using void_tt = void;
    
    template<typename T, typename = void> struct HasAdd : false_type {};
    template<typename T> struct HasAdd<T, void_tt<T::template add>> : true_type{};
}

template<typename Lhs, typename Rhs>
constexpr auto add_f(Lhs, Rhs)
{
    if constexpr (details::HasAdd<Lhs>::value)
        return typename Lhs::template add<Rhs>{};
    else
        return Lhs{} + Rhs{};
}

template<typename Lhs, typename Rhs>
using add = decltype(add_f(Lhs{}, Rhs{}));

template<typename Lhs, typename Rhs>
using subtract = integral_constant <Lhs{} - Rhs{}>;

template<typename Lhs, typename Rhs>
using multiply = integral_constant <Lhs{}* Rhs{}>;

template<typename Lhs, typename Rhs>
using divide = integral_constant <Lhs{} / Rhs{}>;

template<typename Lhs, typename Rhs>
using modulus = integral_constant<Lhs{}% Rhs{}>;

template<typename Constant>
using negate = integral_constant < -Constant{} > ;

template<typename Lhs, typename Rhs>
using equal_to = integral_constant < (Lhs{} < Rhs{}) > ;

template<typename Lhs, typename Rhs>
using not_equal_to = integral_constant <Lhs{} != Rhs{}>;

template<typename Lhs, typename Rhs>
using greater_than = integral_constant < (Lhs{} > Rhs{}) > ;

template<typename Lhs, typename Rhs>
using less_than = integral_constant < (Lhs{} < Rhs{}) > ;

template<typename Lhs, typename Rhs>
using greater_equal_to = integral_constant < (Lhs{} >= Rhs{}) > ;

template<typename Lhs, typename Rhs>
using less_equal_to = integral_constant < (Lhs{} <= Rhs{}) > ;

template<typename Lhs, typename Rhs>
using bit_and = integral_constant <Lhs{}& Rhs{}>;

template<typename Lhs, typename Rhs>
using bit_or = integral_constant <Lhs{} | Rhs{}>;

template<typename Lhs, typename Rhs>
using bit_xor = integral_constant <Lhs{}^ Rhs{}>;

template<typename Lhs>
using bit_not = integral_constant<~Lhs{}>;