#include <iostream>
#include "TypeMap.hpp"
#include "IntegerSequence.hpp"
#include <type_traits>
#include "TMP/Arithmetic.hpp"
#include "TMP/Arithmetic.hpp"
#include "TMP/Ratio.hpp"

namespace details
{
    template <typename T>
    struct is_instance_of_impl
    {
    };
} // namespace details

template <typename T, typename U>
constexpr auto operator*(T&& object, details::is_instance_of_impl<U> query)
{
    return std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(object)>>, U>;
}

#define is_instance_of(Type) \
    *details::is_instance_of_impl<Type> {}




template<auto V, typename T = decltype(V)>
struct A
{
    static constexpr auto value = V;
};

template<typename T>
struct is_integral_constant : false_type {};

template<auto v>
struct is_integral_constant<integral_constant<v>> : true_type
{};

#include "IteratorTraits.hpp"
namespace Test
{
    namespace TMP
    {
        namespace Arithmetic
        {
            using A = integral_constant<1>;
            using B = integral_constant<2>;

            using RatioA = ratio<1, 2>;
            using RatioB = ratio<1, 3>;

            constexpr void MemberTypedef()
            {
                static_assert(std::is_same_v<A::add<B>, integral_constant<3>>);
            }

            constexpr void ExternalTypedef()
            {
                static_assert(details::HasAdd<A>::value);
                static_assert(std::is_same_v<add<A, B>, integral_constant<3>>);
                static_assert(add<RatioA, RatioB>{} == ratio<5, 6>{});
            }

            constexpr void RatioSimplify()
            {
                static_assert(RatioA{} == ratio<2, 4>{});
            }
        }
        
        namespace IntegerSequence
        {
            
            using empty = integer_sequence<int>;
            using seq1 = integer_sequence<int, 1, 2, 3>;
            using seq2 = integer_sequence<char, 'a', 'b', 'c', 'd'>;

            static constexpr void EmptySequenceProperty()
            {
                static_assert(empty::size() == 0, "Empty sequence size = 0");
                static_assert(empty::sum() == 0, "Empty sequence sum = 0");
                static_assert(empty::product() == 0, "Empty sequence product = 0");

                constexpr auto array = empty::to_array();
                static_assert(array.empty(), "Empty sequence -> array empty");
                static_assert(std::is_same_v<empty::to_type_list, type_list<>>, "Empty sequence -> type_list<>");
            }

            static constexpr void ValueType()
            {
                static_assert(std::is_same_v<empty::value_type, int>, "Empty int sequence value_type: int");
                static_assert(std::is_same_v<decltype(empty::to_array())::value_type, int>, "Empty int sequence -> array value_type: int");
                static_assert(std::is_same_v<seq1::value_type, int>, "Int sequence value_type: int");
                static_assert(std::is_same_v<seq2::value_type, char>, "Char sequence value_type: char");
            }

            static constexpr void Size()
            {
                static_assert(seq1::size() == 3);
                static_assert(seq2::size() == 4);
            }

            static constexpr void Back()
            {
                static_assert(seq1::back() == 3);
                static_assert(seq2::back() == 'd');
            }

            static constexpr void Front()
            {
                static_assert(seq1::front() == 1);
                static_assert(seq2::front() == 'a');
            }

            static constexpr void ToArray()
            {
                constexpr auto arr1 = seq1::to_array();
                constexpr auto arr2 = seq2::to_array();

                static_assert(arr1[0] == 1);
                static_assert(arr1[1] == 2);
                static_assert(arr1[2] == 3);

                static_assert(arr2[0] == 'a');
                static_assert(arr2[1] == 'b');
                static_assert(arr2[2] == 'c');
                static_assert(arr2[3] == 'd');
            }

            static constexpr void PushBack()
            {
                static_assert(std::is_same_v<seq1::push_back_t<4>, integer_sequence<int, 1, 2, 3, 4>>);
                static_assert(std::is_same_v<seq2::push_back_t<'e'>, integer_sequence<char, 'a', 'b', 'c', 'd', 'e'>>);
            }

