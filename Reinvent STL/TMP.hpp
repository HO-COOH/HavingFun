#pragma once

#include <type_traits>
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

    template<typename ToFind, typename... Ts>
    struct index_of_type_impl;
    template<typename ToFind>
    struct index_of_type_impl<ToFind> :std::integral_constant<int, -1> {};
    template<typename ToFind, typename... Rest>
    struct index_of_type_impl<ToFind, ToFind, Rest...> :std::integral_constant<int, 0> {};
    template<typename ToFind, typename Front, typename... Rest>
    struct index_of_type_impl<ToFind, Front, Rest...> :std::integral_constant<int, 1 + index_of_type_impl<ToFind, Rest...>::value> {};


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
        static_assert(N <= sizeof...(Rest));
        using type = typename n_th_type_impl<N - 1, Rest...>::type;
    };
    template <typename Front, typename... Rest>
    struct n_th_type_impl<0, Front, Rest...>
    {
        using type = Front;
    };

    static constexpr auto GetThis() noexcept
    {
        return type_list<T...>{};
    }

    

    template<typename...U, typename... TypeLists>
    static constexpr auto append_impl(type_list<U...>, TypeLists...)
    {
        if constexpr (sizeof...(TypeLists) == 0)
            return type_list<T..., U...>{};
        else
            return typename type_list<T..., U...>::template append<TypeLists...>{};
    }

    template<typename U, typename...Rest>
    static constexpr auto append_impl(U, Rest...)
    {
        return typename type_list<T..., U>::template append<Rest...>{};
    }

    static constexpr auto append_impl()
    {
        return GetThis();
    }

    template<typename...U, typename... TypeLists>
    static constexpr auto prepend_impl(type_list<U...>, TypeLists...)
    {
        if constexpr (sizeof...(TypeLists) == 0)
            return type_list<U..., T...>{};
        else
            return typename type_list<U...>::template append<TypeLists...>::template append<T...>{};
    }

    template<typename U, typename... Rest>
    static constexpr auto prepend_impl(U, Rest...)
    {
        return typename type_list<U>::template append<Rest...>::template append<type_list<T...>>{};
    }

    static constexpr auto prepend_impl()
    {
        return GetThis();
    }

    template<int N, int Start, int End, typename...Types>
    static constexpr auto split_impl()
    {
        if constexpr (N < Start)
            return split_impl<N + 1, Start, End>();
        else if constexpr (N >= Start && N <= End)
            return split_impl<N + 1, Start, End, Types..., n_th_type<N>>();
        else
            return GetThis();
    }
public:
    
    static constexpr auto size() noexcept
    {
        return sizeof...(T);
    }
    
    template <template <typename...> typename ToType>
    using to = ToType<T...>;

    template <typename... U>
    using prepend = decltype(prepend_impl(std::declval<U>()...));

    template <typename... U>
    using append = decltype(append_impl(std::declval<U>()...));

    using permutations = typename details::permutations_impl<sizeof...(T), type_list<T...>>::type;

    template <typename TypeList>
    using merge = typename details::merge<type_list<T...>, TypeList>::type;

    template <typename TypeToRemove>
    using remove_type = typename details::remove_type_impl<TypeToRemove, type_list<>, T...>::type;

    template <unsigned N> 
    using n_th_type = typename n_th_type_impl<N, T...>::type;

    

    template<typename ToFind>
    static constexpr auto index_of = details::index_of_type_impl<ToFind, T...>::value;

    template<typename TypeMap>
    using transform = int;

    template<typename... U>
    friend struct type_list;
};