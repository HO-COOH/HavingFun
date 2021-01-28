#pragma once
#include "TypeTraits.hpp"
#include "PointerTraits.hpp"
#include "Utility.hpp"
#include <limits>

/**
 * @brief Provides the standardized way to access various properties of Allocators
 * 
 * @tparam Alloc Type of the allocator
 * @details The standard containers and other standard library components access allocators through this template,
 * which makes it possible to use any class type as an allocator, as long as the user-provided specialization of
 * allocator_traits implements all required functionality
 */
template<typename Alloc>
struct allocator_traits
{
private:
    template <typename T> using m_pointer = typename T::pointer;

    template <typename T> using m_const_pointer = typename T::const_pointer;

    template <typename T> using m_void_pointer = typename T::void_pointer;

    template <typename T> using m_difference_type = typename T::difference_type;

    template <typename T> using m_size_type = typename T::size_type;

    template <typename T> using m_propagate_on_container_copy_assignment = typename T::propagate_on_container_copy_assignment;

    template <typename T> using m_propagate_on_container_move_assignment = typename T::propagate_on_container_move_assignment;
    
    template <typename T> using m_propagate_on_container_swap = typename T::propagate_on_container_swap;

    template <typename T> using m_is_always_equal = typename T::is_always_equal;

    template <typename T, typename U> using m_rebind_other = typename T::template rebind<U>::other;

    template <typename Void, typename Allocator, typename Pointer, typename... Args>
    struct can_construct : false_type{};
    template <typename Allocator, typename Pointer, typename... Args>
    struct can_construct<decltype(declval<Pointer>().construct(declval<Pointer*>(), declval<Args>()...), void()), Allocator, Pointer, Args...>
        : true_type{};

    template<typename Void, typename Allocator, typename Pointer> struct can_destroy : false_type{};
    template<typename Allocator, typename Pointer>
    struct can_destroy<decltype(declval<Allocator>().destory(declval<Pointer>()), void()), Allocator, Pointer> : true_type{};

    template<typename Void, typename Allocator> struct has_max_size : false_type{};
    template<typename Allocator> struct has_max_size<decltype(declval<Allocator>().max_size(), void()), Allocator> : true_type{};

    template<typename Void, typename Allocator> struct has_select_on_container_copy_construction : false_type{};
    template<typename Allocator>
    struct has_select_on_container_copy_construction<decltype(declval<Allocator>().select_on_container_copy_construction(), void()), Allocator>
        : true_type{};

public:
    using allocator_type                        = Alloc;
    using value_type                            = typename Alloc::value_type;
    using pointer                               = detected_or_t<value_type*, m_pointer, Alloc>;
    using const_pointer                         = detected_or_t<typename pointer_traits<pointer>::template rebind<const value_type>, m_const_pointer, Alloc>;
    using void_pointer                          = detected_or_t<typename pointer_traits<pointer>::template rebind<void>, m_void_pointer, Alloc>;
    using const_void_pointer                    = detected_or_t<typename pointer_traits<pointer>::template rebind<const void>, m_const_pointer, Alloc>;
    using difference_type                       = detected_or_t<typename pointer_traits<pointer>::difference_type, m_difference_type, Alloc>;
    using size_type                             = detected_or_t<typename make_unsigned<difference_type>::type, m_size_type, Alloc>;
    using propagate_on_container_copy_assignment= detected_or_t<false_type, m_propagate_on_container_copy_assignment, Alloc>;
    using propagate_on_container_move_assignment= detected_or_t<false_type, m_propagate_on_container_move_assignment, Alloc>;
    using propagate_on_container_swap           = detected_or_t<false_type, m_propagate_on_container_swap, Alloc>;
    using is_always_equal                       = detected_or_t<typename is_empty<Alloc>::type, m_is_always_equal, Alloc>;

    //<Alloc::rebind<T>::other> if present, otherwise <Alloc<T, Args>> if this Alloc is <Alloc<U, Args>>
    template<typename T>
    using rebind_alloc = detected_or_t<replace_first_template_type_t<Alloc, T>, m_rebind_other, Alloc, T>;

    template<typename T>
    using rebind_traits = rebind_alloc<T>;


    /**
     * @brief Use the allocator to allocate n*sizeof(Alloc::value_type) bytes of uninitialized storage
     * @param a allocator to use
     * @param n the number of objects to allocate storage for
     * @return The pointer returned by the call to a.allocate(n)
     */
    static pointer allocate(Alloc& a, size_type n)
    {
        return a.allocate(n);
    }

    /**
     * @brief Use the allocator to allocate n*sizeof(Alloc::value_type) bytes of uninitialized storage
     * @param a allocator to use
     * @param n the number of objects to allocate storage for
     * @param hint pointer to a nearby memory location
     * @return The pointer returned by the call to a.allocate(n)
     */
    static pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
    {
        return a.allocate(a, n, hint);
    }

    /**
     * @brief Uses the allocator a to deallocate the storage referenced by p, by calling a.deallocate(p, n)
     * @param a allocator to use
     * @param p pointer to the previously allocated storage
     * @param n the number of objects the storage was allocated for
     */
    static void deallocate(Alloc& a, pointer p, size_type n)
    {
        a.deallocate(p, n);
    }

    
    /**
     * @brief If possible, constructs an object of type T in allocated uninitialized storage pointed to by p, by calling
        a.construct(p, std::forward<Args>(args)...)
        If not, calls the global placement new operator
     * @param a allocator to use for construction
     * @param p pointer to the uninitialized storage on which a T object will be constructed
     * @param args the constructor arguments to pass to a.construct() or to placement-new
     */
    template<typename T, typename... Args>
    static void construct(Alloc& a, T* p, Args&&... args)
    {
        if constexpr (can_construct<void, Alloc, T, Args...>::value)
            a.construct(p, forward<Args>(args)...);
        else
            ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
    }


    /**
     * @brief Calls the destructor of the object pointed to by p.
     *  If possible, does so by calling a.destroy(p).
     *  If not possible (e.g. Alloc does not have the member function destroy()), then calls the destructor of *p directly, as p->~T()
     * @param a allocator to use for destruction
     * @param p pointer to the object being destroyed
     */
    template<typename T>
    static void destroy(Alloc& a, T* p)
    {
        if constexpr (can_destroy<void, Alloc, T>::value)
            a.destroy(p);
        else
            p->~T();
    }


    /**
     * @brief If possible, obtains the maximum theoretically possible allocation size from the allocator a, by calling a.max_size().
     *  If not possible (e.g. Alloc does not have the member function max_size()), then returns std::numeric_limits<size_type>::max() / sizeof(value_type)
     * @param a allocator to detect
     * @return Theoretical maximum allocation size
     */
    static size_type max_size(Alloc const& a) noexcept
    {
        if constexpr (has_max_size<void, Alloc>::value)
            return a.max_size();
        else
            return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }


    /**
     * @brief If possible, obtains the copy-constructed version of the allocator a, by calling a.select_on_container_copy_construction().
     *  If the above not possible (e.g. Alloc does not have the member function select_on_container_copy_construction(), then returns a, unmodified.
     * @param a allocator used by a standard container passed as an argument to a container copy constructor
     * @return The allocator to use by the copy-constructed standard containers
     */
    static Alloc select_on_container_copy_construction(Alloc const& a)
    {
        if constexpr (has_select_on_container_copy_construction<void, Alloc>::value)
            return a.select_on_container_copy_construction();
        else
            return a;
    }
};