            static constexpr void PushFront()
            {
                static_assert(std::is_same_v<seq1::push_front_t<0>, integer_sequence<int, 0, 1, 2, 3>>);
                static_assert(std::is_same_v<seq2::push_front_t<'z'>, integer_sequence<char, 'z', 'a', 'b', 'c', 'd'>>);
            }

            static constexpr void PopBack()
            {
                static_assert(std::is_same_v<seq1::pop_back_t, integer_sequence<int, 1, 2>>);
                static_assert(std::is_same_v<seq2::pop_back_t, integer_sequence<char, 'a', 'b', 'c'>>);
            }

            static constexpr void PopFront()
            {
                static_assert(std::is_same_v<seq1::pop_front_t, integer_sequence<int, 2, 3>>);
                static_assert(std::is_same_v<seq2::pop_front_t, integer_sequence<char, 'b', 'c', 'd'>>);
            }

            static constexpr void SplitFirstPart()
            {
                static_assert(std::is_same_v<seq1::split_first_part_t<0>, integer_sequence<int, 1>>);
                static_assert(std::is_same_v<seq1::split_first_part_t<1>, integer_sequence<int, 1, 2>>);
                static_assert(std::is_same_v<seq1::split_first_part_t<2>, integer_sequence<int, 1, 2, 3>>);
                
                static_assert(std::is_same_v<seq2::split_first_part_t<0>, integer_sequence<char, 'a'>>);
                static_assert(std::is_same_v<seq2::split_first_part_t<1>, integer_sequence<char, 'a', 'b'>>);
                static_assert(std::is_same_v<seq2::split_first_part_t<2>, integer_sequence<char, 'a', 'b', 'c'>>);
                static_assert(std::is_same_v<seq2::split_first_part_t<3>, integer_sequence<char, 'a', 'b', 'c', 'd'>>);
            }

            static constexpr void SplitSecondPart()
            {
                static_assert(std::is_same_v<seq1::split_second_part_t<0>, integer_sequence<int, 2, 3>>);
                static_assert(std::is_same_v<seq1::split_second_part_t<1>, integer_sequence<int, 3>>);
                static_assert(std::is_same_v<seq1::split_second_part_t<2>, integer_sequence<int>>);

                static_assert(std::is_same_v<seq2::split_second_part_t<0>, integer_sequence<char, 'b', 'c', 'd'>>);
                static_assert(std::is_same_v<seq2::split_second_part_t<1>, integer_sequence<char, 'c', 'd'>>);
                static_assert(std::is_same_v<seq2::split_second_part_t<2>, integer_sequence<char, 'd'>>);
                static_assert(std::is_same_v<seq2::split_second_part_t<3>, integer_sequence<char>>);
            }

            static constexpr void Insert()
            {
                static_assert(std::is_same_v<seq1::insert_t<0, 0>, integer_sequence<int, 0, 1, 2, 3>>, "Insert front");
                static_assert(std::is_same_v<seq1::insert_t<seq1::size(), 10>, integer_sequence<int, 1, 2, 3, 10>>, "Insert back");
                static_assert(std::is_same_v<seq1::insert_t<1, 10>, integer_sequence<int, 1, 10, 2, 3>>, "Insert middle");
                static_assert(std::is_same_v<seq1::insert_t<2, 10>, integer_sequence<int, 1, 2, 10, 3>>, "Insert middle");
            }

            static constexpr void EraseSingle()
            {
                static_assert(std::is_same_v<seq1::erase_t<3>, integer_sequence<int, 1, 2>>);
                static_assert(std::is_same_v<seq2::erase_t<'b'>, integer_sequence<char, 'a', 'c', 'd'>>);
            }
            
            static constexpr void EraseDuplicate()
            {
                using hasDuplicate = seq1::push_back_t<3>::push_back_t<1>;
                static_assert(std::is_same_v<hasDuplicate::erase_t<1>, integer_sequence<int, 2, 3, 3>>);
            }

