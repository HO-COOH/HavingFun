/*
 * @lc app=leetcode id=350 lang=cpp
 *
 * [350] Intersection of Two Arrays II
 */

// @lc code=start
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iterator>

using namespace std;
/*
Solution runtime 100% memory: 10M
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2)
    {
        std::vector<int> result;
        std::sort(nums1.begin(), nums1.end());
        std::sort(nums2.begin(), nums2.end());
        std::set_intersection(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), std::back_inserter(result));
        return result;
    }
*/
class Solution //Runtime 68% memory: 10.7M
{
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2)
    {
        std::vector<int> result;
        std::unordered_map<int, int> map;
        for (auto v : nums1)
            ++map[v];
        for (auto v : nums2)
        {
            if (auto iter = map.find(v); iter != map.cend() && iter->second > 0)
            {
                --iter->second;
                result.push_back(v);
            }
        }
        return result;
    }
};
// @lc code=end

