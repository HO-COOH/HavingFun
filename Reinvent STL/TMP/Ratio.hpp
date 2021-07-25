

#pragma once

#include <cstdint>
#include "../Numerics.hpp"


/**
 * @brief A compile-time rational number, which is usually used as a type as a template argument
 * Otherwise it could be implemented as a constexpr class
 * @tparam Num The numerator
 * @tparam Denom The denominator
 */
template<std::intmax_t Num, std::intmax_t Denom = 1>
struct ratio
{
private:
    static constexpr auto greatestCommonDevisor = gcd(Num, Denom);
public:
    static_assert(Denom != 0, "Denominator cannot be 0");
    /**
     * @brief The reduced numerator, i.e sign(Num) * sign(Denom) * abs(Num) / gcd(Num, Denom)
     */
    static constexpr std::intmax_t num = Num * (Denom > 0 ? 1 : -1) / greatestCommonDevisor;
    /**
     * @brief The reduced denominator, i.e abs(Denom) / gcd(Num, Denom)
     */
    static constexpr std::intmax_t den = abs_constexpr(Denom) / greatestCommonDevisor;

    using type = ratio<num, den>;

    template<typename T>
    static constexpr T value = static_cast<T>(num) / den;

    template<typename T>
    constexpr operator T() noexcept
    {
        return value<T>;
    }
};

template<typename R1, typename R2>
using ratio_add = typename ratio<R1::num* R2::den + R2::num * R1::den, R1::den* R2::den>::type;

template<typename R1, typename R2>
using ratio_subtract = typename ratio<R1::num* R2::den - R2::num * R1::den, R1::den* R2::den>::type;

template<typename R1, typename R2>
using ratio_multiply = typename ratio<R1::num* R2::num, R1::den* R2::den>::type;

template<typename R1, typename R2>
using ratio_divide = typename ratio<R1::num* R2::den, R1::den* R2::num>::type;

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator+(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_add<ratio<num1, den1>, ratio<num2, den2>>{};
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator-(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_subtract<ratio<num1, den1>, ratio<num2, den2>>{};
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator*(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_multiply<ratio<num1, den1>, ratio<num2, den2>>{};
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator/(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_divide<ratio<num1, den1>, ratio<num2, den2>>{};
}

template<typename R1, typename R2>
using ratio_equal = bool_constant<R1::num == R2::num && R1::den == R2::den>;
template<typename R1, typename R2>
inline constexpr bool ratio_equal_v = ratio_equal<R1, R2>::value;

template<typename R1, typename R2>
using ratio_not_equal = bool_constant<!ratio_equal_v<R1, R2>>;
template<typename R1, typename R2>
inline constexpr bool ratio_not_equal_v = ratio_not_equal<R1, R2>::value;

template<typename R1, typename R2>
using ratio_less = bool_constant < (R1::num* R2::den < R2::num* R1::den)>;
template<typename R1, typename R2>
inline constexpr bool ratio_less_v = ratio_less<R1, R2>::value;

template<typename R1, typename R2>
using ratio_less_equal = bool_constant<(R1::num* R2::den <= R2::num * R1::den)>;
template<typename R1, typename R2>
inline constexpr bool ratio_less_equal_v = ratio_less_equal<R1, R2>::value;

template<typename R1, typename R2>
using ratio_greater = bool_constant<!ratio_less_equal_v<R1, R2>>;
template<typename R1, typename R2>
inline constexpr bool ratio_greater_v = ratio_greater<R1, R2>::value;

template<typename R1, typename R2>
using ratio_greater_equal = bool_constant<!ratio_less_v<R1, R2>>;
template<typename R1, typename R2>
inline constexpr bool ratio_greater_equal_v = ratio_greater_equal<R1, R2>::value;

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator==(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_equal_v<ratio<num1, den1>, ratio<num2, den2>>;
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator!=(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_not_equal_v<ratio<num1, den1>, ratio<num2, den2>>;
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator<(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_less_v<ratio<num1, den1>, ratio<num2, den2>>;
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator<=(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_less_equal_v<ratio<num1, den1>, ratio<num2, den2>>;
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator>(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_greater_v<ratio<num1, den1>, ratio<num2, den2>>;
}

template<std::intmax_t num1, std::intmax_t num2, std::intmax_t den1, std::intmax_t den2>
constexpr auto operator>=(ratio<num1, den1>, ratio<num2, den2>)
{
    return ratio_greater_equal_v<ratio<num1, den1>, ratio<num2, den2>>;
}