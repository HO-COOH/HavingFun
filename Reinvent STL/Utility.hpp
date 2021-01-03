#pragma once
#include <array>
#include <cstddef>

/**
 * @brief A super enhanced version of the std::integer_sequence.
 * The class template std::integer_sequence represents a compile-time sequence of integers. 
 * When used as an argument to a function template, the parameter pack Ints can be deduced and used in pack expansion.
 * 
 * @tparam T an integer type to use for the elements of the sequence
 * @tparam Ints a non-type parameter pack representing the sequence
 */
template<typename T, T... Ints>
struct integer_sequence
{
private:

    template<unsigned N, T... Values>
    static constexpr auto pop_back_impl() noexcept
    {
        if constexpr (N == size() - 1)
            return integer_sequence<T, Values...>{};
        else
            return pop_back_impl<N + 1, Values..., at(N)>();
    }

    template<T front, T... Values>
    static constexpr auto pop_front_impl() noexcept
    {
        return integer_sequence<T, Values...>{};
    }

    template<T element, typename Predicate>
    static constexpr auto filter_impl(integer_sequence<T, element>, Predicate predicate) noexcept
    {
        if constexpr (predicate(element))
            return integer_sequence<T, element>{};
        else
            return integer_sequence<T>{};
    }

    template<unsigned N, unsigned Start, unsigned End, T... Values>
    static constexpr auto split_impl()
    {
        if constexpr (N < Start)
            return split_impl<N + 1, Start, End>();
        else if constexpr (N >= Start && N <= End)
            return split_impl<N + 1, Start, End, Values..., at(N)>();
        else
            return integer_sequence<T, Values...>{};
    }
public:
    using value_type = T;

    /**
     * @brief Returns the number of elements in the sequence
     */
    static constexpr auto size() noexcept
    {
        return sizeof...(Ints);
    }

    /**
     * @brief Return the last elements in the sequence
     */
    static constexpr auto back() noexcept
    {
        return ((Ints, ...));
    }

    /**
     * @brief Return the i-th element in the sequence
     */
    static constexpr auto at(T i) noexcept
    {
        return to_array()[i];
    }

    /**
     * @brief Return the i-th element in the sequence
     */
    template<unsigned i>
    static constexpr auto at() noexcept
    {
        return to_array()[i];
    }

    /**
     * @brief Return the first element in the sequence, same as at(0)
     */
    static constexpr auto front() noexcept
    {
        return at(0);
    }

    /**
     * @brief Append an element to the end of the sequence
     * 
     * @tparam Int The element to append
     * @return a new sequence with the element appended
     */
    template<T Int>
    static constexpr auto push_back() noexcept
    {
        return integer_sequence<T, Ints..., Int>{};
    }

    /**
     * @brief Prepend an element to the front of the sequence
     * 
     * @tparam Int The element to append
     * @return a new sequence with the element prepended
     */
    template<T Int>
    static constexpr auto push_front() noexcept
    {
        return integer_sequence<T, Int, Ints...>{};
    }

    /**
     * @brief Remove an element at the end of the sequence
     * 
     * @return a new sequence without the last element in the original sequence
     */
    static constexpr auto pop_back() noexcept
    {
        if constexpr(size() > 0)
            return pop_back_impl<0>();
        else
            return integer_sequence<T>{};
    }

    /**
     * @brief Remove an element at the front of the sequence
     * 
     * @return a new sequence without the first element in the original sequence
     */
    static constexpr auto pop_front() noexcept
    {
        if constexpr(size() > 0)
            return pop_front_impl<Ints...>();
        else
            return integer_sequence<T>{};
    }

    /**
     * @brief Returns an integer_sequence that has elements [0, Pos]
     * 
     * @tparam Pos The position to split
     */
    template<unsigned Pos>
    static constexpr auto split_first_part() noexcept
    {
        return split_impl<0, 0, Pos>();
    }

    /**
     * @brief Returns an integer_sequence that has elements [Pos + 1, End]
     * 
     * @tparam Pos The position to split
     */
    template<unsigned Pos>
    static constexpr auto split_second_part() noexcept
    {
        return split_impl<Pos, Pos + 1, size() - 1>();
    }

