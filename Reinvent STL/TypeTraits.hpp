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
template<bool B, typename T, typename F> using conditionitional_t = typename conditional<B, T, F>::type;

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
template<typename T, typename ...Ts>    struct are_same : conditionitional_t<(is_same_v<T, Ts> && ...), true_type, false_type> {};
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

template<typename T> struct is_volatile : false_type{};
template<typename T> struct is_volatile<volatile T> : true_type{};
template<typename T> inline constexpr bool is_volatile_v = is_volatile<T>::value;

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
    using type = conditionitional_t
    <
        is_array_v<U>,
        remove_extent_t<U>*,
        conditionitional_t
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


template<bool Conditionition, typename T = void> struct enable_if { };
template<typename T> struct enable_if<true, T> { using type = T; };
template<bool Conditionition, typename T = void> using enable_if_t = typename enable_if<Conditionition, T>::type;

//This is almost the implementation of MSVC in <type_traits> Line 927
namespace details
{
    template<unsigned size, typename T>
    struct sizeof_type;

    template<typename T>
    struct sizeof_type<1, T>
    {
        using unsigned_type = unsigned char;
        using signed_type = signed char;
    };

    template<typename T>
    struct sizeof_type<2, T>
    {
        using unsigned_type = unsigned short;
        using signed_type = signed short;
    };

    template<typename T>
    struct sizeof_type<4, T>
    {
        static constexpr auto is_any_long = is_same_v<T, long> || is_same_v<T, unsigned long>;
        using unsigned_type = conditionitional_t<
            is_any_long,
            unsigned long,
            int
        >;
        using signed_type = conditionitional_t<is_any_long,
            long,
            int
        >;
    };

    template<typename T>
    struct sizeof_type<8, T>
    {
        using unsigned_type = unsigned long long;
        using signed_type = long long;
    };

    template<typename Original, typename T>
    struct add_back_cv
    {
        using type = T;
    };

    template<typename Original, typename T>
    struct add_back_cv<const Original, T>
    {
        using type = const T;
    };

    template<typename Original, typename T>
    struct add_back_cv<volatile Original, T>
    {
        using type = volatile T;
    };

    template<typename Original, typename T>
    struct add_back_cv<const volatile Original, T>
    {
        using type = const volatile T;
    };

    template<typename Original, typename T>
    using add_back_cv_t = typename add_back_cv<Original, T>::type;
}
template<typename T>
struct make_unsigned
{
    static_assert(!is_same_v<remove_cv_t<T>, bool> && !is_enum_v<remove_cv_t<T>>);
    using type = details::add_back_cv_t<T, typename details::sizeof_type<sizeof(T), T>::unsigned_type>;
};
template<typename T> using make_unsigned_t = typename make_unsigned<T>::type;

template<typename T>
struct make_signed
{
    static_assert(!is_same_v<remove_cv_t<T>, bool> && !is_enum_v<remove_cv_t<T>>);
    using type = details::add_back_cv_t<T, typename details::sizeof_type<sizeof(T), T>::signed_type>;
};
template<typename T> using make_signed_t = typename make_signed<T>::type;

//This requires compiler intrinsics
//See more here: https://stackoverflow.com/a/35531415/11108133
template<typename T> struct is_empty : bool_constant<__is_empty(T)>{};
template<typename T> inline constexpr bool is_empty_v = is_empty<T>::value;

/*Logic AND*/
//And this cannot be named as "and" because it is one of the alternative tokens.
template<typename...> 
struct conjunction;

template<> 
struct conjunction <>;

template<typename Condition> 
struct conjunction<Condition> : Condition{};

template<typename Condition1, typename Condition2> 
struct conjunction<Condition1, Condition2> : conditionitional_t<Condition1::value, conjunction<Condition2>, false_type>{};

template<typename Condition1, typename Condition2, typename... Conditions>
struct conjunction<Condition1, Condition2, Conditions...> : conditionitional_t<Condition1::value, conjunction <Condition2, Conditions...>, false_type>{};

template<typename... Condition> inline constexpr bool conjunction_v = conjunction<Condition...>::value;

/*Logic OR*/
//And this cannot be named "or" because it is one of the alternative tokens.
template<typename...> struct disjunction;

