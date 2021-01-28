#pragma once
#include "TypeTraits.hpp"
#include "IteratorTraits.hpp"
#include "Utility.hpp"
#include "PointerTraits.hpp" //for addressof
#include "InitializedMemory.hpp"
#include "AllocatorTraits.hpp"

template<typename T, typename... Args>
inline void construct(T* p, Args&&... args)
{
    ::new(static_cast<void*>(p)) T(forward<Args>(args)...);
}

template<typename T>
inline void destroy(T* p)
{
    p->~T();
}

template<typename Iter>
inline void destroy(Iter first, Iter last)
{
    for (; first != last; ++first)
        destroy(addressof(*first));
}




template<typename ForwardIt, typename T>
void uninitialized_fill(ForwardIt first, ForwardIt last, T const& value)
{

}

template<typename ForwardIt, typename SizeType, typename T>
ForwardIt uninitialized_fill_n(ForwardIt first, SizeType count, T const& value)
{
    using value_type = typename iterator_traits<ForwardIt>::value_type;
    static_assert(is_constructible_v<value_type, T const&>, "result type must be constructible from input type");
    if constexpr (is_trivial_v<T> && is_copy_assignable_v<T>) //check if we can use std::fill
    {
        return fill_n(first, count, value);
    }
    else
    {
        auto current = first;
        try
        {
            for (; n > 0; --n, (void)++current)
                construct(addressof(*current), value);
            return current;
        }
        catch (...)
        {
            destroy(first, current);
            throw;
        }
    }
}

/**
 * @brief Construct objects of type typename iterator_traits<ForwardIt>::value_type in the uninitialized storage designated 
 * by the range [first, last) by default-initialization.
 * @param first the range of the elements to initialize
 * @param last the range of the elements to initialize
 */
template<typename ForwardIt, typename SizeType>
ForwardIt uninitialized_default_construct(ForwardIt first, ForwardIt last)
{
    using value_type = typename iterator_traits<ForwardIt>::value_type;
    auto current = first;
    try
    {
        for (; current != last; ++current)
            ::new (static_cast<void*>(addressof(*current))) value;
    }
    catch (...)
    {
        destroy(first, current);
        throw;
    }
    return current;
}

/**
 * @brief Copies elements from the range [first, last) to an uninitialized memory area beginning at d_first.
 * @param first the range of elements to copy
 * @param last the range of elements to copy
 * @param d_first the beginning of the destination range
 */
template<typename InputIt, typename ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
{
    using value_type = typename iterator_traits<ForwardIt>::value_type;
    auto current = d_first;
    try
    {
        for (; first != last; ++first, (void)++current)
            ::new (static_cast<void*>(addressof(*current))) value_type{ *first };
        return current;
    }
    catch (...)
    {
        destroy(first, current);
        throw;
    }
}


namespace details
{
    template<typename Iter, typename Allocator>
    inline void destroy(Iter first, Iter last, Allocator& a)
    {
        for (; first != last; ++first)
            allocator_traits<Allocator>::destory(a, addressof(*first));
    }

    template<typename ForwardIt, typename SizeType, typename T, typename Allocator>
    ForwardIt uninitialized_fill_n_a(ForwardIt first, SizeType count, T const& value, Allocator& a)
    {   
        auto current = first;
        static_assert(is_constructible_v<value_type, T const&>, "result type must be constructible from input type");
        try
        {
            for (; n > 0; --n, (void)++current)
                allocator_traits<Allocator>::construct(a, addressof(*current), value);
            return current;
        }
        catch (...)
        {
            destroy(first, current, a);
            throw;
        }
    }

    template<typename ForwardIt, typename Allocator>
    void uninitialized_default_a(ForwardIt first, ForwardIt last, Allocator& a)
    {
        auto current = first;
        try
        {
            for (; current != last; ++current)
                allocator_traits<Allocator>::construct(a, addressof(*current));
        }
        catch (...)
        {
            destroy(first, current, a);
            throw;
        }
    }

    template<typename ForwardIt, typename SizeType, typename Allocator>
    ForwardIt uninitialized_default_n_a(ForwardIt first, SizeType n, Allocator& a)
    {
        using value_type = typename iterator_traits<ForwardIt>::value_type;
        if constexpr (is_trivial_v<value_type> && is_copy_assignable_v<value_type>)
        {
            return uninitialized_fill_n(first, n, value_type{});
        }
        else
        {
            return details::uninitialized_fill_n_a(first, n, value_type{}, a);
        }
    }
}