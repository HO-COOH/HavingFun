#pragma once

#include "IteratorTraits.hpp"
#include "TypeTraits.hpp"

/**
 * @brief an iterator adaptor which behaves exactly like the underlying iterator (which must be at least an LegacyInputIterator, 
 * except that dereferencing converts the value returned by the underlying iterator into an rvalue
 * 
 * @tparam Iter 
 */
template<typename Iter>
class move_iterator
{
    Iter m_current;
    using base_reference = iterator_traits<Iter>::reference;
public:
    using iterator_type     = Iter;
    using difference_type   = iterator_traits<Iter>::difference_type;
    using value_type        = iterator_traits<Iter>::value_type;
    using pointer           = Iter;
    using reference         = std::conditional_t<is_reference<base_reference>, remove_reference_t<base_reference>, base_reference>;
    using iterator_category = iterator_traits<Iter>::iterator_category;

    constexpr move_iterator() : m_current{} {}
    explicit constexpr move_iterator(iterator_type iter) : m_current{std::move(iter) } {}
    template<typename U> constexpr move_iterator(move_iterator<U> const& other) : m_current{other.base()} {}

    /*internal iterator access*/
    constexpr iterator_type base() const { return m_current; }

    /*Forward iterator requirements*/
    constexpr reference operator*() const    { return static_cast<reference>(base()); }
    constexpr pointer operator->() const     { return base(); }
    
};