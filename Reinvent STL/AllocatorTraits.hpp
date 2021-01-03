#pragma once
#include "TypeTraits.hpp"
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
    template <typename T> using m_pointer = typename T::pointer;

    template <typename T> using m_const_pointer = typename T::const_pointer;

    template <typename T> using m_void_pointer = typename T::void_pointer;

    template <typename T> using m_difference_type = typename T::difference_type;

    template <typename T> using m_size_type = typename T::size_type;

    template <typename T> using m_propagate_on_container_copy_assignment = typename T::propagate_on_container_copy_assignment;

    template <typename T> using m_propagate_on_container_move_assignment = typename T::propagate_on_container_move_assignment;
    
    template <typename T> using m_propagate_on_container_swap = typename T::propagate_on_container_swap;

    template <typename T> using m_is_always_equal = typename T::is_always_equal;

public:
    using allocator_type                        = Alloc;
    using value_type                            = Alloc::value_type;
    using pointer                               = detected_or_t<value_type*, m_pointer, Alloc>;
    using const_pointer                         = detected_or_t<typename pointer_traits<pointer>::rebind<const value_type>, m_const_pointer, Alloc>;
    using void_pointer                          = detected_or_t<typename pointer_traits<pointer>::rebind<void>, m_void_pointer, Alloc>;
    using const_void_pointer                    = detected_or_t<typename pointer_traits<pointer>::rebind<const void>, m_const_pointer, Alloc>;
    using difference_type                       = detected_or_t<typename pointer_traits<pointer>::difference_type, m_difference_type, Alloc>;
    using size_type                             = detected_or_t<typename make_unsigned<difference_type>::type, m_size_type, Alloc>;
    using propagate_on_container_copy_assignment= detected_or_t<false_type, m_propagate_on_container_copy_assignment, Alloc>;
    using propagate_on_container_move_assignment= detected_or_t<false_type, m_propagate_on_container_move_assignment, Alloc>;
    using propagate_on_container_swap           = detected_or_t<false_type, m_propagate_on_container_swap, Alloc>;
    using is_always_equal                       = detected_or_t<typename is_empty<Alloc>::type, m_is_always_equal, Alloc>;
    
};