            static constexpr void EraseNonExist()
            {
                static_assert(std::is_same_v<seq1::erase_t<4>, seq1>, "Erasing non-exist element returns the original sequence unchanged");
            }

            static constexpr void EraseAt()
            {
                static_assert(std::is_same_v<seq1::erase_at_t<0>, integer_sequence<int, 2, 3>>, "Erasing front");
                static_assert(std::is_same_v<seq1::erase_at_t<seq1::size() - 1>, integer_sequence<int, 1, 2>>, "Erasing back");
                static_assert(std::is_same_v<seq1::erase_at_t<1>, integer_sequence<int, 1, 3>>, "Erasing middle");
            }

            static constexpr void Sum()
            {
                static_assert(seq1::sum() == 1 + 2 + 3);
            }

            static constexpr void AppendEmpty()
            {
                static_assert(std::is_same_v<seq1::append_t<empty>, seq1>);
                static_assert(std::is_same_v<empty::append_t<seq1>, seq1>);
                static_assert(std::is_same_v<empty::append_t<empty>, empty>);
            }

            static constexpr void AppendNothing()
            {
                static_assert(std::is_same_v<seq1::append_t<>, seq1>);
                static_assert(std::is_same_v<empty::append_t<>, empty>);
            }

            static constexpr void AppendMultipleEmpty()
            {
                static_assert(std::is_same_v <
                    decltype(seq1::append(empty{}, empty{}, empty{})),
                    seq1
                >);
            }

            static constexpr void AppendOne()
            {
                using appendSeq = integer_sequence<seq1::value_type, 4, 5, 6>;
                static_assert(std::is_same_v<
                    seq1::append_t<appendSeq>,
                    integer_sequence<seq1::value_type, 1, 2, 3, 4, 5, 6>
                >);
                static_assert(std::is_same_v<
                    appendSeq::append_t<seq1>,
                    integer_sequence<seq1::value_type, 4, 5, 6, 1, 2, 3>
                >);
            }

            static constexpr void AppendMultiple()
            {
                using appendSeq1 = integer_sequence<seq1::value_type, 4, 5, 6>;
                using appendSeq2 = integer_sequence<seq1::value_type, 7, 8, 9>;
                static_assert(
                    std::is_same_v <
                    seq1::append_t<appendSeq1, appendSeq2>,
                    integer_sequence<seq1::value_type, 1, 2, 3, 4, 5, 6, 7, 8, 9>
                >);
            }

            static constexpr void Transform()
            {
                static_assert(std::is_same_v<
                    seq1::transform_t<decltype([](auto value) {return value * 2;})>,
                    integer_sequence<int, 2, 4, 6>
                >);

                /*These 2 lambdas are not constant expression? */

                // static_assert(std::is_same_v<
                //     seq1::transform_t<decltype([i = 1](auto value)constexpr {return value * 2 + i;}) > ,
                //     integer_sequence<int, 3, 5, 7>
                // >);

                // static_assert(std::is_same_v <
                //     decltype(seq1::transform([i = 1](auto value) constexpr {return value * 2 + i;})),
                //     integer_sequence<int, 3, 5, 7>
                // >);

                static_assert(std::is_same_v <
                    seq1::transform_t<decltype([](auto value) { constexpr auto i = 1; return value * 2 + i;}) > ,
                    integer_sequence<int, 3, 5, 7>
                > );

                static_assert(std::is_same_v <
                    decltype(seq1::transform([](auto value) { constexpr auto i = 1; return value * 2 + i;})),
                    integer_sequence<int, 3, 5, 7>
                > );
                
                static_assert(std::is_same_v <
                    decltype(seq1::transform([](auto value) {return value * 2;})),
                    integer_sequence<int, 2, 4, 6>
                >);
            }

            static constexpr void RepeatZero()
            {
                static_assert(std::is_same_v<seq1::repeat_t<0>, integer_sequence<seq1::value_type>>);
            }

