#pragma once
#include "TMP.hpp"

template<typename Key, typename Value>
struct pair
{
    using key = Key;
    using value = Value;
};
//                  v   This is fucked
//template<typename Enum, Enum e, typename Value>
//struct Pair
//{
//    constexpr static Enum Key = e;
//    using Value = Value;
//};

template<typename...Pairs>
struct type_map
{
private:

public:
    using KeyTypes = type_list<typename Pairs::key...>;
    using ValueTypes = type_list<typename Pairs::value...>;

    template<typename Key>
    using value_of = std::conditional_t<
        KeyTypes:: template index_of<Key> >= 0,
        typename ValueTypes::template n_th_type<KeyTypes::template index_of<Key>>,
        void
    >;

    template<int I>
    using n_th_key = typename KeyTypes::n_th_type<I>;

    template<int I>
    using n_th_value = typename ValueTypes::n_th_type<I>;

    
};