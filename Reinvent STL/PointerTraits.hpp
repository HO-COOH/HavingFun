#pragma once
#include "TypeTraits.hpp"
#include <cstddef>

/**
 * @brief Obtains the actual address of the object or function arg, even in presence of overloaded operator&

 * @param arg lvalue object or function
 * @return Pointer to arg
 * @note In C++20 this is no longer implementable due to constexpr requirements
 */
template<typename T>
T* addressof(T& arg) noexcept
{
    if constexpr (is_object_v<T>)
        return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
    /* Why does it cast like this you ask?
     * Because reinterpret_cast can add const volatile qualifiers, but cannot remove it.
     * So reinterpret_cast it first to const volatile char& to be compatible if T is const volatile qualified.
     * Then const_cast removes the const, so that it can be reinterpret_cast next(because reinterpret_cast cannot remove const I just mentioned :)
     * Finally it is reinterpret_cast to T*, with the original const volatile qualifiers, if any
     */
    else //for reference, function, or void
        return &arg;
}
/* Rvalue overload is deleted to prevent taking the address of const rvalues.
 * This is important because
 * int const&& get() {return 1;}
 * addressof(get()); will deduce T->const int, which shouldn't be taken the address from and should be avoided
 */
template <typename T>
const T *addressof(const T &&) = delete;

/**
 * @brief The pointer_traits class template provides the standardized way to access certain properties of pointer-like types
 * 
 * @details The standard template std::allocator_traits relies on pointer_traits 
 * to determine the defaults for various typedefs required by Allocator.
 */
template<typename Ptr>
struct pointer_traits
{
private:
    template<typename T> using m_element_type = typename T::element_type;
    
    template<typename T> using m_difference_type = typename T::difference_type;

    template<typename T, typename U> using m_rebind = typename T:: template rebind<U>;
public:
    using pointer           = Ptr;
    using element_type      = detected_or_t<i_th_template_type_t<0, Ptr>, m_element_type, Ptr>;
    using difference_type   = detected_or_t<std::ptrdiff_t, m_difference_type, Ptr>;
    template<typename U> using rebind = detected_or_t<replace_first_template_type_t<Ptr, U>, m_rebind, Ptr, U>;


    /**
     * @brief non-specialized std::pointer_traits template simply calls Ptr::pointer_to(r), 
     * and if Ptr does not provide a static member function pointer_to, 
     * instantiation of this function is a compile-time error
     * 
     * @param r reference to an object of type element_type&, except if element_type is void, in which case the type of r is unspecified
     * @return A dereferenceable pointer to r
     */
    static pointer pointer_to(element_type& r)
    {
        return Ptr::pointer_to(r);
    }
};

template<typename T>
struct pointer_traits<T*>
{
    using pointer                       = T *;
    using element_type                  = T;
    using difference_type               = std::ptrdiff_t;
    template<typename U> using rebind   = U*;

    /**
     * @brief returns std::addressof(r)
     * 
     * @param r reference to an object of type element_type&, except if element_type is void, in which case the type of r is unspecified
     * @return A dereferenceable pointer to r
     */
    static pointer pointer_to(element_type& r)
    {
        return Ptr::pointer_to(r);
    }
};