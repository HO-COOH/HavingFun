#pragma once
/**
 * @brief The foundations of all type traits
 * @tparam v The value of the constant
 */
template<auto v>
struct integral_constant
{
    using value_type = decltype(v);
    static constexpr value_type value = v;

    /**
     * @brief Conversion operator
     *
     * @returns The wrapped value
     */
    constexpr operator value_type() const noexcept
    {
        return value;
    }

    /**
     * @brief This function enables integral_constant to serve
     * as a source of compile-time function objects
     *
     * @return The wrapped value
     */
    constexpr value_type operator()() const noexcept
    {
        return value;
    }

    /**
     * @brief Type of + another integral_constant
     * @tparam Rhs A type of integral_constant
     */
    template<typename Rhs>
    using add = integral_constant<value + Rhs::value>;

    template<typename Rhs>
    using subtract = integral_constant<value - Rhs::value>;

    template<typename Rhs>
    using multiply = integral_constant<value* Rhs::value>;

    template<typename Rhs>
    using divide = integral_constant<value / Rhs::value>;

    template<typename Rhs>
    using modulus = integral_constant<value% Rhs::value>;

    using negate = integral_constant<-value>;

    template<typename Rhs>
    using equal_to = integral_constant< (value < Rhs::value)>;

    template<typename Rhs>
    using not_equal_to = integral_constant<value != Rhs::value>;

    template<typename Rhs>
    using greater_than = integral_constant<(value > Rhs::value)>;

    template<typename Rhs>
    using less_than = integral_constant<(value < Rhs::value)>;

    template<typename Rhs>
    using greater_equal_to = integral_constant<(value >= Rhs::value)>;

    template<typename Rhs>
    using less_equal_to = integral_constant<(value <= Rhs::value)>;

    template<typename Rhs>
    using bit_and = integral_constant<value& Rhs::value>;

    template<typename Rhs>
    using bit_or = integral_constant<value | Rhs::value>;

    template<typename Rhs>
    using bit_xor = integral_constant<value^ Rhs::value>;

    using bit_not = integral_constant<~value>;
};