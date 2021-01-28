#pragma once
#include "Iterator.hpp"
#include "AllocatorTraits.hpp"
#include "Allocator.hpp"
#include "UninitializedMemory.hpp"
#include "Algorithm.hpp"
#include <initializer_list>

namespace details
{
    template<typename T, typename Allocator>
    struct vector_base
    {
        using pointer = typename  allocator_traits<Allocator>::pointer;

        //<Alloc::rebind<T>::other> if present, otherwise <Alloc<T, Args>> if this Alloc is <Alloc<U, Args>>
        using alloc_type = typename allocator_traits<Allocator>::template rebind_alloc<T>;
        
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

            //Not really copied the data, just copied the pointers
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

        struct vector_impl : alloc_type, vector_impl_data
        {
            vector_impl() noexcept(is_nothrow_default_constructible_v<alloc_type>) : alloc_type{} {}
            vector_impl(alloc_type const& a) : alloc_type{ a } {}
            vector_impl(vector_impl&& x) : alloc_type{ std::move(x) }, vector_impl_data{ std::move(x) } {}
            vector_impl(alloc_type&& a) : alloc_type{ std::move(a) } {}
            vector_impl(alloc_type&& a, vector_impl&& x) : alloc_type{ std::move(a) }, vector_impl{ std::move(x) }{}
        };

        vector_impl m_impl;

        pointer allocate(size_t n)
        {
            return n != 0 ? allocator_traits<alloc_type>::allocate(m_impl, n);
        }

        void deallocate(pointer p, size_t n)
        {
            if (p)
                allocator_traits<alloc_type>::deallocate(m_impl, p, n);
        }

        vector_base() = default;
        vector_base(Allocator const& a) noexcept : m_impl{ a } {}
        vector_base(Allocator const& a, vector_base&& x) : m_impl{ alloc_type(a), std::move(x.m_impl) } {}
        vector_base(size_t n) : m_impl{} { create_storage(n); }
        vector_base(size_t n, Allocator const& a) : m_impl{ a } { create_storage(n); }
        vector_base(vector_base&&) = default;
        vector_base(alloc_type&& a) noexcept : m_impl{ std::move(a) } {}

        ~vector_base() noexcept
        {
            deallocate(m_impl.m_start, m_impl.end_of_stroage - m_impl.start);
        }
            
    protected:
        void create_storage(std::size_t n)
        {
            m_impl.m_start = allocate(n);
            m_impl.m_finish = m_impl.m_start;
            m_impl.m_end_of_storage = m_impl.start + n;
        }
    };
}

/**
 * @brief a sequence container that encapsulates dynamic size arrays
 * 
 * @tparam T type of the elements
 * @tparam Allocator 	An allocator that is used to acquire/release memory and to construct/destroy the elements in that memory
 * 
 * @details Requirement of T:
 *          Erasable
 * 
 */
template<typename T, typename Allocator = allocator<T>>
class vector : protected details::vector_base<T, Allocator>
{
    using base = details::vector_base<T, Allocator>;

    void fill_initialize(size_type n, value_type const& value)
    {
        m_impl.m_finish = unintialized_fill_n(m_impl.m_start, n, value, m_impl);
    }

    void default_initialize(size_type n)
    {
        m_impl.m_finish = details::uninitialized_default_n_a(m_impl.m_start, n, m_impl);
    }

    /**
     * Why do we need 2 overload for input_iterator and forward_iterator here you ask?
     * Because forward_iterator can do multi-passes, which is used in uninitialized_copy, like:
     *      auto current = first;   //We need forward_iterator
     * And also, std::distance requires input_iterator
     *  template<typename InputIt>
     *  constexpr typename std::iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last)
     * While input_iterator can't do that, therefore we don't know the distance between 2 input_iterator
     * If it is input_iterator, we have to use emplace_back()/push_back() every time we advance the iterator
     */
    template<typename ForwardIt>
    void range_initialize(ForwardIt first, ForwardIt last, forward_iterator_tag)
    {
        const auto n = distance(first, last);
        create_storage(n);
        m_impl.m_finish = uninitialized_copy(first, last, m_impl.m_start, m_impl);
    }

