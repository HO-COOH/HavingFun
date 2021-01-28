#pragma once
namespace details
{
    template <typename T1, typename T2>
    struct merge;

    template <template <typename...> class TypeList, typename... Ts1, typename... Ts2>
    struct merge<TypeList<Ts1...>, TypeList<Ts2...>>
    {
        using type = TypeList<Ts1..., Ts2...>;
    };

    template <typename T, typename P>
    struct prepend
    {
    };

    template <typename T, template <typename...> class P, typename... Packs>
    struct prepend<T, P<Packs...>>
    {
        using type = P<typename merge<P<T>, Packs>::type...>;
    };

    // N is the number of rotations to go
    template <std::size_t N, typename Pack, typename = void>
    struct permutations_impl
    {
    };

    template <template <typename...> class P, typename... Ts>
    struct permutations_impl<0, P<Ts...>>
    {
        // All rotations done, break the recursion
        using type = P<>;
    };

    template <std::size_t N, template <typename...> class P, typename T>
    struct permutations_impl<N, P<T>>
    {
        using type = P<P<T>>;
    };

    template <std::size_t N, template <typename...> class P, typename F, typename... Rest>
    struct permutations_impl<N, P<F, Rest...>, std::enable_if_t<(sizeof...(Rest) && N != 0)>>
    {
        using PermuteRest = typename permutations_impl<sizeof...(Rest), P<Rest...>>::type;
        using NextRotation = typename permutations_impl<N - 1, P<Rest..., F>>::type;

        using type = typename merge<typename prepend<F, PermuteRest>::type, NextRotation>::type;
    };

    template <typename TypeToRemove, typename TypeList, typename...>
    struct remove_type_impl;

    template <typename TypeToRemove, typename TypeList, typename Front, typename... Rest>
    struct remove_type_impl<TypeToRemove, TypeList, Front, Rest...>
    {
        using type = std::conditional_t<
            std::is_same_v<Front, TypeToRemove>,
            typename merge<TypeList, typename remove_type_impl<TypeToRemove, TypeList, Rest...>::type>::type,
            typename merge<typename TypeList::template append<Front>, typename remove_type_impl<TypeToRemove, TypeList, Rest...>::type>::type>;
    };
    template <typename TypeToRemove, typename TypeList, typename Front>
    struct remove_type_impl<TypeToRemove, TypeList, Front>
    {
        using type = std::conditional_t<
            std::is_same_v<Front, TypeToRemove>,
            TypeList,
            typename TypeList::template append<Front>>;
    };
} // namespace details

template <typename... T>
struct type_list
{
private:
    template <unsigned N, typename...>
    struct n_th_type_impl;

    template <unsigned N, typename Front, typename... Rest>
    struct n_th_type_impl<N, Front, Rest...>
    {
        using type = typename n_th_type_impl<N - 1, Rest...>::type;
    };
    template <typename Front, typename... Rest>
    struct n_th_type_impl<0, Front, Rest...>
    {
        using type = Front;
    };

public:
    template <template <typename...> typename ToType>
    using to = ToType<T...>;

    template <typename U>
    using prepend = type_list<U, T...>;

    template <typename... U>
    using append = type_list<T..., U...>;

    using permutations = typename details::permutations_impl<sizeof...(T), type_list<T...>>::type;

    template <typename TypeList>
    using merge = typename details::merge<type_list<T...>, TypeList>::type;

    template <typename TypeToRemove>
    using remove_type = typename details::remove_type_impl<TypeToRemove, type_list<>, T...>::type;

    template <unsigned N, typename = std::enable_if_t<N<sizeof...(T)>> using n_th_type = typename n_th_type_impl<N, T...>::type;

    static constexpr auto size = sizeof...(T);
};