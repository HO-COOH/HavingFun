#pragma once
#include "IteratorTraits.hpp"

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
    Iter m_current;
    using traits = iterator_traits<Iter>;
public:
    using container = Container;
    constexpr normal_iterator() noexcept : m_current{Iter{}} {}
    explicit constexpr normal_iterator(Iter const& i) noexcept : m_current{i} {}
    //const normal_iterator -> normal_iterator?

    /*internal iterator access*/
    constexpr Iter const &base() const noexcept { return m_current; }

    /*Forward iterator requirements*/
    constexpr reference operator*() const noexcept      { return *m_current; }
    constexpr pointer operator->() const noexcept       { return *m_current; }
    constexpr normal_iterator& operator++() noexcept    { ++m_current; return *this; }
    constexpr normal_iterator operator++(int) noexcept  { return normal_iterator{m_current++}; }

    /*Bidirectional iterator requirements*/
    constexpr normal_iterator& operator--() noexcept    { --m_current; return *this; }
    constexpr normal_iterator operator--(int) noexcept  { return normal_iterator{m_current--}; }

    /*Random access iterator requirements*/
    constexpr reference operator[](difference_type n) const noexcept        { return m_current[n]; }
    constexpr normal_iterator& operator+=(difference_type n) noexcept       { m_current+=n; return *this; }
    constexpr normal_iterator operator+(difference_type n) const noexcept   { return normal_iterator{m_current + n}; }
    constexpr normal_iterator& operator-=(difference_type n) noexcept       { m_current-=n; return *this; }
    constexpr normal_iterator operator-(difference_type n) const noexcept   { return normal_iterator{m_current - n}; }
};

template<typename Iter1, typename Iter2, typename Container>
constexpr bool operator==(const normal_iterator<Iter1, Container>& __lhs,
	       const normal_iterator<Iter2, Container>& __rhs)
    noexcept
    { return __lhs.base() == __rhs.base(); }

  template<typename _Iterator, typename Container>
    constexpr bool
    operator==(const normal_iterator<_Iterator, Container>& __lhs,
	       const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
    { return __lhs.base() == __rhs.base(); }

  template<typename Iter1, typename Iter2, typename Container>
    constexpr bool
    operator!=(const normal_iterator<Iter1, Container>& __lhs,
	       const normal_iterator<Iter2, Container>& __rhs)
    noexcept
    { return __lhs.base() != __rhs.base(); }

  template<typename _Iterator, typename Container>
    constexpr bool
    operator!=(const normal_iterator<_Iterator, Container>& __lhs,
	       const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
    { return __lhs.base() != __rhs.base(); }

  // Random access iterator requirements
  template<typename Iter1, typename Iter2, typename Container> bool
    operator<(const normal_iterator<Iter1, Container>& __lhs,
	      const normal_iterator<Iter2, Container>& __rhs)
    noexcept
    { return __lhs.base() < __rhs.base(); }

  template<typename _Iterator, typename Container>
    constexpr bool
    operator<(const normal_iterator<_Iterator, Container>& __lhs,
	      const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
    { return __lhs.base() < __rhs.base(); }

  template<typename Iter1, typename Iter2, typename Container> bool
    operator>(const normal_iterator<Iter1, Container>& __lhs,
	      const normal_iterator<Iter2, Container>& __rhs)
    noexcept
    { return __lhs.base() > __rhs.base(); }

  template<typename _Iterator, typename Container>
    constexpr bool
    operator>(const normal_iterator<_Iterator, Container>& __lhs,
	      const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
    { return __lhs.base() > __rhs.base(); }

  template<typename Iter1, typename Iter2, typename Container> bool
    operator<=(const normal_iterator<Iter1, Container>& __lhs,
	       const normal_iterator<Iter2, Container>& __rhs)
    noexcept
    { return __lhs.base() <= __rhs.base(); }

  template<typename _Iterator, typename Container>
    constexpr bool
    operator<=(const normal_iterator<_Iterator, Container>& __lhs,
	       const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
    { return __lhs.base() <= __rhs.base(); }

  template<typename Iter1, typename Iter2, typename Container> bool
    operator>=(const normal_iterator<Iter1, Container>& __lhs,
	       const normal_iterator<Iter2, Container>& __rhs)
    noexcept
    { return __lhs.base() >= __rhs.base(); }

  template<typename _Iterator, typename Container>
    constexpr bool
    operator>=(const normal_iterator<_Iterator, Container>& __lhs,
	       const normal_iterator<_Iterator, Container>& __rhs)
    noexcept
    { return __lhs.base() >= __rhs.base(); }