#include <array>
#include <iostream>
/**
  * Given an array of length[n] containing numbers ranged from [1...n-1]
  * There will be exact one number which is repeated at least 1 time.
  * Find that number
  */

/*Using tortoise and hare algorithm */
template <typename Container>
auto FindDuplicate(Container const &nums)
{
    auto tortoise = nums[0], hare = nums[nums[0]];
    while (tortoise != hare)
    {
        tortoise = nums[tortoise];
        hare = nums[nums[hare]];
    }
    auto ptr = nums[0];
    while (ptr != nums[tortoise])
    {
        ptr = nums[ptr];
        tortoise = nums[tortoise];
    }
    return ptr;
}

int main()
{
    std::cout << FindDuplicate(std::array{2, 5, 1, 4, 3, 6, 7, 8, 3}) << '\n';
    std::cout << FindDuplicate(std::array{2, 5, 1, 2, 3, 6, 7, 8, 4}) << '\n';
    std::cout << FindDuplicate(std::array{3, 1, 3, 4, 2}) << '\n';
    std::cout << FindDuplicate(std::array{1, 4, 6, 2, 6, 3, 5}) << '\n';
}