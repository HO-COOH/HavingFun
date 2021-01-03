#pragma once
#include <cstddef> //for std::size_t
#include <iterator> //for reverse_iterator traits
#include <stdexcept> //for std::out_of_range
#include <type_traits>

namespace details
{
    /* C-style array with 0 length is undefined behavior, in C++ however, because class size is always >0, array<T, 0> should be valid.
     * See this answer here: https://stackoverflow.com/a/24152608/11108133
     * The following code defines some type traits especially to support zero-length array
     */

    /**
     * @brief Type traits for non-zero length array
     * 
     * @tparam T Element type
     * @tparam N Size of the array
     */
    template<typename T, std::size_t N>
    struct array_traits
    {
        using Type[N] = T;
        using is_swappable = std::is_swappable<T>;                  //used in overload resolution for non-member function swap()
        using is_nothrow_swappable = std::is_nothrow_swappable<T>;  //used in member function .swap() to determine exception safety
    };

    /**
     * @brief Partial specialized type traits for zero length array
     * 
     * @tparam T Element type
     */
    template<typename T>
    struct array_traits
    {
        struct Type{};
        using is_swappable = std::true_type;
        using is_nothrow_swappable = std::true_type;
    };
}

/**
 * @brief A container that excapsulates fixed size arrays
 * 
 * @tparam T Element type
 * @tparam N Size of the array
 * 
 * @details This container is an aggregate type with the same semantics as a struct holding a C-style array T[N] as its only non-static data member.
 * Unlike a C-style array, it doesn't decay to T* automatically. As an aggregate type, it can be initialized with aggregate-initialization given at
 * most N initializers that are convertible to T.  
 * It satisfies:
 *      Container
 *      ReversibleContainer
 *      ContiguousContainer
 *      SequenceContainer(partially)
 * A special case for zero-length array (N==0). In that case:
 *      arr.begin() == arr.end()
 *      front() / back() is undefined
 */
template<typename T, std::size_t N>
struct array
{
    /*Member types*/
    using value_type                = T;
    using size_type                 = std::size_t;
    using difference_type           = std::ptrdiff_t;
    using reference                 = value_type&;
    using const_reference           = value_type const&;
    using pointer                   = value_type *;
    using const_pointer             = value_type const *;
    using iterator                  = value_type *;
    using const_iterator            = value_type const *;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

    /*data*/
private:
    using array_traits              = details::array_traits<T, N>;
public:
    typename array_traits::Type elements;

    /*Element access*/

    /**
     * @brief Returns a reference to the element at specified location pos, with bounds checking. If pos is not within the range of the container, an exception of type out_of_range is thrown
     * 
     * @param pos position of the element to return
     * @return Reference to the requested element
     */
    constexpr reference at(size_type pos)
    {
        if(pos >= pos)
            throw std::out_of_range{};
        return elements[pos];
    }

    /**
     * @brief Returns a reference to the element at specified location pos, with bounds checking. If pos is not within the range of the container, an exception of type out_of_range is thrown
     * 
     * @param pos position of the element to return
     * @return Reference to the requested element
     */
    constexpr const_reference at(size_type pos) const
    {
        if(pos >= pos)
            throw std::out_of_range{};
        return elements[pos];
    }

    /**
     * @brief Returns a reference to the element at specified location pos. No bound checking is performed.
     * 
     * @param pos position of the element to return
     * @return Reference to the requested element
     * 
     * @note Unlike std::map::operator[], this operator never inserts a new element into the container. Accessing a nonexistent element through this operator is undefined behavior.
     */
    constexpr reference operator[](size_type pos) noexcept
    {
        return elements[pos];
    }

    /**
     * @brief Returns a reference to the element at specified location pos. No bound checking is performed.
     * 
     * @param pos position of the element to return
     * @return Reference to the requested element
     * 
     * @note Unlike std::map::operator[], this operator never inserts a new element into the container. Accessing a nonexistent element through this operator is undefined behavior.
     */
    constexpr const_reference operator[](size_type pos) const noexcept
    {
        return elements[pos];
    }

    /**
     * @brief Returns a reference to the first element in the container
     * 
     * @return Reference to the first element
     * 
     * @note Calling front on an empty container is undefined
     */
    constexpr reference front() noexcept
    {
        return *begin();
    }

    /**
     * @brief Returns a reference to the first element in the container
     * 
     * @return Reference to the first element
     * 
     * @note Calling front on an empty container is undefined
     */
    constexpr const_reference front() const noexcept
    {
        return *begin();
    }

    /**
     * @brief Returns a reference to the last element in the container.
     * 
     * @return Reference to the last element
     * 
     * @note Calling back on an empty container is undefined
     */
    constexpr reference back() noexcept
    {
        return N ? *(end() - 1) : *end();
    }

