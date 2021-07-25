#pragma once
#include "TypeTraits.hpp"
#include "TMP.hpp"
#include <array>

template<typename T, T... Ints>
struct integer_sequence;
namespace details
{
    template<typename Seq1, typename Seq2>
    struct reverse_impl {};

    template<typename T, T...Ints2>
    struct reverse_impl<integer_sequence<T>, integer_sequence<T, Ints2...>>
    {
        using type = integer_sequence<T, Ints2...>; //end condition
    };

    template<typename T, T front, T...Ints1, T...Ints2>
    struct reverse_impl<integer_sequence<T, front, Ints1...>, integer_sequence<T, Ints2...>>
    {
        //push front of seq1 -> front of seq2
        using type = typename reverse_impl<integer_sequence<T, Ints1...>, integer_sequence<T, front, Ints2...>>::type;

        /* Using the back of seq1 won't work because non-terminal pack is not deduced
        That is: integer_sequence<T, Ints..., back>, where Ints... is not deducible
        */
    };
}

/**
 * @brief A super enhanced version of the `std::integer_sequence`.
 * @details The class template `std::integer_sequence` represents a compile-time sequence of integers.
 * When used as an argument to a function template, the parameter pack Ints can be deduced and used in pack expansion.
 *
 * @tparam T an integer type to use for the elements of the sequence
 * @tparam Ints a non-type parameter pack representing the sequence
 */
template<typename T, T... Ints>
struct integer_sequence
{
private:

    template<int N, T... Values>
    static constexpr auto pop_back_impl() noexcept
    {
        if constexpr (N == size() - 1)
            return type_identity<integer_sequence<T, Values...>>{};
        else
            return pop_back_impl<N + 1, Values..., at(N)>();
    }

    template<T front, T... Values>
    static constexpr auto pop_front_impl() noexcept
    {
        return type_identity<integer_sequence<T, Values...>>{};
    }

    template<T element, typename Predicate>
    static constexpr auto filter_impl(integer_sequence<T, element>, Predicate predicate) noexcept
    {
        if constexpr (predicate(element))
            return integer_sequence<T, element>{};
        else
            return integer_sequence<T>{};
    }

    template<int N, int Start, int End, T... Values>
    static constexpr auto split_impl()
    {
        if constexpr (N < Start)
            return split_impl<N + 1, Start, End>();
        else if constexpr (N >= Start && N <= End)
            return split_impl<N + 1, Start, End, Values..., at(N)>();
        else
            return integer_sequence<T, Values...>{};
    }

    static constexpr auto GetThis() noexcept
    {
        return integer_sequence<T, Ints...>{};
    }
public:
    /**
     * @brief the value type of the integer sequence 
     */
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
    * @brief Construct a std::array from this sequence, same as to_container<std::array<T, size()>>
    */
    static constexpr auto to_array() noexcept
    {
        return std::array<T, size()>{ Ints... };
    }

    /**
     * @brief Return the i-th element in the sequence
     */
    static constexpr auto at(int i) noexcept
    {
        return to_array()[i];
    }

    /**
     * @brief Return the i-th element in the sequence
     */
    template<int i>
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
     * @brief New type of the sequence after appending an element to the end
     * @tparam Int the integer to append 
     */
    template<T Int>
    using push_back_t = decltype(push_back<Int>());

    /**
     * @brief Prepend an element to the front of the sequence
     *
     * @tparam Int The element to prepend
     * @return a new sequence with the element prepended
     */
    template<T Int>
    static constexpr auto push_front() noexcept
    {
        return integer_sequence<T, Int, Ints...>{};
    }

    /**
     * @brief New type of the sequence after prepending an element to the front of the sequence
     * @tparam Int The element to prepend
     */
    template<T Int>
    using push_front_t = decltype(push_front<Int>());

    /**
     * @brief Remove an element at the end of the sequence
     *
     * @return a new sequence without the last element in the original sequence
     */
    static constexpr auto pop_back() noexcept
    {
        if constexpr (size() > 0)
            return pop_back_impl<0>();
        else
            return type_identity<integer_sequence<T>>{};
    }

    /**
     * @brief New type of the sequence after removing an element at the end of the sequence
     */
    using pop_back_t = typename decltype(pop_back())::type;

