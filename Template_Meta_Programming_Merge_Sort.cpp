/**Description: Let's do a template meta programming version of merge sort.
*/
#include <iostream>
#include <utility>

template<int... Val>    //a integer sequence template, similar to std::integer_sequence
struct seq{};

template<int... Val>
void print_seq(seq<Val...>) //a function for printing the sequence
{
    ((std::cout << Val <<' '), ...);
    std::cout<<'\n';
}

/*Divide*/
template<typename First, typename Second>
struct divide;

template<int... First, int Middle, int... Second>
struct divide<seq<First...>, seq<Middle, Second...>>
    : std::conditional_t<sizeof...(First) < 1+sizeof...(Second), 
                        divide<seq<First..., Middle>, seq<Second...>>,
                        std::pair<seq<First...>, seq<Middle, Second...>>>
{};

template<int... First>
struct divide<seq<First...>, seq<>> : std::pair<seq<First...>, seq<>> {};

/*Merge*/
template<typename Result, typename First, typename Second>
struct merge;

template<int... Result>
struct merge<seq<Result...>, seq<>, seq<>> : std::enable_if<true, seq<Result...>> {};

template<int... Result, int FirstHead, int... FirstTail> 
struct merge<seq<Result...>, seq<FirstHead, FirstTail...>, seq<>> : std::enable_if<true, seq<Result..., FirstHead, FirstTail...>> {};

template<int... Result, int SecondHead, int... SecondTail>
struct merge<seq<Result...>, seq<>, seq<SecondHead, SecondTail...>> : std::enable_if<true, seq<Result..., SecondHead, SecondTail...>> {};

template<int... Result, int FirstHead, int... FirstTail, int SecondHead, int... SecondTail>
struct merge<seq<Result...>, seq<FirstHead, FirstTail...>, seq<SecondHead, SecondTail...>>
: std::conditional_t<(FirstHead<SecondHead),
    merge<seq<Result..., FirstHead>, seq<FirstTail...>, seq<SecondHead, SecondTail...>>,
    merge<seq<Result..., SecondHead>, seq<FirstHead, FirstTail...>, seq<SecondTail...>>
    >
{};


/*Sort*/
template<typename ToSort>
struct sort;

template<>
struct sort<seq<>> : std::enable_if<true, seq<>> {};

template<int Num>
struct sort<seq<Num>> : std::enable_if<true, seq<Num>> {};

template<int Num1, int Num2>
struct sort<seq<Num1, Num2>>
: std::conditional<(Num1 < Num2),
    seq<Num1, Num2>,
    seq<Num2, Num1>
    >
{};

template<int... ToSort>
struct sort<seq<ToSort...>>
{
    using Divided=divide<seq<>, seq<ToSort...>>;
    using Merged=merge<
        seq<>,
        typename sort<typename Divided::first_type>::type,
        typename sort<typename Divided::second_type>::type
    >;
    using type=typename Merged::type;
};

int main()
{
    using ToSort=sort<seq<541,56,23,12,4,789,6>>;
    print_seq(ToSort::type{});
}