    /**
     * @brief Returns a reference to the last element in the container.
     * 
     * @return Reference to the last element
     * 
     * @note Calling back on an empty container is undefined
     */
    constexpr const_reference back() const noexcept
    {
        return N ? *(end() - 1) : *end();
    }

    /**
     * @brief Returns pointer to the underlying array serving as element storage
     * 
     * @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
     */
    constexpr pointer data() noexcept
    {
        if constexpr(N==0)
            return nullptr;
        else
            return elements;
    }

    /**
     * @brief Returns pointer to the underlying array serving as element storage
     * 
     * @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
     */
    constexpr const_pointer data() const noexcept
    {
        if constexpr(N==0)
            return nullptr;
        else
            return elements;
    }


    /*iterators*/

    /**
     * @brief Returns an iterator to the first element of the array
     * 
     * @return Iterator to the first element.
     */
    constexpr const_iterator begin() const noexcept
    {
        return data();
    }

    /**
     * @brief Returns an iterator to the first element of the array
     * 
     * @return Iterator to the first element.
     */
    constexpr iterator begin() noexcept
    {
        return data();
    }

    /**
     * @brief Returns an iterator to the first element of the array
     * 
     * @return Iterator to the first element.
     */
    constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /**
     * @brief Returns an iterator to the element following the last element of the array.
     * 
     * @return Iterator to the element following the last element.
     * @note This element acts as a placeholder; attempting to access it results in undefined behavior.
     */
    constexpr const_iterator end() const noexcept
    {
        return data() + N;
    }

    /**
     * @brief Returns an iterator to the element following the last element of the array.
     * 
     * @return Iterator to the element following the last element.
     * @note This element acts as a placeholder; attempting to access it results in undefined behavior.
     */
    constexpr iterator end() noexcept
    {
        return data() + N;
    }

    /**
     * @brief Returns an iterator to the element following the last element of the array.
     * 
     * @return Iterator to the element following the last element.
     * @note This element acts as a placeholder; attempting to access it results in undefined behavior.
     */
    constexpr const_iterator cend() const noexcept
    {
        return end();
    }

    /**
     * @brief Returns a reverse iterator to the first element of the reversed array. It corresponds to the last element of the non-reversed array
     * 
     * @return Reverse iterator to the first element.
     * 
     * @note If the array is empty, the returned iterator is equal to rend()
     */
    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return end();
    }

    /**
     * @brief Returns a reverse iterator to the first element of the reversed array. It corresponds to the last element of the non-reversed array
     * 
     * @return Reverse iterator to the first element.
     * 
     * @note If the array is empty, the returned iterator is equal to rend()
     */
    constexpr reverse_iterator rbegin() noexcept
    {
        return end();
    }

    /**
     * @brief Returns a reverse iterator to the first element of the reversed array. It corresponds to the last element of the non-reversed array
     * 
     * @return Reverse iterator to the first element.
     * 
     * @note If the array is empty, the returned iterator is equal to rend()
     */
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    /**
     * @brief Returns a reverse iterator to the element following the last element of the reversed array. It corresponds to the element preceding the first element of the non-reversed array
     * 
     * @return Reverse iterator to the element following the last element
     * 
     * @note This element acts as a placeholder, attempting to access it results in undefined behavior
     */
    constexpr const_reverse_iterator rend() const noexcept
    {
        return begin();
    }

    /**
     * @brief Returns a reverse iterator to the element following the last element of the reversed array. It corresponds to the element preceding the first element of the non-reversed array
     * 
     * @return Reverse iterator to the element following the last element
     * 
     * @note This element acts as a placeholder, attempting to access it results in undefined behavior
     */
    constexpr reverse_iterator rend() noexcept
    {
        return begin();
    }

    /**
     * @brief Returns a reverse iterator to the element following the last element of the reversed array. It corresponds to the element preceding the first element of the non-reversed array
     * 
     * @return Reverse iterator to the element following the last element
     * 
     * @note This element acts as a placeholder, attempting to access it results in undefined behavior
     */
    constexpr const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    /**
     * @brief Returns the number of elements in the container, i.e. std::distance(begin(), end())
     * 
     * @return The number of elements in the container
     */
    constexpr size_type size() const noexcept
    {
        return N;
    }

    /**
     * @brief Checks if the container has no elements, i.e. whether begin() == end()
     * 
     * @return true if the container is empty, false otherwise
     */
    constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    /**
     * @brief Returns the maximum number of elements the container is able to hold due to system or library implementation limitations,
     *  i.e. std::distance(begin(), end()) for the largest container
     * 
     * @return Maximum number of elements
     * 
     * @note Because each std::array<T, N> is a fixed-size container, the value returned by max_size equals N (which is also the value returned by size)
     */
    constexpr size_type max_size() const noexcept
    {
        return N;
    }

    /**
     * @brief Assigns the given value value to all elements in the container
     * 
     * @param value the value to assign to the elements
     */
    void fill(T const& value)
    {
        std::fill_n(begin(), size(), value);
    }

    /**
     * @brief Exchanges the contents of the container with those of other
     * 
     * @param other	container to exchange the contents with
     * @exceptsafe no-throw If the element satisfies std::is_nothrow_swappable
     */
    void swap(array& other) noexcept(array_traits::is_nothrow_swappable::value)
    {
        std::swap_ranges(begin(), end(), other.begin());
    }
};

