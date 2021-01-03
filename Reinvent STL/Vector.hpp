#pragma once
#include "Iterator.hpp"
#include "Allocator.hpp"

namespace details
{
    template<typename T, typename Allocator>
    struct vector_base
    {
        using pointer = allocator_traits<Allocator>::pointer;
        
        struct vector_impl_data
        {
            pointer m_start{};
            pointer m_finish{};
            pointer m_end_of_storage{};

            vector_impl_data() noexcept = default;
            vector_impl_data(vector_impl_data&& x) noexcept 
                : m_start{x.m_start}, m_finish{x.m_finish}, m_end_of_storage{x.m_end_of_storage}
            {
                x.m_start = x.m_finish = x.m_end_of_storage = nullptr;
            }

            void copy_data(vector_impl_data const& rhs) noexcept
            {
                m_start = rhs.m_start;
                m_finish = rhs.m_finish;
                m_end_of_storage = rhs.m_end_of_storage;
            }

            void swap_data(vector_impl_data& rhs) noexcept
            {
                vector_impl_data temp;
                temp.copy_data(*this);  //this->temp
                copy_data(rhs);         //rhs->this
                rhs.copy_data(temp);    //temp(old this)->rhs
            }
        };

    protected:
        void create_storage(std::size_t n)
        {
            
        }
    };
}

/**
 * @brief a sequence container that encapsulates dynamic size arrays
 * 
 * @tparam T type of the elements
 * @tparam Allocator 	An allocator that is used to acquire/release memory and to construct/destroy the elements in that memory
 * 
 * @details Requirment of T:
 *          Erasable
 * 
 */
template<typename T, typename Allocator>
class vector
{
public:
    /*Member types*/
    using value_type                = T;
    using size_type                 = std::size_t;
    using difference_type           = std::ptrdiff_t;
    using reference                 = value_type&;
    using const_reference           = value_type const&;
    using pointer                   = value_type *;
    using const_pointer             = value_type const *;
    using iterator                  = normal_iterator<pointer, vector>;
    using const_iterator            = normal_iterator<pointer const, vector>;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
    using allocator_type = Allocator;

};