            static constexpr void RepeatMultiple()
            {
                static_assert(std::is_same_v<seq1::repeat_t<1>, seq1>);
                static_assert(std::is_same_v<seq1::repeat_t<2>, integer_sequence<seq1::value_type, 1, 2, 3, 1, 2, 3>>);
            }

            constexpr void SingleElementSequence()
            {
                using empty = integer_sequence<int>;
                using append_one = empty::push_back_t<1>;
                static_assert(append_one::front() == 1);
                static_assert(append_one::back() == 1);
                static_assert(append_one::at(0) == 1);

                constexpr auto array = append_one::to_array();
                static_assert(append_one::size() == 1);
                static_assert(std::is_same_v<integer_sequence<int, 1>, append_one>);
                static_assert(std::is_same_v<append_one::pop_back_t, empty>);
                static_assert(std::is_same_v<append_one::pop_front_t, empty>);
                static_assert(std::is_same_v<append_one::split_first_part_t<0>, integer_sequence<int, 1>>);
                static_assert(std::is_same_v<append_one::split_second_part_t<0>, empty>);
                static_assert(std::is_same_v<empty::push_front_t<1>, append_one>);
                static_assert(array[0] == 1);
                static_assert(std::is_same_v<decltype(array)::value_type, int>);

            }

            constexpr void MultiElementsSequence()
            {
                using test = integer_sequence<int, 1, 2, 3, 5, 10>;
                static_assert(test::size() == 5);

                static_assert(std::is_same_v<test::pop_back_t, integer_sequence<int, 1, 2, 3, 5>>);
                static_assert(std::is_same_v<test::pop_front_t, integer_sequence<int, 2, 3, 5, 10>>);

                /*insert front*/
                static_assert(std::is_same_v<test::insert_t<0, -1>, integer_sequence<int, -1, 1, 2, 3, 5, 10>>);
                /*insert back*/
                static_assert(std::is_same_v<test::insert_t<test::size(), -1>, integer_sequence<int, 1, 2, 3, 5, 10, -1>>);
                /*insert middle*/
                static_assert(std::is_same_v<test::insert_t<2, -1>, integer_sequence<int, 1, 2, -1, 3, 5, 10>>);
            }

            constexpr void MakeSequence()
            {
                static_assert(std::is_same_v<make_integer_sequence<int, 5>, integer_sequence<int, 0, 1, 2, 3, 4>>);
                static_assert(std::is_same_v<make_integer_sequence<int, 3, -3>, integer_sequence<int, -3, -2, -1, 0, 1, 2>>);
            }

            constexpr void Reverse()
            {
                static_assert(std::is_same_v<make_integer_sequence<int, 3>::reverse, integer_sequence<int, 2, 1, 0>>);
            }

            constexpr void All()
            {
                EmptySequenceProperty();
                SingleElementSequence();
                MultiElementsSequence();
            }
        }

        namespace TypeMap
        {
            struct TestTypes
            {
                struct ShouldBeInt {};
                struct ShouldBeFloat {};
                struct ShouldBeString {};
            };

            using map = type_map<
                pair<TestTypes::ShouldBeInt, int>,
                pair<TestTypes::ShouldBeFloat, float>,
                pair<TestTypes::ShouldBeString, char const*>
            >;
            
            constexpr void Mapping()
            {
                static_assert(std::is_same_v<map::value_of<TestTypes::ShouldBeFloat>, float>);
                static_assert(std::is_same_v<map::value_of<TestTypes::ShouldBeString>, char const*>);
                static_assert(std::is_same_v<map::value_of<TestTypes::ShouldBeInt>, int>);
            }

            constexpr void FindType()
            {
                
            }
        }

        namespace TypeList
        {
            using empty = type_list<>;
            
            static constexpr void EmptyListProperty()
            {
                static_assert(empty::size() == 0, "Empty type_list size: 0");
            }