    /**
     * @brief Remove an element at the front of the sequence
     *
     * @return a new sequence without the first element in the original sequence
     */
    static constexpr auto pop_front() noexcept
    {
        if constexpr (size() > 0)
            return pop_front_impl<Ints...>();
        else
            return type_identity<integer_sequence<T>>{};
    }

    /**
     * @brief New type of the sequence after removing an element at the front of the sequence
     */
    using pop_front_t = typename decltype(pop_front())::type;

    /**
     * @brief Returns an integer_sequence that has elements [0, Pos]
     * @tparam Pos The position to split
     * @note `Pos` starts from 0
     */
    template<int Pos>
    static constexpr auto split_first_part() noexcept
    {
        return split_impl<0, 0, Pos>();
    }

    /**
     * @brief New type of the sequence that has elements [0, Pos]
     * @tparam Pos The position to split
     * @note `Pos` starts from 0 
     */
    template<int Pos>
    using split_first_part_t = decltype(split_first_part<Pos>());

    /**
     * @brief Returns an integer_sequence that has elements [Pos + 1, End]
     * @tparam Pos The position to split
     * @note `Pos` starts from 0
     */
    template<int Pos>
    static constexpr auto split_second_part() noexcept
    {
        if constexpr (Pos + 1 > size() - 1)
            return type_identity<integer_sequence<T>>{};
        else
            return type_identity<decltype(split_impl<Pos, Pos + 1, size() - 1>())>{};
    }

    /**
     * @brief New type of the sequence that has elements [Pos + 1, End]
     * @tparam Pos The position to split
     * @note `Pos` starts from 0
     */
    template<int Pos>
    using split_second_part_t = typename decltype(split_second_part<Pos>())::type;

    /**
     * @brief Return a type_list of split result, that is <[0, Pos], [Pos + 1, End]>
     * @tparam Pos The position to split
     * @note `Pos` starts from 0
     */
    template<int Pos>
    static constexpr auto split_at() noexcept
    {
        return type_list<decltype(split_first_part<Pos>()), decltype(split_second_part<Pos>())> {};
    }

    /**
     * @brief A type list holding the split resulting type, that is <[0, Pos], [Pos + 1, End]>
     * @tparam Pos The position to split
     * @note `Pos` starts from 0
     */
    template<int Pos>
    using split_at_t = type_list<split_first_part_t<Pos>, split_second_part_t<Pos>>;

    /**
     * @brief Insert an element at a position, shift the original elements at and after the position back
     *
     * @tparam Pos The position to insert
     * @note `Pos` starts from 0
     * @tparam element The element to be inserted
     * @return A new sequence after the insertion
     */
    template<int Pos, T element>
    static constexpr auto insert() noexcept
    {
        return split_first_part<Pos - 1>().append(integer_sequence<T, element>{}, split_second_part_t<Pos - 1>{});
    }

    /**
     * @brief New type of sequence after inserting an element at a specified position and shifting the original elements at and after the position back
     * @tparam Pos The position to insert
     * @tparam element The element to be inserted
     * @note `Pos` starts from 0
     */
    template<int Pos, T element>
    using insert_t = decltype(insert<Pos, element>());

    /**
     * @brief Remove all the elements that is equal to the specified value in the sequence
     * @tparam element The elements to be removed
     * @return A new sequence after the erase
     */
    template<T element>
    static constexpr auto erase() noexcept
    {
        return filter([](auto const num) { return element != num; });
    }

    /**
     * @brief New type of the sequnece after removing all elements that is equal to the specified value
     * @tparam element The elements to be removed
     */
    template<T element>
    using erase_t = decltype(erase<element>());

    /**
     * @brief Remove the element at a position
     * @tparam Pos The position to be removed
     * @note `Pos` starts from 0
     * @return A new sequence after the erase
     */
    template<int Pos>
    static constexpr auto erase_at() noexcept
    {
        static_assert(Pos < size());
        if constexpr (Pos < 1)
            return split_second_part_t<Pos>{};
        else
            return split_first_part<Pos - 1>().append(split_second_part_t<Pos>{});
    }

    /**
     * @brief New type of the sequence after removing an element at a specified position
     * @tparam Pos The position to be removed
     * @note `Pos` starts from 0
     */
    template<int Pos>
    using erase_at_t = decltype(erase_at<Pos>());
    
    /**
     * @brief Construct a container from this sequence, using list initialization
     * @tparam Container The type of the container
     */
    template<typename Container>
    static constexpr auto to_container()
    {
        return Container{ Ints... };
    }