template<> struct disjunction<>;

template<typename Condition>
struct disjunction<Condition> : Condition {};

template<typename Condition1, typename Condition2>
struct disjunction<Condition1, Condition2> : conditionitional_t<Condition1::value, true_type, disjunction<Condition2>> {};

template<typename Condition1, typename Condition2, typename... Conditions>
struct disjunction<Condition1, Condition2, Conditions...> : conditionitional_t<Condition1::value, true_type, disjunction <Condition2, Conditions...>> {};


/**
 * Note this is not strictally std::is_constructible, because the line T(declval<Args>()...) is a C-style cast,
 * therefore it counts types that can be performed a reinterpret_cast as constructible, while std:: counter-part does not.
 * For example:
 *  int x{};
 *  intptr_t pt = &x;   //This fails to compile, so is_constructible<intptr_t, int*> should be false, but this implementation will returns true
 * GCC & MSVC both use compiler intrinsics
 */
namespace details
{
    template<typename Void, typename T, typename... Args> struct is_constructible : false_type {};

    template<typename T, typename... Args>
    struct is_constructible<void_t<decltype(T(declval<Args>()...))>, T, Args...> : true_type {};
}
template<typename T, typename... Args> struct is_constructible : details::is_constructible<void, T, Args...> {};
template<typename T, typename... Args> inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

template<typename T> struct is_default_constructible : is_constructible<T> {};
template<typename T> inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;


/**
 * A trivial type is:
 *  TriviallyCopyable, that is:
 *      - Scalar (arithmetics, enum, pointer, member pointer, null pointer)
 *      - Trivially copyable classes, that is
 *          - at least 1 
 *              copy constructor (that it is not deleted) 
 *              / move constructor (that it is not deleted)
 *              / copy assignment operator (that it is not deleted)
 *              / move assignment operator is eligible (that it is not deleted)
 *          -  Every eligible copy constructor (if any) is trivial, that is:
 *              - It is not user-provided (i.e., is implicitly-defined or defaulted)
 *              - T has no virtual member functions
 *              - T has no virtual base classes
 *              - The copy constructor selected for every direct base of T is trivial
 *              - The copy constructor selected for every non-static class type (or array of class type) member of T is trivial
 *          - Every eligible move constructor (if any) is trivial, that is:
 *              - It is not user-provided (i.e., is implicitly-defined or defaulted)
 *              - T has no virtual member functions
 *              - T has no virtual base classes
 *              - The move constructor selected for every direct base of T is trivial
 *              - The move constructor selected for every non-static class type (or array of class type) member of T is trivial
 *          - Every eligible copy assignment operator (if any) is trivial, that is:
 *              - It is not user-provided (i.e., is implicitly-defined or defaulted)
 *              - T has no virtual member functions
 *              - T has no virtual base classes
 *              - The copy assignment operator selected for every direct base of T is trivial
 *              - The copy assignment operator selected for every non-static class type (or array of class type) member of T is trivial 
 *          - Every eligible move assignment operator (if any) is trivial, that is:
 *              - It is not user-provided (i.e., is implicitly-defined or defaulted)
 *              - T has no virtual member functions
 *              - T has no virtual base classes
 *              - The move assignment operator selected for every direct base of T is trivial
 *              - The move assignment operator selected for every non-static class type (or array of class type) member of T is trivial 
 *          - Has a trivial non-deleted destructor, that is:
 *              - It is not user-provided (i.e., is implicitly-defined or defaulted on its first declaration)
 *              - It is not virtual (that is, the base class destructor is not virtual)
 *                  * Having a non-virtual base class destructor does NOT necessarilly means that it should not be inherited,
 *                    it just means that the pointer /reference of a base class should not be assigned to a pointer / reference of a derived class.
 *                    That is, the following is wrong:
 *                      class Base {};
 *                      class Derived : public Base {};
 *                      Base* pt = new Derived;     //Error, derived class destructor not called
 *                    But this is okay:
 *                      Derived obj;        //Stack allocated Derived class object
 *              - All direct base classes have trivial destructors
 *              - All non-static (i.e, normal) data members of class type (or array of class type) have trivial destructors
 *      - Arrays of triviallyCopyable objects
 *  If the type is a class type (that has member function and is not union) or array thereof, the class has one or more eligible default constructors 
 *  (a default constructor is eligible if it is not deleted), all of which are trivial, that is:
 *      - The constructor is not user-provided (i.e., is implicitly-defined or defaulted on its first declaration)
 *          * defaulted on its first declaration is something like this:
 *              struct A
 *              {
 *                  A() = default;  //not user provided
 *              };
 *              
 *              struct B
 *              {
 *                  B();
 *              };
 *              B::B() = default;   //user provided
 *      - T has no virtual member functions
 *      - T has no virtual base classes
 *      - T has no non-static (i.e. normal) members with default initializer
 *      - Every direct base of T has a trivial default constructor
 *      - Every non-static member of class type (or array thereof) has a trivial default constructor 
 */