            static constexpr void AppendEmpty()
            {
                static_assert(std::is_same_v<empty::append<>, empty>);
                static_assert(std::is_same_v<empty::append<empty>, empty>);

                using appendResult = type_list<int>;
                static_assert(std::is_same_v<appendResult::append<>, appendResult>);
                static_assert(std::is_same_v<appendResult::append<empty>, appendResult>);
                static_assert(std::is_same_v<empty::append<appendResult>, appendResult>);
            }
            
            static constexpr void AppendOne()
            {
                using appendResult = type_list<int>;
                static_assert(std::is_same_v<empty::append<int>, appendResult>);
                static_assert(std::is_same_v<empty::append<type_list<int>>, appendResult>);
                
            }

            static constexpr void AppendMultiple()
            {
                using appendResult = type_list<int, float, double>;
                static_assert(std::is_same_v<empty::append<int, float, double>, appendResult>);
                static_assert(std::is_same_v<empty::append<type_list<int, float, double>>, appendResult>);
                static_assert(std::is_same_v<empty::append<type_list<int, float>, type_list<double>>, appendResult>);
                static_assert(std::is_same_v<empty::append<type_list<int, float>, double>, appendResult>);
                static_assert(std::is_same_v<empty::append<int, float, type_list<double>>, appendResult>);
                static_assert(std::is_same_v<empty::append<type_list<>, int, float, double>, appendResult>);
                static_assert(std::is_same_v<empty::append<int, float, double, type_list<>>, appendResult>);

                class NonDefaultConstructible
                {
                public:
                    NonDefaultConstructible() = delete;
                };

                static_assert(std::is_same_v<empty::append<NonDefaultConstructible, NonDefaultConstructible>, type_list<NonDefaultConstructible, NonDefaultConstructible>>);
            }

            static constexpr void PrependEmpty()
            {
                static_assert(std::is_same_v<empty::prepend<>, empty>);
                static_assert(std::is_same_v<empty::prepend<empty>, empty>);

                using toPrepend = type_list<int, float, double>;
                static_assert(std::is_same_v<toPrepend::prepend<empty>, toPrepend>);
                static_assert(std::is_same_v<toPrepend::prepend<>, toPrepend>);
                
            }

            static constexpr void Prepend()
            {
                using toPrepend = type_list<int>;
                using prependResult = type_list<float, double, int>;
                
                static_assert(std::is_same_v<toPrepend::prepend<float, double>, prependResult>);
                static_assert(std::is_same_v<toPrepend::prepend<type_list<float, double>>, prependResult>);
                static_assert(std::is_same_v<toPrepend::prepend<float, type_list<double>>, prependResult>);
                static_assert(std::is_same_v<toPrepend::prepend<float, double, type_list<>>, prependResult>);
            }
            
            static constexpr void Permute()
            {

            }

            static constexpr void NThType()
            {
                using testList = type_list<int, float, double>;
                static_assert(std::is_same_v<testList::n_th_type<0>, int>);
                static_assert(std::is_same_v<testList::n_th_type<1>, float>);
                static_assert(std::is_same_v<testList::n_th_type<2>, double>);
            }

            static constexpr void Transform()
            {
                struct ShouldBeInt{};
                struct ShouldBeFloat {};
                struct ShouldBeString {};

                using transformMap = type_map <
                    pair<int, ShouldBeInt>,
                    pair<ShouldBeFloat, float>,
                    pair<const char*, ShouldBeString>
                >;

                using toTransform = type_list<int, ShouldBeFloat, const char*>;
                using transformResult = toTransform::transform<transformMap>;

                //static_assert(std::is_same_v<transformResult, type_list<ShouldBeInt, float, ShouldBeString>>);
            }

            static constexpr void Reassign()
            {
                using testList = type_list<int, float, double>;
                //static_assert(std::is_same_v<testList::reassign<0, long>, type_list<long, float, double>>);
            }

            
        }
    }

    namespace Iterator
    {
        template<typename T>
        struct InputIterator
        {
        private:
            T* ptr{};
        public:
            bool operator!=(InputIterator const& rhs) const
            {
                return true;
            }