    /**
     * @brief Returns the sum of the sequence
     */
    static constexpr auto sum() noexcept
    {
        if constexpr (size() == 0)
            return T{ 0 };
        else
            return (Ints + ...);
    }

    /**
     * @brief Returns the product of the sequence
     */
    static constexpr auto product() noexcept
    {
        if constexpr (size() == 0)
            return T{ 0 };
        else
            return (T{ 1 } * ... * Ints);
    }

    /**
     * @brief Append *this with another sequence in parameter
     */
    template<typename Rhs, Rhs... Ints2>
    static constexpr auto append(integer_sequence<Rhs, Ints2...>) noexcept
    {
        return integer_sequence<std::common_type_t<T, Rhs>, Ints..., Ints2...>{};
    }

    /**
     * @brief Append *this with multiple sequences in template parameters
     */
    template<typename...IntegerSequences>
    static constexpr auto append() noexcept
    {
        return append(IntegerSequences{}...);
    }

    /**
     * @brief Append *this with multiple sequences in parameters
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
     * @brief New type of sequence after appending some other integer_sequences or integers
     * @tparam IntegerSequence Integer sequences or integers to append
     */
    template<typename... IntegerSequence>
    using append_t = decltype(append(IntegerSequence{}...));

    /**
     * @brief Prevent accidental appending nothing
     */
    static constexpr auto append() noexcept
    {
        return GetThis();
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
        //TODO: assert the predicate return type
        if constexpr (sizeof...(Ints) > 0)
            return integer_sequence<T>{}.append(filter_impl(integer_sequence<T, Ints>{}, predicate)...);
        else
            return integer_sequence<T>{};
    }

    /**
     * @brief New type of sequence after removing all the elements that does not satisfy the predicate
     * @tparam Predicate Type of the predicate, which should be a constexpr function that accepts one element and returns a bool
     */
    template<typename Predicate>
    using filter_t = decltype(filter(Predicate{}));

    /**
     * @brief Transform this integer_sequence to a new integer_sequence
     * @tparam UnaryFunction The type of the transform function
     * @param func The transform function
     * @return A new sequence after the transform
     */
    template<typename UnaryFunction>
    static constexpr auto transform(UnaryFunction func) noexcept
    {
        //TODO: assert the unaryFunction type
        return integer_sequence<T, func(Ints)...>{};
    }

    /**
     * @brief New type of the sequence after transforming
     * @tparam UnaryFunction The type of the transform function
     */
    template<typename UnaryFunction>
    using transform_t = decltype(transform(UnaryFunction{}));

    /**
     * @brief Repeat all the elements in the sequence for a specified times
     * @tparam times Repeating times, must >= 0 
     * @return A new sequence after repeat
     */
    template<int times>
    static constexpr auto repeat() noexcept
    {
        static_assert(times >= 0, "Cannot repeat negative times!");
        if constexpr (times == 0)
            return integer_sequence<T>{};
        else
            return append(repeat<times - 1>());
    }

    /**
     * @brief New type of the sequence containing repeated elements
     * @tparam times Repeating times, must >= 0
     */
    template<int times>
    using repeat_t = decltype(repeat<times>());

    //using reverse = typename decltype(details::reverse_impl<integer_sequence<T, Ints...>, integer_sequence<T>>())::type;
    using reverse = typename details::reverse_impl<integer_sequence<T, Ints...>, integer_sequence<T>>::type;

    /**
     * @brief Get the type of type_list<integral_constant<Int1>, integral_constant<Int2>...>
     */
    using to_type_list = type_list<integral_constant<Ints>...>;
};

namespace details
{
    template<typename T, T Start, T N, T... Ints>
    constexpr auto make_integer_sequence_impl()
    {
        if constexpr (N == Start)
            return integer_sequence<T, Ints...>{};
        else
            return make_integer_sequence_impl<T, Start, N - 1, N - 1, Ints...>();
    }

}

/**
 * @brief Generate a sequence from [Start,... End - 1]
 * @tparam T Type of the integer in the sequence
 * @tparam End One over the ending value (for compatibility of std::make_integer_sequence, where the second tparam is for number of elements)
 * @tparam Start The starting value
 */
template<typename T, T End, T Start = T{0}>
using make_integer_sequence = decltype(details::make_integer_sequence_impl<T, Start, End>());

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