// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem14.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode id=14 lang=cpp
 *
 * [14] Longest Common Prefix
 */

 // @lc code=start
#include <string>
#include <vector>
#include <algorithm>
using namespace std;



class Solution {
public:
    string longestCommonPrefix(vector<string>& strs)
    {
        if (strs.empty())
            return "";

        std::string result;
        for (auto i = 0;i < strs.front().length(); ++i)
        {
            char const c = strs[0][i];
            if (std::all_of(std::begin(strs), std::end(strs), [c, i](std::string const& str) { return i < str.length() && str[i] == c; }))
                result += c;
            else
                break;
        }
        return result;
    }
};
// @lc code=end