            auto operator*()
            {
                return T{};
            }

            auto operator->()
            {
                return ptr;
            }

            InputIterator operator++()
            {
                ++ptr;
                return *this;
            }
        };

        template<typename T>
        struct NotInputIterator1
        {

        };

        // template<typename T>
        // struct NotInputIterator2
        // {
        // private:
        //     T* ptr{};
        // public:
        //     void operator!=(InputIterator const& rhs) const
        //     {
        //     }

        //     auto operator*()
        //     {
        //         return T{};
        //     }

        //     auto operator->()
        //     {
        //         return ptr;
        //     }

        //     InputIterator operator++()
        //     {
        //         ++ptr;
        //         return *this;
        //     }
        // };

        

        static constexpr auto Trait()
        {
            static_assert(details::is_input_iterator<InputIterator<int>>::value);
            //static_assert(details::is_input_iterator<int*>::value);
            static_assert(!details::is_input_iterator<NotInputIterator1<int>>::value);
            //static_assert(!details::is_input_iterator<NotInputIterator2<int>>::value);
        }
    }
}

namespace MergeSortTest
{
    namespace UseType
    {
        /*Merge*/
        template<typename FirstSortedHalf, typename SecondSortedHalf>
        struct Merge;

        template<> struct Merge<integer_sequence<int>, integer_sequence<int>> { using Merged = integer_sequence<int>; };

        template<int... Ints1> struct Merge<integer_sequence<int, Ints1...>, integer_sequence<int>> { using Merged = integer_sequence<int, Ints1...>; };

        template<int... Ints2> struct Merge<integer_sequence<int>, integer_sequence<int, Ints2...>> { using Merged = integer_sequence<int, Ints2...>; };

        template<int... Ints1, int... Ints2>
        struct Merge<integer_sequence<int, Ints1...>, integer_sequence<int, Ints2...>>
        {
        private:
            using FirstPack = integer_sequence<int, Ints1...>;
            using SecondPack = integer_sequence<int, Ints2...>;
        public:
            using Merged = std::conditional_t
            <
                (FirstPack::front() < SecondPack::front()),
                typename integer_sequence<int, FirstPack::front()>:: template append_t
                <
                    typename Merge<typename FirstPack::pop_front_t, SecondPack>::Merged
                >,
                typename integer_sequence<int, SecondPack::front()>:: template append_t
                <
                    typename Merge<FirstPack, typename SecondPack::pop_front_t>::Merged
                >
            >;
        };

        /*Sort*/
        template<typename IntegralSequence>
        struct MergeSort
        {
            constexpr static auto splitPos = IntegralSequence::size() / 2 - 1;
            using FirstHalf = typename IntegralSequence::template split_first_part_t<splitPos>;
            using SecondHalf = typename IntegralSequence::template split_second_part_t<splitPos>;
            using Sorted = typename Merge<
                typename MergeSort<FirstHalf>::Sorted,
                typename MergeSort<SecondHalf>::Sorted
            >::Merged;
        };

        template<typename T>
        struct MergeSort<integer_sequence<T>>
        {
            using Sorted = integer_sequence<T>;
        };

        template<typename T, T v>
        struct MergeSort<integer_sequence<T, v>>
        {
            using Sorted = integer_sequence<T, v>;
        };

        template<typename IntegralSequence>
        constexpr auto sort()
        {
            if constexpr (IntegralSequence::size() <= 1)
                return IntegralSequence{};
            else
            {
                constexpr auto splitPos = IntegralSequence::size() / 2 - 1;
                using FirstHalf = typename IntegralSequence::template split_first_part_t<splitPos>;
                using SecondHalf = typename IntegralSequence::template split_second_part_t<splitPos>;
                return typename Merge<
                    typename MergeSort<FirstHalf>::Sorted,
                    typename MergeSort<SecondHalf>::Sorted
                >::Merged{};
            }
        }
    }

