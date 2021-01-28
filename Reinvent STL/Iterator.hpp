#pragma once
#include "IteratorTraits.hpp"
#include "PointerTraits.hpp"

/**
 * @brief Adapt a pointer to a class
 *
 * @tparam Iter
 * @tparam Container
 */
template<typename Iter, typename Container>
class normal_iterator : public iterator_traits<Iter>    //so we don't have traits duplication
{
protected:
    Iter current;
    using traits = iterator_traits<Iter>;
public:
    using container = Container;
    constexpr normal_iterator() noexcept : current{ Iter{} } {}
    explicit constexpr normal_iterator(Iter const& i) noexcept : current{ i } {}
    //const normal_iterator -> normal_iterator?

    /*internal iterator access*/
    constexpr Iter const& base() const noexcept { return current; }

    /*Forward iterator requirements*/
    constexpr reference operator*() const noexcept { return *current; }
    constexpr pointer operator->() const noexcept { return *current; }
    constexpr normal_iterator& operator++() noexcept { ++current; return *this; }
    constexpr normal_iterator operator++(int) noexcept { return normal_iterator{ current++ }; }

    /*Bidirectional iterator requirements*/
    constexpr normal_iterator& operator--() noexcept { --current; return *this; }
    constexpr normal_iterator operator--(int) noexcept { return normal_iterator{ current-- }; }

    /*Random access iterator requirements*/
    constexpr reference operator[](difference_type n) const noexcept { return current[n]; }
    constexpr normal_iterator& operator+=(difference_type n) noexcept { current += n; return *this; }
    constexpr normal_iterator operator+(difference_type n) const noexcept { return normal_iterator{ current + n }; }
    constexpr normal_iterator& operator-=(difference_type n) noexcept { current -= n; return *this; }
    constexpr normal_iterator operator-(difference_type n) const noexcept { return normal_iterator{ current - n }; }
};

template<typename Iter1, typename Iter2, typename Container>
constexpr bool operator==(const normal_iterator<Iter1, Container>& __lhs,
    const normal_iterator<Iter2, Container>& __rhs)
    noexcept
{
    return __lhs.base() == __rhs.base();
}

