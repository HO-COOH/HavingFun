#pragma once

#include <cstddef>
#include "TypeTraits.hpp"

namespace details
{
    template<typename Iter, typename = void>
    struct has_iterator_category : false_type {};

    template<typename Iter>
    struct has_iterator_category<Iter, void_t<typename Iter::iterator_category>> : true_type {};

    // template<typename Iter>
    

    template<typename Iter, typename = void>
    struct is_input_iterator : false_type
    {
    };

    template<typename Iter>
    struct is_input_iterator<Iter, decltype(declval<Iter>() != declval<Iter>(), *(declval<Iter>()), ++(declval<Iter>()), void())> : true_type {};

    template<typename Iter>
    struct is_output_iterator {};

    template<typename Iter>
    struct is_forward_iterator {};

    template<typename Iter>
    struct is_bidirectional_iterator {};

    template<typename Iter>
    struct is_random_access_iterator {};
}

///  Marking input iterators.
struct input_iterator_tag { };
///  Marking output iterators.
struct output_iterator_tag { };
/// Forward iterators support a superset of input iterator operations.
struct forward_iterator_tag : public input_iterator_tag { };
/// Bidirectional iterators support a superset of forward iterator
/// operations.
struct bidirectional_iterator_tag : public forward_iterator_tag { };
/// Random-access iterators support a superset of bidirectional
/// iterator operations.
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

/**
 * @brief If Iter does not have all five member types 
 *      difference_type, 
 *      value_type, 
 *      pointer, 
 *      reference, and 
 *      iterator_category, 
 * then this template has no members by any of those names 
 * (std::iterator_traits is SFINAE-friendly)
 */
template<typename Iter, typename = void>
struct iterator_traits
{};

/**
 * @brief Type trait class that provides uniform interface to the properties of LegacyIterator types
 * 
 * @tparam Iter the iterator type to retrieve properties for
 * 
 * @details LegacyIterator:
 *              CopyConstructible
 *              CopyAssignable
 *              Destructible
 *              Swappable for lvalue
 *              std::iterator_traits<Iter> has member typedefs:
 *                  value_type
 *                  difference_type
 *                  reference
 *                  pointer
 *                  iterator_category
 *             Given r, an lvalue of type Iter, the following expressions must be valid:
 *                  *r
 *                  ++r
 */
template<typename Iter>
struct iterator_traits
<
    Iter, void_t
    <
        typename Iter::difference_type,
        typename Iter::value_type,
        typename Iter::pointer,
        typename Iter::reference,
        typename Iter::category
    >
>
{
    /*Member types*/
    using difference_type   = typename Iter::difference_type;
    using value_type        = typename Iter::value_type;
    using pointer           = typename Iter::pointer;
    using reference         = typename Iter::reference;
    using iterator_category = typename Iter::category;
};

/**
 * @brief Partial specialization for pointer types T*
 */
template<typename T>
struct iterator_traits<T*>
{
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = random_access_iterator_tag;
};

/**
 * @brief Partial specialization for pointer types T const*, because we need value_type to be T, not T const
 */
template<typename T>
struct iterator_traits<T const*>
{
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T const*;
    using reference         = T const&;
    using iterator_category = random_access_iterator_tag;
};