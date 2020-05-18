#include <iostream>
#include <vector>
#include <cstring>
#include <iterator>

/*来介绍一个高效的字符串匹配算法：KMP算法
 *在此之前我们先来看看最简单的暴力算法是如何匹配两个字符串的：
 * 对于两个字符串：原字符串text、待匹配字符串pattern，我们假定原字符串的长度>=待匹配字符串长度（不然肯定不匹配呀）
 * 定义两个指针，分别指向原字符串和待匹配字符串的开头，再定义一个索引，记录当前是从原字符串的第几个字符开始匹配的
 * 然后如果它们匹配，我们就对这两个指针都后移
 * 如果待匹配字符串指针已经移到最后了，那么就找到了一个匹配~！
 * 如果中间出现了不匹配，我们将索引后移1位，原字符串的指针返回到索引位置，将待匹配字符串的指针返回到待匹配字符串的起始位置
 * 当索引大于原字符串长度 减去 待匹配字符串的长度时，（也就是如果将待匹配字符串在索引位置对齐，待匹配字符串的尾巴已经超过原字符串的尾巴啦，所以不可能会匹配），此时比较结束
 * 写成算法就是：
 * */
void naive_match(const char *text, const char *pattern)
{
    const size_t text_length = strlen(text);
    const size_t pattern_length = strlen(pattern);                           //养成好习惯，常量就要声明成const
    for (size_t start = 0; start <= (text_length - pattern_length); ++start) //一个索引
    {
        for (size_t text_ptr = start, pattern_ptr = 0; pattern_ptr < pattern_length; ++text_ptr, ++pattern_ptr) //两个指针，原字符串指针从索引位置开始，待匹配字符串的指针从0开始
        {
            if (text[text_ptr] != pattern[pattern_ptr]) //找到不匹配的字母啦！本轮比较结束，所以退出这个循环，更新索引
                break;
            else if (pattern_ptr == pattern_length - 1)          //如果指向待匹配字符串的指针已经挪到最后一个位置了，依然匹配，所以找到了一个匹配！如果还没有到最后，就什么都不做，继续这个循环
                std::cout << "Found at index " << start << '\n'; //把匹配的位置输出一下~
        }
    }
}
/*做亿点点分析：我们发现在最坏的情况下，原字符串的所有字符都跟待匹配的字符串的所有字符都比较了一遍
 * 那么假设原字符串长度为M，待匹配字符串的长度为N，那么这个算法的时间复杂度为O(M*N)，显然是不太理想的T_T
 * 如果可以避免每次在出现不匹配后原字符串指针回到索引位置，而是从当前位置（原字符串中与待匹配字符串不匹配的字母的位置）开始，继续与待匹配字符串的某个位置开始比较，这样就能大幅减少时间开销
 * 关键在于需要在待匹配字符串上获取一些信息，具体是什么信息呢？
 * 我们来想，当找到一个不匹配的字母，如果这个字母不是待匹配字符串的第一个的话，那么在它前面的字母已经是匹配的了！
 * 所以我们如果能跳过待匹配字符串的匹配部分的结尾部分与开头部分相同的部分（有点绕口xixi）
 * 就可以省下这个匹配的部分再与待匹配字符串的开头部分重新比较
 * 举个栗子：
 * 原字符串: a b c a b  c a b ...
 *                ^ ^  | 不匹配
 * 待匹配串:[a b c a b] y           -> 匹配部分是[a b c a b]，因为结尾的[a b]与开头的[a b]相同，所以可以跳过待匹配字符串中开头的[a b]与原字符串的比较
 * 跳!   :        a b  c a b y
 *
 * 再举个栗子：
 *
 * 所以我们需要的信息就是待匹配字符串的各个位置上，开头部分与当前位置的结尾部分中相同的字母的个数
 * 严格来讲，就是对于i从0-字符串长度，字符串[0..i]的这部分子串，开头部分与结尾部分中相同字母的个数
 * AAAA -> [0,1,2,3]
 * ABCDE -> [0,0,0,0,0]
 * AABAACAABAA -> [0,1,0,1,2,0,1,2,3,4,5]
 * AAACAAAAAC -> [0,1,2,0,1,2,3,3,3,4]
 * AAABAAA -> [0,1,2,0,1,2,3]
 */

std::vector<size_t> preprocess(const char *pattern)
{
    const size_t length = strlen(pattern);
    std::vector<size_t> lps(length);
    lps[0] = 0;
    size_t lps_pre = 0;
    size_t i = 1;
    while (i < length)
    {
        if (pattern[i] == pattern[lps_pre])
        {
            ++lps_pre;
            lps[i] = lps_pre;
            ++i;
        }
        else
        {
            if (lps_pre != 0)
                lps_pre = lps[lps_pre - 1];
            else
            {
                lps[i] = 0;
                ++i;
            }
        }
    }
    return lps;
}

void print(std::vector<size_t> const &lps)
{
    std::copy(lps.cbegin(), lps.cend(), std::ostream_iterator<size_t>{std::cout, ", "});
    std::cout << '\n';
}

void KMP(const char *text, const char *pattern)
{
    size_t M = strlen(pattern);
    size_t N = strlen(text);

    auto lps = preprocess(pattern);

    size_t i = 0; //text index
    size_t j = 0; //pattern index
    while (i < N)
    {
        if (pattern[j] == text[i])
        {
            ++i;
            ++j;
        }
        else
        {
            if (j == M)
            {
                std::cout << "Found pattern at index " << i - M << '\n';
                j = lps[j - 1];
            }
            else //pattern[j] != text[i] && j != M
            {
                if (j != 0)
                    j = lps[j - 1];
                else
                    ++i;
            }
        }
    }
}

int main()
{
    //    print(preprocess("AAAA"));
    //    print(preprocess("ABCDE"));
    //    print(preprocess("AABAACAABAA"));
    //    print(preprocess("AABAACAABAAD"));
    //    print(preprocess("AAACAAAAAC"));
    //    print(preprocess("AAABAAA"));
    naive_match("AABAACAADAABAAABAA", "AABA"); //测试一下我们的简单暴力方法，答案应该是0,9,13
    //KMP("AAAAABAAABA", "AAAA");
}