    template<typename InputIt>
    void range_initialize(InputIt first, InputIt last, input_iterator_tag)
    {
        try 
        {
            for (; first != last; ++first)
                emplace_back(*first);   //should be the same as push_back(*first), which is used if __cplusplus < 201103L
        }
        catch (...)
        {
            clear();
            throw;
        }
    }


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
    using reverse_iterator          = reverse_iterator<iterator>;
    using const_reverse_iterator    = reverse_iterator<const_iterator>;
    using allocator_type            = Allocator;

    /*Constructors*/

    /**
     * @brief Default constructor, constructs an empty container with a default-constructed allocator
     */
    vector() = default;
    
    /**
     * @brief Construct an empty container with the given allocator alloc
     * @param alloc allocator to use for all memory allocations of this container
     */
    explicit vector(Allocator const& alloc) noexcept : base(alloc) {}

    /**
     * @brief Constructs the container with count copies of elements with value value
     * @param count the size of the container
     * @param value the value to initialize elements of the container with
     * @param alloc allocator to use for all memory allocations of this container
     */
    explicit vector(size_type count, T const& value, Allocator const& alloc = Allocator{}) : base{count, alloc} 
    {
        fill_initialize(count, value);
    }

    /**
     * @brief Construct the container with count default-inserted instances of T. No copies are made
     * @param count the size of the container
     * @param alloc allocator to use for all memory allocations of this container
     */
    explicit vector(size_type count, Allocator const& alloc = Allocator{}) : base{count, alloc}
    {
        default_initialize(count);
    }

    /**
     * @brief Construct the container with the contents of the range [first, last)
     * @param first the iterator to the first element of the range to copy the elements from
     * @param last the iterator to the past-end element of the range to copy
     * @param alloc allocator to use for all memory allocations of this container
     */
    template<typename InputIt>
    vector(InputIt first, InputIt last, Allocator const& alloc = Allocator{})
    {

    }

    /**
     * @brief copy constructor
     * @param other another container to be used as source to initialize the elements of the container with
     */
    vector(vector const& other);

    /**
     * @brief Constructs the container with the copy of the contents of other, using alloc as the allocator.
     * @param other another container to be used as source to initialize the elements of the container with
     * @param alloc allocator to use for all memory allocations of this container
     */
    vector(vector const& other, Allocator const& alloc);

    /**
     * @brief move constructor.
     * @param other another container to be used as source to initialize the elements of the container with
     * @note After the move, other is guaranteed to be empty()
     */
    vector(vector&& other) noexcept;

    /**
     * @brief Allocator-extended move constructor. Using alloc as the allocator for the new container, 
     *  moving the contents from other; if alloc != other.get_allocator(), 
     *  this results in an element-wise move. (in that case, other is not guaranteed to be empty after the move)
     * @param other another container to be used as source to initialize the elements of the container with
     * @param alloc allocator to use for all memory allocations of this container
     */
    vector(vector&& other, Allocator const& alloc);


    /**
     * @brief Construct the container with the contents of the initializer list init
     * @param init initializer list to initialize the elements of the container with
     * @param alloc allocator to use for all memory allocations of this container
     */
    vector(std::initializer_list<T> init, Allocator const& alloc = Allocator{});

    /*Destructor*/
    
    /**
     * @brief Destruct the vector
     * @details The destructors of the elements are called and the used storage is deallocated.
     * @note If the elements are pointers, the pointed_to objects are not destroyed
     */
    ~vector();

    /*Assignment operator*/
    
    /**
     * @brief Copy assignment operator.
     * @param other another container to be used as source to initialize the elements of the container with
     */
    vector& operator=(vector const& other);

    /**
     * @brief Move assignment operator.
     * @param other another container to be used as source to initialize the elements of the container with
     */
    vector& operator=(vector&& other);

    /**
     * @brief Replaces the contents with those identified by initializer_list
     * @param ilist initializer list to use as data source
     */
    vector& operator=(std::initializer_list<T> ilist);

    /*assignment*/
    
    /**
     * @brief Replaces the contents with count copies of value value
     * @param count the new size of the container
     * @param value the value to initialize elements of the container with
     */
    void assign(size_type count, T const& value);

    /**
     * @brief Replaces the contents with copies of those in the rage [first, last).
     * @param first the range to copy the elements from
     * @param last the range to copy the elements from
     * @note The behavior is undefined if either argument is an iterator to *this
     */
    template<typename InputIt>
    void assign(InputIt first, InputIt last);