template<typename _Iterator, typename Container>
constexpr bool
operator==(const normal_iterator<_Iterator, Container>& __lhs,
    const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
{
    return __lhs.base() == __rhs.base();
}

template<typename Iter1, typename Iter2, typename Container>
constexpr bool
operator!=(const normal_iterator<Iter1, Container>& __lhs,
    const normal_iterator<Iter2, Container>& __rhs)
    noexcept
{
    return __lhs.base() != __rhs.base();
}

template<typename _Iterator, typename Container>
constexpr bool
operator!=(const normal_iterator<_Iterator, Container>& __lhs,
    const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
{
    return __lhs.base() != __rhs.base();
}

// Random access iterator requirements
template<typename Iter1, typename Iter2, typename Container> bool
operator<(const normal_iterator<Iter1, Container>& __lhs,
    const normal_iterator<Iter2, Container>& __rhs)
    noexcept
{
    return __lhs.base() < __rhs.base();
}

template<typename _Iterator, typename Container>
constexpr bool
operator<(const normal_iterator<_Iterator, Container>& __lhs,
    const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
{
    return __lhs.base() < __rhs.base();
}

template<typename Iter1, typename Iter2, typename Container> bool
operator>(const normal_iterator<Iter1, Container>& __lhs,
    const normal_iterator<Iter2, Container>& __rhs)
    noexcept
{
    return __lhs.base() > __rhs.base();
}

template<typename _Iterator, typename Container>
constexpr bool
operator>(const normal_iterator<_Iterator, Container>& __lhs,
    const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
{
    return __lhs.base() > __rhs.base();
}

template<typename Iter1, typename Iter2, typename Container> bool
operator<=(const normal_iterator<Iter1, Container>& __lhs,
    const normal_iterator<Iter2, Container>& __rhs)
    noexcept
{
    return __lhs.base() <= __rhs.base();
}

template<typename _Iterator, typename Container>
constexpr bool
operator<=(const normal_iterator<_Iterator, Container>& __lhs,
    const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
{
    return __lhs.base() <= __rhs.base();
}

template<typename Iter1, typename Iter2, typename Container> bool
operator>=(const normal_iterator<Iter1, Container>& __lhs,
    const normal_iterator<Iter2, Container>& __rhs)
    noexcept
{
    return __lhs.base() >= __rhs.base();
}

template<typename _Iterator, typename Container>
constexpr bool
operator>=(const normal_iterator<_Iterator, Container>& __lhs,
    const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
{
    return __lhs.base() >= __rhs.base();
}


template<typename Iter>
class reverse_iterator
{
protected:
    Iter current;
public:
    /*Member types*/
    using iterator_type = Iter;
    using difference_type = typename iterator_traits<Iter>::difference_type;
    using value_type = typename iterator_traits<Iter>::value_type;
    using pointer = typename iterator_traits<Iter>::pointer;
    using reference = typename iterator_traits<Iter>::reference;
    using iterator_category = typename iterator_traits<Iter>::iterator_category;

    /*constructor*/
    constexpr reverse_iterator() = default;
    explicit constexpr reverse_iterator(iterator_type x) : current{x} {}
    template<typename U>
    reverse_iterator(reverse_iterator<U> const& other) : current{other.base()} {}

    template<typename U>
    constexpr reverse_iterator& operator=(reverse_iterator<U> const& other) { current = other.base(); return *this; }

    /*internal iterator access*/
    constexpr iterator_type base() const { return current; }

    /*Forward iterator requirements*/
    constexpr reference operator*() const 
    { 
        Iter tmp = current;
        return *--tmp;  
        /* Why not directly return *(m_current - 1) you ask?
         * Because the requirments for Iter is a LegacyBidirectionalIterator, and (m_current - 1) is only possible for RandomAccessIterator
         */
    }

    constexpr pointer operator->() const 
    { 
        return addressof(operator*());
    }

    /*Bidirectional iterator requirements*/
    constexpr reverse_iterator& operator++() { --current; return *this; }
    constexpr reverse_iterator& operator--() { ++current; return *this; }
    constexpr reverse_iterator& operator++(int) { reverse_iterator tmp = *this; --current; return tmp; }
    constexpr reverse_iterator& operator--(int) { reverse_iterator tmp = *this; ++current; return tmp; }
    constexpr reverse_iterator operator+(difference_type n) const { return reverse_iterator{ current - n }; }
    constexpr reverse_iterator operator-(difference_type n) const { return reverse_iterator{ current + n }; }
    constexpr reverse_iterator& operator+=(difference_type n) { current -= n; return *this; }
    constexpr reverse_iterator& operator-=(difference_type n) { current += n; return *this; }

    /*Random access iterator requirements*/
    constexpr reference operator[](difference_type n) const { return base()[-n - 1]; }
};

template<typename Iterator1, typename Iterator2>
inline constexpr bool operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() == rhs.base();
}

template<typename Iterator1, typename Iterator2>
inline constexpr bool operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() > rhs.base();
}

template<typename Iterator1, typename Iterator2>
inline constexpr bool operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() != rhs.base();
}

template<typename Iterator1, typename Iterator2>
inline constexpr bool operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() < rhs.base();
}

template<typename Iterator1, typename Iterator2>
inline constexpr bool operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() >= rhs.base();
}

template<typename Iterator1, typename Iterator2>
inline constexpr bool operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() <= rhs.base();
}

/**
 * @brief Returns the iterator it incremented by n
 * @notes This operator function is for expressions like: auto iter = 3 + it;
 */
template<typename Iter>
constexpr reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, reverse_iterator<Iter> const& it)
{
    return reverse_iterator<Iter>{it.base() - n};
}

/**
 * @brief Returns the distance between 2 reverse_iterator
 */
template<typename Iterator1, typename Iterator2>
constexpr auto operator-(reverse_iterator<Iterator1> const& lhs, reverse_iterator<Iterator2> const& rhs) -> decltype(rhs.base() - lhs.base())
{
    return rhs.base() - lhs.base();
}

/**
 * @brief Returns the number of hops from first -> last.
 * @param first iterator pointing to the first element
 * @param last iterator pointing to the end of the range
 */
template<typename InputIt>
constexpr typename iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last)
{
    using category = typename iterator_traits<InputIt>::iterator_category;
    static_assert(is_base_of_v<input_iterator_tag, category>, "distance() requires at least input_iterator");

    if constexpr (is_base_of_v<random_access_iterator_tag, category>)
        return last - first;
    else
    {
        typename std::iterator_traits<InputIt>::difference_type result{};
        while (first != last)
        {
            ++first;
            ++result;
        }
        return result;
    }
}