    namespace UseFunction
    {
        template<typename FirstPack, typename SecondPack>
        constexpr auto merge()
        {
            if constexpr (FirstPack::size() == 0 && SecondPack::size() == 0)
                return integer_sequence<int>{};
            else if constexpr (FirstPack::size() == 0)
                return SecondPack{};
            else if constexpr (SecondPack::size() == 0)
                return FirstPack{};
            else
            {
                if constexpr (FirstPack::front() < SecondPack::front())
                    return typename integer_sequence<int, FirstPack::front()>::template append_t<decltype(merge<typename FirstPack::pop_front_t, SecondPack>())>{};
                else
                    return typename integer_sequence<int, SecondPack::front()>::template append_t<decltype(merge<FirstPack, typename SecondPack::pop_front_t>())>{};
            }
        }

        template<typename IntegralSequence>
        constexpr auto sort()
        {
            if constexpr (IntegralSequence::size() <= 1)
                return IntegralSequence{};
            else
            {
                constexpr auto splitPos = IntegralSequence::size() / 2 - 1;
                using FirstHalf = typename IntegralSequence::template split_first_part_t<splitPos>;
                using SecondHalf = typename IntegralSequence::template split_second_part_t<splitPos>;
                return merge<decltype(sort<FirstHalf>()), decltype(sort<SecondHalf>())>();
            }
        }
    }

    auto test()
    {
        using ToSort = integer_sequence<int, 541, 56, 23, 12, -1, 0, 7>;

        
        using ResultUsingType = decltype(UseType::sort<ToSort>());
        using ResultUsingFunction = decltype(UseFunction::sort<ToSort>());
        static_assert(std::is_same_v<ResultUsingType, integer_sequence<int, -1, 0, 7, 12, 23, 56, 541>>);
        static_assert(std::is_same_v<ResultUsingType, ResultUsingFunction>);
    }
}

#include "MakeString.hpp"
#include <cassert>

// template<typename T>
// inline void print(T&& t)
// {
//     std::cout << t << '\n';
// }

// template<typename T>
// struct C
// {
//     explicit C(T const& value) 
//     {
//         //std::cout << "T const&\n";
//     }
//     template<typename U = T, typename = std::enable_if_t<!std::is_reference_v<U>>>
//     explicit C(U&& value)
//     {
//         //std::cout << "U&&\n";
//     }

// };

// template<typename T>
// using M = C<T>;

// template<typename T>
// using MRef = C<T&>;

// template<typename T>
// using MRefRef = C<T&&>;

// template<typename T>
// C(T&&)->C<T&&>;

// template<typename T>
// C(T&)->C<T&>;

// template<typename T>
// C(T const&)->C<T const&>;


// template<typename T>
// void f(C<T const&> u)
// {
//     std::cout << "T const& "<< '\n';
// }

// template<typename T>
// void f(C<T&> u)
// {
//     std::cout << "T&" << '\n';
// }

// template<typename T>
// void f(C<T&&> u)
// {
//     std::cout << "T&&" << '\n';
// }

int main()
{
    // {
    //     int i = 0;
    //     f(C{std::move(i) });
    // }
    // {
    //     int i=0;
    //     f(C{ i });
    // }
    // {
    //     int const i = 0;
    //     f(C{ i });
    // }


    // {
    //     auto s = make_string();
    //     assert(s == "");
    // }

    // {
    //     auto s = make_string(Count{ 3 }, Character{ 'a' });
    //     assert(s == "aaa");
    // }

    // {
    //     std::string s1 = "To copy";
    //     auto s2 = make_string(Other{ s1 }, Pos{ 1 });
    //     assert(s2 == "o copy");
    // }

    {
        std::string s1 = "To copy";
        auto s2 = make_string(Other{ s1 }, Pos{ 1 }, Count{ std::string::npos });
        assert(s2 == "o copy");
    }

    // {
    //     char const* s1 = "C string";
    //     auto s2 = make_string(Pointer{ s1 }, Count{ 5 });
    //     assert(s2 == "C str");
    // }

    // {
    //     char* s1 = "C string";
        
    // }
}