    /**
     * @brief Replaces the contents with the elements from the initializer list ilist.
     * @param ilist initializer list to copy the values from
     */
    void assign(std::initializer_list<T> ilist);

    /**
     * @brief Returns the allocator associated with the container.
     */
    allocator_type get_allocator() const noexcept
    {
        return allocator_type{ m_impl };
    }

    /*Accessors*/

    /**
     * @brief Returns a reference to the element at specified location pos, with bounds checking
     * @param pos position of the element to return
     * @throw std::out_of_range if pos is not within the range of the container
     */
    reference at(size_type pos);

    /**
     * @brief Returns a reference to the element at specified location pos. No bounds checking is performed.
     * @param pos position of the element to return
     * @return Reference to the requested element
     */
    reference operator[](size_type pos);

    /**
     * @brief Returns a reference to the first element in the container.
     * @note Calling front() on an empty container is undefined.
     */
    reference front();

    /**
     * @brief Returns a reference to the first element in the container.
     * @note Calling front() on an empty container is undefined.
     */
    const_reference front() const;

    /**
     * @brief Returns a reference to the last element in the container.
     * @note Calling back() on an empty container is undefined.
     */
    reference back();

    /**
     * @brief Returns a reference to the last element in the container.
     * @note Calling back() on an empty container is undefined.
     */
    const_reference back() const;

    /**
     * @brief Returns pointer to the underlying array serving as element storage.
     * @note If size() is 0, data() may or may not return a null pointer
     */
    T* data() noexcept;

    /**
     * @brief Returns pointer to the underlying array serving as element storage.
     * @note If size() is 0, data() may or may not return a null pointer
     */
    T const* data() const noexcept;

    /*Iterators*/
    
    /**
     * @brief Returns an iterator to the first element of the array
     *
     * @return Iterator to the first element.
     */
    const_iterator begin() const noexcept
    {
        return data();
    }

    /**
     * @brief Returns an iterator to the first element of the array
     *
     * @return Iterator to the first element.
     */
    iterator begin() noexcept
    {
        return data();
    }

    /**
     * @brief Returns an iterator to the first element of the array
     *
     * @return Iterator to the first element.
     */
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /**
     * @brief Returns an iterator to the element following the last element of the array.
     *
     * @return Iterator to the element following the last element.
     * @note This element acts as a placeholder; attempting to access it results in undefined behavior.
     */
    const_iterator end() const noexcept
    {
        return data() + N;
    }

    /**
     * @brief Returns an iterator to the element following the last element of the array.
     *
     * @return Iterator to the element following the last element.
     * @note This element acts as a placeholder; attempting to access it results in undefined behavior.
     */
    iterator end() noexcept
    {
        return data() + N;
    }

    /**
     * @brief Returns an iterator to the element following the last element of the array.
     *
     * @return Iterator to the element following the last element.
     * @note This element acts as a placeholder; attempting to access it results in undefined behavior.
     */
    const_iterator cend() const noexcept
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
    const_reverse_iterator rbegin() const noexcept
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
    reverse_iterator rbegin() noexcept
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
    const_reverse_iterator crbegin() const noexcept
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
    const_reverse_iterator rend() const noexcept
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
    reverse_iterator rend() noexcept
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
    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    /*Capacity*/

    /**
     * @brief Check if the container has no elements.
     */
    bool empty() const noexcept;

    /**
     * @brief Returns the number of elements in the container.
     */
    size_type size() const noexcept;

    /**
     * @brief Reurns the maximum number of elements the container is able to hold due to system or library implementation limimtations.
     * @note This value typically reflects the theoretical limit on the size of the container, at most std::numeric_limits<difference_type>::max(). 
     * At runtime, the size of the container may be limited to a value smaller than max_size() by the amount of RAM available.
     */
    size_type max_size() const noexcept;

    /**
     * @brief Increase the capacity of the vector to a value that's >= new_cap. If new_cap is >= the current capacity(), new storage is allocated, 
     * otherwise the method does nothing.
     * @param new_cap new capacity of the vector
     * @note T must meet the requirements of MoveInsertable
     * @throw std::length_error if new_cap > max_size(), and any exception thrown by Allocator::allocate(), typically std::bad_alloc
     */
    void reserve(size_type new_cap);

