#pragma once

/**
 * @brief The foundations of all type traits
 * 
 * @tparam Type The type of the constant
 * @tparam v The value of the constant
 */
template<typename Type, Type v>
struct integral_constant
{
    static constexpr Type value = v;
    using value_type = Type;
    
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
};

/**
 * @brief A helper alias template for boolean constant
 * 
 * @tparam boolValue The constant boolean value
 */
template <bool boolValue>
using bool_constant = integral_constant<bool, boolValue>;

/**
 * @brief The type for a true bool_constant
 */
using true_type = bool_constant<true>;

/**
 * @brief The type for a false bool_constant
 */
using false_type = bool_constant<false>;