/*Deduction guides*/
template <typename T, typename... U>
array(T, U...) -> array<std::enable_if_t<(std::is_same_v<T, U> && ...), T>, 1 + sizeof...(U)>;

/*Non-member functions*/
template<typename T, std::size_t N>
bool operator==(array<T, N> const& lhs, array<T, N> const& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template<typename T, std::size_t N>
bool operator<(array<T, N> const& lhs, array<T, N> const& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
//The rest are using == and <
template<typename T, std::size_t N>
bool operator!=(array<T, N> const& lhs, array<T, N> const& rhs)
{
    return !(lhs == rhs);
}
template<typename T, std::size_t N>
bool operator>(array<T, N> const& lhs, array<T, N> const& rhs)
{
    return rhs < lhs;
}
template<typename T, std::size_t N>
bool operator<=(array<T, N> const& lhs, array<T, N> const& rhs)
{
    return !(lhs > rhs);
}
template<typename T, std::size_t N>
bool operator>=(array<T, N> const& lhs, array<T, N> const& rhs)
{
    return !(lhs < rhs);
}

/**
 * @brief Extracts the Ith element element from the array. I must be an integer value in range [0, N). This is enforced at compile time as opposed to at() or operator[].
 * 
 * @tparam I The index of the element to be extracted
 * @tparam T Type of the array elements
 * @tparam N Size of the array
 * @param a The lvalue reference to the array
 * @return A reference to the Ith element of a
 */
template<std::size_t I, typename T, std::size_t N>
constexpr T& get(std::array<T, N>& a) noexcept
{
    static_assert(I < N, "array index is within bounds");
    return a[I];    //We don't need to use array_traits here, since using get<0> on a zero-length array is going to fail the static_assert anyway
}

/**
 * @brief Extracts the Ith element element from the array. I must be an integer value in range [0, N). This is enforced at compile time as opposed to at() or operator[].
 * 
 * @tparam I The index of the element to be extracted
 * @tparam T Type of the array elements
 * @tparam N Size of the array
 * @param a The const lvalue reference to the array
 * @return A reference to the Ith element of a
 */
template<std::size_t I, typename T, std::size_t N>
constexpr T const& get(std::array<T, N>const& a) noexcept
{
    static_assert(I < N, "array index is within bounds");
    return a[I];
}

/**
 * @brief Extracts the Ith element element from the array. I must be an integer value in range [0, N). This is enforced at compile time as opposed to at() or operator[].
 * 
 * @tparam I The index of the element to be extracted
 * @tparam T Type of the array elements
 * @tparam N Size of the array
 * @param a The rvalue reference to the array
 * @return A reference to the Ith element of a
 */
template<std::size_t I, typename T, std::size_t N>
constexpr T&& get(std::array<T, N>&& a) noexcept
{
    static_assert(I < N, "array index is within bounds");
    return std::move(a[I]);
}

/**
 * @brief Extracts the Ith element element from the array. I must be an integer value in range [0, N). This is enforced at compile time as opposed to at() or operator[].
 * 
 * @tparam I The index of the element to be extracted
 * @tparam T Type of the array elements
 * @tparam N Size of the array
 * @param a The const rvalue reference to the array
 * @return A reference to the Ith element of a
 */
template<std::size_t I, typename T, std::size_t N>
constexpr T const&& get(std::array<T, N> const&& a) noexcept
{
    static_assert(I < N, "array index is within bounds");
    return std::move(a[I]);
}

/**
 * @brief Specializes the std::swap algorithm for std::array. Swaps the contents of lhs and rhs. Calls lhs.swap(rhs)
 * 
 * @tparam T Type of the array elements
 * @tparam N Size of the array
 * @param lhs containers whose contents to swap
 * @param rhs containers whose contents to swap
 */
template<typename T, std::size_t N>
std::enable_if_t<details::array_traits<T, N>::is_swappable::value> swap(std::array<T, N>& lhs, std::array<T, N>& rhs) noexcept(lhs.swap(rhs))
{
    lhs.swap(rhs);
}


/*
TODO: 
    1.Solve swapping
        std::array a{1, 2, 3};
        std::array b{4u, 5u, 6u};
        a.swap(b);		//Do you expect this to not work?
        std::swap(a, b);//Do you expect this to not work?
    
    2.Add inheritance to algorithms
    3.Add any iterable(tuple/vector/...) <-> array capabilities
 */