    /**
     * @brief Returns the number of elements that the container has currently allocated space for.
     */
    size_type capacity() const noexcept;

    /**
     * @brief Requests the removal of unsed capacity.
     * @note It depends on the implementation whether the request is fulfilled
     */
    void shrink_to_fit();

    /*Modifiers*/

    /**
     * @brief Erase all elements from the container. After this call, size() returns 0
     * @note capacity() is unchanged.
     */
    void clear() noexcept;

    /**
     * @brief Inserts value before pos.
     * @param pos iterator before which the content will be inserted. pos may be the end() iterator
     * param value element value to insert
     */
    iterator insert(const_iterator pos, T const& value);

    /**
     * @brief Inserts value before pos.
     * @param pos iterator before which the content will be inserted. pos may be the end() iterator
     * param value element value to insert
     */
    iterator insert(const_iterator pos, T&& value);

    /**
     * @brief insert count copies of the value before pos.
     * @param pos iterator before which the content will be inserted. pos may be the end() iterator
     * @param count number of copies of value to insert
     * @param value element value to insert
     */
    iterator insert(const_iterator pos, size_type count, T const& value);

    /**
     * @brief insert elements from range [first, last) before pos.
     * @param first	the range of elements to insert, can't be iterators into container for which insert is called
     * @param last the range of elements to insert, can't be iterators into container for which insert is called
     * @note This overload only participates in overload resolution if InputIt satisfies LegacyInputIterator to avoid ambiguity
     * with insert(pos, count, value)
     */
    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);

    /**
     * @brief Inserts elements from initializer list ilist before pos.
     * @param ilist initializer list to insert the values from
     */
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    /**
     * @brief Inserts a new element into the container before pos.
     * @param pos iterator before which the new element will be constructed
     * @param args arguments to forward to the constructor of the element
     * @details The element is constructed through std::allocator_traits::construct, 
     * which typically uses placement-new to construct the element in-place at a location provided by the container. 
     * However, if the required location has been occupied by an existing element, 
     * the inserted element is constructed at another location at first, and then move assigned into the required location.
     */
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    /**
     * @brief Removes the element at pos.
     * @param pos iterator to the element to remove
     */
    iterator erase(const_iterator pos);

    /**
     * @brief Removes the elements in the range [first, last).
     * @param first range of elements to remove
     * @param last range of elements to remove
     */
    iterator erase(const_iterator first, const_iterator last);

    /**
     * @brief Appends the given element value to the end of the container, the new element is initialized as a copy of value
     * @param value the value of the element to append
     */
    void push_back(T const& value);

    /**
     * @brief Appends the given element value to the end of the container, value is momved into the new element.
     * @param value the value of the element to append
     */
    void push_back(T&& value);

    /**
     * @breif Append a new element to the end of the container, the element is constructed through allocator_traits::construct.
     * @param args arguments to forward to the constructor of the element
     */
    template<typename... Args>
    reference emplace_back(Args&&... args);

    /**
     * @brief Remomves the last element of the container.
     * @note Calling pop_back() on an empty container results in undefined behavior.
     */
    void pop_back();

    /**
     * @brief Resizes the container. If size() <= count, additional default-inserted elements are appended
     * @param count new size of the container
     */
    void resize(size_type count);

    /**
     * @brief Resizes the container. If size() <= count, additional copies of value are appended
     * @param count new size of the container
     * @param value the value to initialize the new elements with
     */
    void resize(size_type count, T const& value);

    /**
     * @brief Exchanges the contents of the container with those of other. Doesn't invoke any move/copy/swap operations on individual elements
     * @throw nothrow if (std::allocator_traits<Allocator>::propagate_on_container_swap::value || std::allocator_traits<Allocator>::is_always_equal::value)
     */
    void swap(vector& other) noexcept();
};

template<typename T, typename Alloc>
bool operator==(vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs) { return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()); }
template<typename T, typename Alloc>
bool operator!=(vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs) { return !(lhs == rhs); }
template<typename T, typename Alloc>
bool operator<(vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs)
{ return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
template<typename T, typename Alloc>
bool operator<=(vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs) { }
template<typename T, typename Alloc>
bool operator>(vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs);
template<typename T, typename Alloc>
bool operator>=(vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs);