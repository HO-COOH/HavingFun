// @before-stub-for-debug-begin
#include <vector>
#include <string>

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode id=122 lang=cpp
 *
 * [122] Best Time to Buy and Sell Stock II
 */

 // @lc code=start
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices)
    {
        int profit{};
        for (size_t i = 1; i < prices.size(); i++)
            profit += std::max(prices[i] - prices[i - 1], 0);
        return profit;
    }
};
// @lc code=end

