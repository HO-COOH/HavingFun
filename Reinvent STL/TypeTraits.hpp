#pragma once
#include <cstddef> //for std::nullptr_t
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

template <typename...>
using void_t = void;

template<bool B, typename T, typename F> struct conditional { using type = T; };
template<typename T, typename F> struct conditional<false, T, F> { using type = F; };
template<bool B, typename T, typename F> using conditional_t = typename conditional<B, T, F>::type;

template<typename T> struct is_reference      : false_type {};
template<typename T> struct is_reference<T&>  : true_type {};
template<typename T> struct is_reference<T&&> : true_type {};
template<typename T> inline constexpr bool is_reference_v = is_reference<T>::value;

template<typename T> struct remove_reference      {using type = T;};
template<typename T> struct remove_reference<T&>  {using type = T;};
template<typename T> struct remove_reference<T&&> {using type = T;};
template<typename T> using remove_reference_t = typename remove_reference<T>::type;


template<typename T> struct remove_cv                   { using type = T; };
template<typename T> struct remove_cv<const T>          { using type = T; };
template<typename T> struct remove_cv<volatile T>       { using type = T; };
template<typename T> struct remove_cv<const volatile T> { using type = T; };
template<typename T> using remove_cv_t = typename remove_cv<T>::type;

 
template<typename T> struct remove_const                { using type = T; };
template<typename T> struct remove_const<const T>       { using type = T; };
template<typename T> using remove_const_t = typename remove_const<T>::type;
 
template<typename T> struct remove_volatile             { using type = T; };
template<typename T> struct remove_volatile<volatile T> { using type = T; };
template<typename T> using remove_volatile_t = typename remove_volatile<T>::type;




template<typename T1, typename T2>      struct is_same : false_type {};
template<typename T>                    struct is_same<T, T> : true_type {};
template<typename T1, typename T2>      inline constexpr bool is_same_v = is_same<T1, T2>::value;
template<typename T, typename ...Ts>    struct are_same : conditional_t<(is_same_v<T, Ts> && ...), true_type, false_type> {};
template<typename T, typename ...Ts>    inline constexpr bool are_same_v = are_same<T, Ts...>::value;

template<unsigned I, typename T, typename ...Ts>
struct i_th_type
{
    static_assert(I <= sizeof...(Ts));
    using type = typename i_th_type<I - 1, Ts...>::type;
};
template<typename T, typename ...Ts>
struct i_th_type<0, T, Ts...>
{
    using type = T;
};
template<unsigned I, typename T, typename ...Ts> using i_th_type_t = typename i_th_type<I, T, Ts...>::type;

/**
 * @brief Get the i-th type of an instantiated class template
 * @details GCC has an implementation of getting the first argument of such template in <bits/ptr_traits.h> Line 49:
 *  template<typename T>
 *  struct get_first_arg;
 *  template<template<typename, typename...> typename Template, typename T, typename... Ts>
 *  struct get_first_arg<Template<T, Ts...>> { using type = T;}
 */
template<unsigned I, typename T>
struct i_th_template_type;
template<unsigned I, template<typename, typename...> typename Thing, typename T, typename... Ts>
struct i_th_template_type<I, Thing<T, Ts...>>
{
    using type = i_th_type_t<I, T, Ts...>;
};
template<unsigned I, typename T>
using i_th_template_type_t = typename i_th_template_type<I, T>::type;


/**
 * @brief Detects if Op<Args...> is a valid type, where Op should be defined as a type alias
 * 
 * @details An example is:
 *  template<typename T>
 *  using pointer = typename T::pointer;
 *  
 * Therefore, pointer<Args...> becomes Args::pointer
 * 
 *  struct HasPointer
 *  {
 *      using pointer = int*;
 *  };  //This will make detector_or_t<Default, pointer, HasPointer> -> int*
 *  struct HasNoPointer {};//This will make detector_or_t<Default, pointer, HasNoPointer> -> Default
 *  
 * This is GCC's implementation in <type_traits> Line 2580:
 *  template<typename Default, typename Void, template<typename...> typename Op, typename...Args>
    struct detector : false_type
    {
        using type = Default;
    };
    template<typename Default, template<typename...> typename Op, typename... Args>
    struct detector<Default, void_t<Op<Args...>>, Op, Args...> : true_type
    {
        using type = Op<Args...>;
    };
    template <typename Default, template <typename...> typename Op, typename... Args>
    using detected_or = detector<Default, void, Op, Args...>;
    template <typename Default, template <typename...> typename Op, typename... Args>
    using detected_or_t = typename detected_or<Default, Op, Args...>::type;

    Here is my implementation, so that you can use it as a normal type traits to detect whether the type alias exist, using [detected_v]
 */
