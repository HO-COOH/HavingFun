#pragma once
#include <cstddef>
#include <cstring>
#include "TypeTraits.hpp"

namespace details
{
    /**
     * @returns 0 -> equal
     */
    template<typename T, typename U>
    constexpr int memcmp(T const* first1, U const* first2, std::size_t num)
    {
        //Seems GCC's typo that "can be compared" in the static_assert message, on <usr/include/c++/10/bits> Line 93
        static_assert(sizeof(T) == sizeof(U), "can't be compared with memcmp"); 
        return std::memcmp(first1, first2, sizeof(T) * num);
    }
}

template<typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
    //TODO: Why can we only use memcmp when it is either integer or pointer?
    if constexpr(!is_class_v<InputIt1> && !is_class_v<InputIt2> && is_memcmpable<InputIt1, InputIt2>::value)
    {
        if (const auto num = last1 - first1)
            return details::memcmp(first1, first2, num);
        return true;    //because num == 0
    }
    else
    {
        for (; first1 != last1; ++first1, (void)++first2)
        {
            if (!(*first1 == *first2))
                return false;
        }
        return true;
    }
}

namespace details
{
    template<typename InputIt1, typename InputIt2>
    bool lexicographical_compare_impl(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
    {
        
    }
}

/**
 * @brief Checks if the first range [first1, last1) is lexicographically less than the second range [first2, last2). Elements are compared using operator<
 * @param first1 the first range of elements to examine
 * @param last1 the first range of elements to examine
 * @param first2  the second range of elements to examine
 * @param last2 the second range of elements to examine
 * @return true if the first range is lexicographically < second range
 */
//TODO: utilize memcmp
template<typename InputIt1, typename InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
    for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2) 
    {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
    }
    //Comparison finished, either first1 reaches last1 || first2 reaches last2 || [first1, last1) == [first2, last2)
    //If first1 reaches last1, => true
    //If first2 reaches last2, => false
    //If [first1, last1) == [first2, last2), => false
    return (first1 == last1) && (first2 != last2);  
}

/**
 * @brief Checks if the first range [first1, last1) is lexicographically less than the second range [first2, last2).
 * Elements are compared using the given binary comparison function comp
 * @param first1 the first range of elements to examine
 * @param last1 the first range of elements to examine
 * @param first2  the second range of elements to examine
 * @param last2 the second range of elements to examine
 * @param compare comparison function object, which returns true if the first argument < second argument
 * @return true if the first range is lexicographically < second range
 */
template<typename InputIt1, typename InputIt2, typename Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare compare)
{
    for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2) 
    {
        if (compare(*first1, *first2)) return true;
        if (compare(*first2, *first1)) return false;
    }
    return (first1 == last1) && (first2 != last2);
}

