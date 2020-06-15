/*Description: Write a function to determine whether a number is a power of 2
*/
#include <iostream>
/*
    A negative number, say [x], is represented as [~x+1] in two's-complement-form.
    That is: [-x == ~x + 1] ----{Formula 1}
    If there is an all-zero suffix in the original number [x], for example: Prefix 1 000
    Then after bit not, ~x, it becomes: ~Prefix 0 111
    Finally after +1, ~x+1, it becomes: ~Prefix 1 000
    And if you do: x & (~x+1), which according to {Formula 1}, is x & (-x), it becomes: [Prefix 1 000] & [~Prefix 1 000]
    You get: [Prefix part all 0, 1, 000]

    So if the original number has a single 1, x & (~x+1) will be equal to x
    Otherwise, if there are two 1s, so there are another 1 in the Prefix part, after this operation, is won't be equal to x.

    A special case is when x is 0, because -0 is 0 so it satisfies the operation but we know 0 is not a power of 2.
*/

inline bool is_power_of_two(uint64_t n)
{
    return (n != 0) && ((n & -n) == n);
}

/*C++11 added the <bitset> library which enables us to directly count the number of true bits in a bitset*/
#include <bitset>
inline bool is_power_of_two_2(uint64_t n)
{
    return std::bitset<sizeof(n) * 8>{n}.count() == 1;
}

/*C++20 added the <bit> library which directly implement this functionality*/
#include <bit>
inline bool is_power_of_two_3(uint64_t n)
{
    return std::has_single_bit(n);
}

int main()
{
    puts("Using our algorithm: ");
    for (auto i = 0ull; i < 100; ++i)
    {
        if (is_power_of_two(i))
            std::cout << i << '\n';
    }
    puts("Using std::bitset");
    for (auto i = 0ull; i < 100; ++i)
    {
        if (is_power_of_two_2(i))
            std::cout << i << '\n';
    }
    puts("Using std::has_single_bit");
    for (auto i = 0ull; i < 100; ++i)
    {
        if (is_power_of_two_2(i))
            std::cout << i << '\n';
    }
}