template<typename Void, template<typename...> typename Op, typename...Args>
struct detected : false_type {};

template<template<typename...> typename Op, typename... Args>
struct detected<void_t<Op<Args...>>, Op, Args...> : true_type
{
    using type = Op<Args...>;
};

template<template<typename...> typename Op, typename... Args> constexpr bool detected_v = detected<void, Op, Args...>::value;

template<template<typename...> typename Op, typename... Args>
using detected_t = typename detected<void, Op, Args...>::type;

template <bool, typename Default, template <typename...> typename Op, typename... Args>
struct detected_or
{
    using type = Default;
};

template <typename Default, template <typename...> typename Op, typename... Args>
struct detected_or<true, Default, Op, Args...>
{
    using type = detected_t<Op, Args...>;
};

template <typename Default, template <typename...> typename Op, typename... Args>
using detected_or_t = typename detected_or<detected_v<Op, Args...>, Default, Op, Args...>::type;

/**
 * @brief Change Thing<T,...> -> Thing<U, ...>
 * 
 * @details This is GCC's implementation in <bits/ptr_traits.h> Line 62
 */
template<typename Thing, typename U>
struct replace_first_template_type { };
template<template<typename, typename...> typename Thing, typename U, typename T, typename...Ts>
struct replace_first_template_type<Thing<T, Ts...>, U>
{
    using type = Thing<U, Ts...>;
};
template<typename Thing, typename U> using replace_first_template_type_t=typename replace_first_template_type<Thing, U>::type;

namespace details
{
    template<typename> struct is_integral               : false_type{};
    template<> struct is_integral<bool>                 : true_type{};
    template<> struct is_integral<char>                 : true_type{};
    template<> struct is_integral<signed char>          : true_type{};
    template<> struct is_integral<unsigned char>        : true_type{};
    template<> struct is_integral<wchar_t>              : true_type{};
    template<> struct is_integral<char16_t>             : true_type{};
    template<> struct is_integral<char32_t>             : true_type{};
    template<> struct is_integral<short>                : true_type{};
    template<> struct is_integral<unsigned short>       : true_type{};
    template<> struct is_integral<int>                  : true_type{};
    template<> struct is_integral<unsigned int>         : true_type{};
    template<> struct is_integral<long>                 : true_type{};
    template<> struct is_integral<unsigned long>        : true_type{};
    template<> struct is_integral<long long>            : true_type{};
    template<> struct is_integral<unsigned long long>   : true_type{};
}
template<typename T> struct is_integral : details::is_integral<remove_cv_t<T>> {};
template<typename T> inline constexpr bool is_integral_v = is_integral<T>::value;

namespace details
{
    template<typename> struct is_floating_point         : false_type{};
    template<> struct is_floating_point<float>          : true_type{};
    template<> struct is_floating_point<double>         : true_type{};
    template<> struct is_floating_point<long double>    : true_type{};
}
template<typename T> struct is_floating_point : details::is_floating_point<remove_cv_t<T>> {};
template<typename T> inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

template<typename T> struct is_arithmetic : bool_constant<is_integral_v<T> || is_floating_point_v<T>> {};
template<typename T> inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

//This requires compiler intrinsics
template<typename T> struct is_enum : bool_constant<__is_enum(T)>{};
template<typename T> inline constexpr bool is_enum_v = is_enum<T>::value;

//This requires compiler intrinsics
template<typename T> struct is_union : bool_constant<__is_union(T)>{};
template<typename T> inline constexpr bool is_union_v = is_union<T>::value;

namespace details
{
    template<typename> struct is_pointer        : false_type{};
    template<typename T> struct is_pointer<T*>  : true_type{};
}
template<typename T> struct is_pointer : details::is_pointer<remove_cv_t<T>> {};
template<typename T> inline constexpr bool is_pointer_v = is_pointer<T>::value;

namespace details
{
    template<typename> struct is_member_pointer                                     : false_type{};
    template<typename T, typename Member> struct is_member_pointer<T Member::*>     : true_type{};
}
template<typename T> struct is_member_pointer : details::is_member_pointer<remove_cv_t<T>>{};
template<typename T> inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

template<typename T> struct is_null_pointer : is_same<std::nullptr_t, remove_cv_t<T>>{};
template<typename T> inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

