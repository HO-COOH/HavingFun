/* Description: Implement a better compile-time integer sequence
* The std::integer_sequence in <utility> has two helper functions: std::make_integer_sequence & std::make_index_sequence
* The former one accepts a type parameter, and the latter is specialized to size_t.
* Their limitation is they only starts at 0 and extends to N-1 as specified.
* Let's make a better integer sequence generator that can is ranged from any integer range you specified.
*/

#include <iostream>
#include <utility>

/* The principle being that each recursion, we "extract" the second element, and "extend" it backward by 1, until it reaches [start]
* For example: [1]..[4]     -> [1][3][4].. -> [1][2][3]..[4]
                |    |          |  |  |        |  |  |
            start    mid    start mid end  start mid end
*/
template<int start, int mid, int... end>
auto make_sequence()
{
    if constexpr (start==mid-1)
        return std::integer_sequence<int, start, mid, end...>{};
    else
        return make_sequence<start, mid-1, mid, end...>();
}

//Here is a function that verifies and prints out the integer in the sequence 1 by 1, seperated by a new line.
//Because why not?
template<int... seq>
void print(std::integer_sequence<int, seq...>)
{
    ((std::cout << seq << '\n'), ...);
}

int main()
{
    print(make_sequence<-10, 20>());
}
//Output:
// -10

// -9

// -8

// -7

// -6

// -5

// -4

// -3

// -2

// -1

// 0

// 1

// 2

// 3

// 4

// 5

// 6

// 7

// 8

// 9

// 10

// 11

// 12

// 13

// 14

// 15

// 16

// 17

// 18

// 19

// 20