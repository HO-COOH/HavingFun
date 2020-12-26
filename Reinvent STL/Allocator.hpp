#pragma once
#include <cstddef>      //for std::size_t
#include <new>          //for placement new
#include <type_traits>  //for std::true_type
#include <limits>

/**
 * @brief Reinvent std::allocator using GCC's source code
 * 
 * @tparam T Type of allocated object
 * @details The implementation of GCC's std::allocator is in <ext/new_allocator.h>
 * And we will follow the C++17 standard (which, unironically, is simpler)
 */
template<typename T>
class allocator
{
public:
    /*Only 4 member typedefs are required in C++17 standard, all others are deprecated and removed in C++20*/
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
private:
    //GCC has this for allocate(), because the max_size() function is removed in C++20
    constexpr m_max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
public:
    /*Constructors*/
    allocator() noexcept = default;
    allocator(const allocator&) noexcept = default;
    template <typename U>
    allocator(const allocator<U> &) noexcept = default;

    /*Member functions*/

    /**
     * @brief Returns the maximum theoretically possible value of n, for which the call allocate(n) could succeed.
     */
    constexpr auto max_size() const noexcept
    {
        return m_max_size();
    }

    /**
     * @brief Allocates n*sizeof(T) bytes of uninitialized memory
     * 
     * @param n the number of objects to allocate storage for
     * @param hint pointer to a nearby memory location
     */
    auto allocate(std::size_t n, const void *hint = {0})
    {
        if(n > m_max_size())
            throw std::bad_alloc{};
        constexpr auto alignment = alignof(T);
        if constexpr(alignment > alignof(std::max_align_t))
            return static_cast<T*>(::operator new(n * sizeof(T), alignment));
        else
            return static_cast<T*>(::operator new(n * sizeof(T));
    }

    /**
     * @brief Deallocates the storage reference by the pointer p, which must be a pointer obtained by an earlier call to allocate
     * 
     * @param p pointer obtained from allocate
     * @param n number of objects earlier passed to allocate
     */
    void deallocate(T* p, std::size_t n)
    {
        constexpr auto alignment = alignof(T);
        if constexpr(alignment > alignof(std::max_align_t))
            ::operator delete(p, n * sizeof(T), alignment);
        else
            ::operator delete(p, n * sizeof(T));
    }
};

namespace details
{
    template<typename T, typename = void>
    struct HasValueType : std::false_type
    {
    };

    template<typename T>
    struct HasValueType<T, std::void_t<typename T::value_type>> : std::false_type
    {
    };

    template<typename Default, typename CheckHasValueType>
    struct DefinedValueTypeOr
    {
        using type = std::conditional_t<HasValueType<CheckHasValueType>::value, typename T::value_type, Default>;
    };

    template <typename Default, typename CheckHasValueType>
    using DefinedValueTypeOr_t = DefinedValueTypeOr<Default, CheckHasValueType>::type;
} // namespace details

/**
 * @brief Provides the standardized way to access various properties of Allocators
 * 
 * @tparam Alloc 
 * @details The standard containers and other standard library components access allocators through this template,
 * which makes it possible to use any class type as an allocator, as long as the user-provided specialization of
 * allocator_traits implements all required functionality
 */
template<typename Alloc>
struct allocator_traits
{
public:
    using allocator_type = Alloc;
    using value_type = Alloc::value_type;
};