template<typename T>
struct is_scalar : bool_constant<
        is_arithmetic_v<T>      ||
        is_enum_v<T>            ||
        is_pointer_v<T>         ||
        is_member_pointer_v<T>  ||
        is_null_pointer_v<T>
>{};
template<typename T> inline constexpr bool is_scalar_v = is_scalar<T>::value;

template<typename T> struct is_array                        : false_type{};
template<typename T> struct is_array<T[]>                   : true_type{};  //?
template<typename T, std::size_t N> struct is_array<T[N]>   : true_type{}; 
template<typename T> inline constexpr bool is_array_v = is_array<T>::value;

template<typename T> struct remove_extent { using type = T; };
template<typename T> struct remove_extent<T[]> { using type = T; };
template<typename T, std::size_t N> struct remove_extent<T[N]> { using type = T; };
template<typename T> using remove_extent_t = typename remove_extent<T>::type;


//In GCC, the is_class<T> type traits is implemented as compiler intrinsics
namespace details
{
    template <typename T>
    bool_constant<!is_union_v<T>> test(int T::*);   //The function parameter is a pointer to member data, so this overload matches a class type pointer
    /* Why it should not be a union you ask? Because a union is actually a class type in C++.
     * It can have data members and member functions as classes.
     * See this answer: https://stackoverflow.com/a/23933679/11108133
     */

    template <typename>
    false_type test(...);   //This overload matches everything else
}
template<typename T> struct is_class : decltype(details::test<T>(nullptr)) {}; 
template<typename T> inline constexpr bool is_class_v = is_class<T>::value;

template<typename T>
struct is_object : bool_constant<
        is_scalar_v<T>  ||
        is_array_v<T>   ||
        is_union_v<T>   ||
        is_class_v<T>
>{};
template<typename T> inline constexpr bool is_object_v = is_object<T>::value;

namespace details
{
    template <typename T>
    remove_reference_t<T>* try_add_pointer(int);
    
    template <typename T>
    T try_add_pointer(...);
}
template<typename T> struct add_pointer { using type = decltype(details::try_add_pointer<T>(0)); };
template<typename T> using add_pointer_t = typename add_pointer<T>::type;

template<typename T> struct is_const : false_type{};
template<typename T> struct is_const<const T> : true_type{};
template<typename T> inline constexpr bool is_const_v = is_const<T>::value;

/**
 * @note Types like std::function, lambdas, classes with overloaded operator() and pointers to functions don't count as function types
 * 
 * @details How does this work?
 * There are only two categories of types in the language that cannot have const-qualification: reference types, and function types. 
 * So, if const T fails to be a const-qualified type, it means T is either a function type or a reference type. 
 * If you can rule out reference types, then you are left with only function types.
 */
template<typename T> struct is_function : bool_constant<!is_const_v<const T> && !is_reference_v<T>>{};
template<typename T> inline constexpr bool is_function_v = is_function<T>::value;

template<typename T>
struct decay
{
private:
    using U = remove_reference_t<T>;
public:
    using type = conditional_t
    <
        is_array_v<U>,
        remove_extent_t<U>*,
        conditional_t
        <
            is_function_v<U>,
            add_pointer_t<U>,
            remove_cv_t<U>
        >
    >;
    /* The logic is
        if is array
            decay to pointer
        else
            if is function(std::function, lambdas, class with overloaded operator() are NOT function types)
                decay to function pointer
            else
                remove any cv-qualifier
    */
};
template<typename T> using decay_t = typename decay<T>::type;

namespace details
{
    template <typename T>
    T &try_add_lvalue_reference(int);

    template <typename T>
    T try_add_lvalue_reference(...);

    template <typename T>
    T &&try_add_rvalue_reference(int);

    template <typename T>
    T try_add_rvalue_reference(...);
}
template<typename T> struct add_lvalue_reference { using type = decltype(details::try_add_lvalue_reference<T>(0)); };
template<typename T> struct add_rvalue_reference { using type = decltype(details::try_add_rvalue_reference<T>(0)); };
template<typename T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
template<typename T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <typename T>
add_rvalue_reference_t<T> declval() noexcept;


template<typename...> struct common_type{};
template<typename T> struct common_type<T> : common_type<T, T>{};
template<typename T1, typename T2> struct common_type<T1, T2>
{
    using type = decay_t<decltype(false ? declval<decay_t<T1>>() : declval<decay_t<T2>>())>;
};
template<typename T1, typename T2, typename... Ts> struct common_type<T1, T2, Ts...> : common_type<typename common_type<T1, T2>::type, Ts...> {};
template <typename... T> using common_type_t = typename common_type<T...>::type;