//These type traits all require compiler intrinsics
template<typename T, typename... Args> struct is_trivially_constructible : bool_constant<__is_trivially_constructible(T, Args...)> {};
template<typename T, typename... Args> inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;
template<typename T> struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};
template<typename T> inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

template<typename T> struct is_trivially_default_constructible : is_trivially_constructible<T> {};
template<typename T> inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;


namespace details
{
    template<typename T> struct is_nonvolatile_trivially_copyable : is_trivially_copyable<T>{};
    template<typename T> struct is_nonvolatile_trivially_copyable<volatile T> : bool_constant<false>{};
}
template<typename T> struct is_nonvolatile_trivially_copyable : details::is_nonvolatile_trivially_copyable<T>{};
template<typename T> inline constexpr bool is_nonvolatile_trivially_copyable_v = is_nonvolatile_trivially_copyable<T>::value;

namespace details
{
    template<typename Iter1, typename Iter2> struct is_memcmpable : bool_constant<false> {};
    template<typename T, typename U> struct is_memcmpable<T*, U*> 
        : bool_constant<is_nonvolatile_trivially_copyable_v<T> && is_nonvolatile_trivially_copyable_v<U>> {};
}
template<typename Iter1, typename Iter2> struct is_memcmpable : details::is_memcmpable<Iter1, Iter2> {};

//TODO: May have problems
namespace details
{
    template<typename Void, typename T, typename... Args>
    struct is_nothrow_constructible;

    template<typename T, typename... Args>
    struct is_nothrow_constructible<void, T, Args...> : bool_constant<noexcept(T(declval<Args>()...))> {};

    template<typename T, size_t Num>
    struct is_nothrow_constructible<void, T[Num]> : is_nothrow_constructible<void, remove_extent_t<T>> {};
}
template<typename T, typename... Args> struct is_nothrow_constructible : details::is_nothrow_constructible<void, T, Args...> {};
template<typename T, typename... Args> inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;


template<typename T> struct is_trivial : bool_constant<is_trivially_copyable_v<T> && is_trivially_default_constructible_v<T>> {};
template<typename T> inline constexpr bool is_trivial_v = is_trivial<T>::value;

namespace details
{
    template<typename Void, typename T, typename U> struct is_assignable : false_type {};
    
    template<typename T, typename U>
    struct is_assignable<void_t<decltype(declval<T>() = declval<U>())>, T, U> : true_type {};
}
template<typename T, typename U> struct is_assignable : details::is_assignable<void, T, U> {};
template<typename T, typename U> inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

template<typename T> struct is_copy_assignable : is_assignable<add_lvalue_reference_t<T>, add_lvalue_reference_t<T const>> {};  // T& = T const&
template<typename T> inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;


namespace details
{
    template<typename Base>
    true_type test_ptr_convertible(const volatile Base*);

    template<typename>
    false_type test_ptr_convertible(...);

    template<typename, typename>
    true_type test_is_base_of(...); //This is for private inheritance, because the next overload will fail in private inheritance

    template<typename Base, typename Derived>
    decltype(test_ptr_convertible<Base>(static_cast<Derived*>(nullptr))) test_is_base_of(int);
}
template<typename Base, typename Derived>
struct is_base_of : bool_constant<is_class_v<Base>&& is_class_v<Derived>&& decltype(details::test_is_base_of<Base, Derived>(0))::value> {};
template<typename Base, typename Derived> inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;