    /**
     * @brief Return a pair of split result, that is <[0, Pos], [Pos + 1, End]>
     * 
     * @tparam Pos The position to split
     */
    template<unsigned Pos>
    static constexpr auto split_at() noexcept
    {
        return std::pair{ split_first_part<Pos>(), split_second_part<Pos>() };
    }

    /**
     * @brief Insert an element at a position, shift the original elements at and after the position back
     * 
     * @tparam Pos The position to insert
     * @tparam element The element to be inserted
     * @return A new sequence after the insertion
     */
    template<unsigned Pos, T element>
    static constexpr auto insert() noexcept
    {
        return split_first_part<Pos - 1>().append(integer_sequence<T, element>{}, split_second_part<Pos - 1>());
    }

    /**
     * @brief Remove all the elements that is equal to the specified value in the sequence
     * 
     * @tparam element The elements to be removed
     * @return A new sequence after the erase
     */
    template<T element>
    static constexpr auto erase() noexcept
    {
        return filter([](auto const num) { return element != num; })
    }

    /**
     * @brief Remove the element at a position
     * 
     * @tparam Pos The position to be removed
     * @return A new sequence after the erase
     */
    template<unsigned Pos>
    static constexpr auto erase_at() noexcept
    {
        static_assert(Pos < size());
        return split_first_part<Pos - 1>().append(split_second_part<Pos>());
    }

    /**
     * @brief Construct a container from this sequence, using list initialization
     * 
     * @tparam Container The type of the container
     */
    template<typename Container>
    static constexpr auto to_container()
    {
        return Container{ Ints... };
    }

    /**
     * @brief Construct a std::array from this sequence, same as to_container<std::array<T, size()>>
     */
    static constexpr auto to_array() noexcept
    {
        return std::array{ Ints... };
    }

    /**
     * @brief Returns the sum of the sequence
     */
    static constexpr auto sum() noexcept
    {
        return (Ints + ...);
    }

    /**
     * @brief Returns the product of the sequence
     */
    static constexpr auto product() noexcept
    {
        return (T{1} * ... * Ints);
    }

    /**
     * @brief Append *this with another sequence
     */
    template<typename Rhs, Rhs... Ints2>
    static constexpr auto append(integer_sequence<Rhs, Ints2...>) noexcept
    {
        return integer_sequence<std::common_type_t<T, Rhs>, Ints..., Ints2...>{};
    }

    /**
     * @brief Append *this with multiple sequences
     */
    template<typename Rhs, Rhs... Ints2, typename... Sequences>
    static constexpr auto append(integer_sequence<Rhs, Ints2...> first, Sequences...rest) noexcept
    {
        if constexpr (sizeof...(Sequences) == 0)
            return append(first);
        else
            return append(first).append(rest...);
    }

    /**
     * @brief Prevent accidental appending nothing
     */
    static constexpr auto append() noexcept
    {
        return *this;
    }

    /**
     * @brief Remove all the elements that does not satisfy the predicate
     * 
     * @tparam Predicate Type of the predicate, which should be a constexpr function that accepts one element and returns a bool
     * @param predicate The constexpr function that accepts one element and returns a bool
     * @return A new sequence whose elements satisfy the predicate
     */
    template<typename Predicate>
    static constexpr auto filter(Predicate predicate) noexcept
    {
        if constexpr (sizeof...(Ints) > 0)
            return integer_sequence<T>{}.append(filter_impl(integer_sequence<T, Ints>{}, predicate)...);    
        else
            return integer_sequence<T>{};
    }
};

namespace details
{
    template<typename T, T N, T... Ints>
    auto make_integer_sequence_impl()
    {
        if constexpr (N == 0)
            return integer_sequence<T, Ints...>{};
        else
            return make_integer_sequence_impl<T, N - 1, N - 1, Ints...>();
    }
}

/**
 * @brief Generate a sequence from [0,... N-1]
 * 
 * @tparam T Type of the integer in the sequence
 */
template<typename T, T N>
using make_integer_sequence = decltype(details::make_integer_sequence_impl<T, N>());

/**
 * @brief A helper alias template std::index_sequence is defined for the common case where T is std::size_t
 */
template<std::size_t... Ints>
using index_sequence = integer_sequence<std::size_t, Ints...>;

/**
 * @brief Generate a sequence from [0,... N-1] with std::size_t as the integer type
 */
template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;
