/*Description: Implement a Boyer Moore string matching algorithm
* https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string-search_algorithm
* The key insight of Boyer-Moore Searching algorithm is that:
* 1. If the last char in pattern does not match text, there is no need to continue searching backwards.
* 2. If the chars in the text does not match any char in the pattern, then the next char to check is [n] char farther along text. (n == length(pattern))
* 3. Otherweise (there are chars in the text that appears in the pattern), then a partial shift is done to align the matching character.
*/
#include <string_view>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

/*@ Build up a character table of pattern string for lookup
* @ param pattern: the pattern string
* @ return: an array of ASCII characters whose value is the highest index of occurence (last occurence) in @pattern
*/
constexpr int alphabet = 128;
std::array<int, alphabet> make_char_table(std::string_view pattern)
{
    std::array<int, alphabet> char_table;
    std::fill(char_table.begin(), char_table.end(), -1);
    for (auto i = 0; i < pattern.size(); ++i)
        char_table[pattern[i]] = std::max(i, char_table[pattern[i]]);
    return char_table;
}

/*@ Return a shift according to the bad character rule
*/
int bad_char_rule(std::string_view text, std::string_view pattern, const std::array<int, alphabet>& char_table)
{
    auto text_iter = text.cbegin() + pattern.length() - 1;
    for (auto pattern_iter = pattern.crbegin(); pattern_iter != pattern.crend(); ++pattern_iter, --text_iter)
    {
        if (*pattern_iter != *text_iter)
            return char_table[*text_iter] != -1 ? pattern.length() - char_table[*text_iter] : pattern.length();
    }
    return 0;
}

std::vector<int> make_suffix_table(std::string_view pattern)
{
    std::vector<int> suffix_table(pattern.length(), -1);
    for (int i = pattern.length() - 1; i >= 0; --i)
    {
        //if pattern[i..n] == pattern[0..L[i]]
        for (int l = 0; l < pattern.length() - i; ++l)
        {
            //      P[0..l]                                     P[i..n]
            if (std::string_view(pattern.data(), l) == std::string_view(pattern.data() + i, pattern.length() - i))
                suffix_table[i] = std::max(suffix_table[i], l);
        }
        if (suffix_table[i] == -1)
            suffix_table[i] = 0;
    }
    return suffix_table;
}

std::vector<int> make_max_suffix_table(std::string_view pattern)
{
    std::vector<int> max_table(pattern.length(), -1);
    for (int i = pattern.length() - 1; i >= 0; --i)
    {
        if (std::string_view{ pattern.data() + i, pattern.length() - i } == std::string_view{ pattern.data(), pattern.length() - i })
            max_table[i] = pattern.length() - i;
        else
            max_table[i] = 0;
    }
    return max_table;
}

/*@return the maximum length of the P[pos-length..pos] == P[n-length..n]
*
*/
int suffixLength(std::string_view pattern, int pos)
{
    int length = 0;
    for (int i = pos, j = pattern.length() - 1; i >= 0 && pattern[i] == pattern[j]; --i, --j)
        ++length;
    return length;
}

std::vector<int> make_offset_table(std::string_view pattern)
{
    std::vector<int> table(pattern.length(), 0);
    int lastPrefixPos = pattern.length();
    for (int i = pattern.length() - 1; i > 0; --i)
    {
        //if P[0..n-i] == P[i..n] ( P[i..n] is a prefix of P )
        if (std::string_view{ pattern.data(), pattern.length() - i } == std::string_view{ pattern.data() + i, pattern.length() - i })
            lastPrefixPos = i;
        table[pattern.length() - i] = lastPrefixPos - i;
    }
    for (int i = 0; i < pattern.length() - 1; ++i)
    {
        int slength = suffixLength(pattern, i);
        table[slength] = pattern.length() - 1 - i;
    }
    return table;
}

//int good_suffix_rule(std::string_view text, std::string_view pattern, const std::vector<int>& suffix_table, const std::vector<int>& max_suffix_table)
//{
//    auto text_iter = text.cbegin() + pattern.length() - 1;
//    for (auto pattern_iter = pattern.crbegin(); pattern_iter != pattern.crend(); ++pattern_iter, --text_iter)
//    {
//        auto i = std::distance(&(*pattern.crbegin()), &(*pattern_iter));
//        if (*pattern_iter != *text_iter)
//            return pattern.length() - (suffix_table[i] != 0 ? suffix_table[i] : max_suffix_table[i]);
//    }
//    return 0;
//}
//

int good_suffix_rule(std::string_view text, std::string_view pattern, const std::vector<int>& offset_table)
{
    for (int i = pattern.length() - 1; i > 0; --i)
    {
        if (pattern[i] != text[i])
            return offset_table[i];
    }
    return -1;
}

std::vector<int> boyer_moore(std::string_view text, std::string_view pattern) //return occurences
{
    std::vector<int> occurence;
    auto char_table = make_char_table(text);
    auto offset_table = make_offset_table(pattern);
    int shift = 0;
    while (shift + pattern.length() <= text.length())
    {
        if (auto shifted_text = std::string_view{ text.data() + shift, pattern.length() }; shifted_text != pattern)
            shift += std::max(bad_char_rule(shifted_text, pattern, char_table), good_suffix_rule(shifted_text, pattern, offset_table));
        else
            occurence.push_back(shift);
    }
    return occurence;
}

void test(std::string_view text, std::string_view pattern)
{
    int i = 0;
    auto char_table = make_char_table(pattern);
    auto offset_table = make_offset_table(pattern);

    std::cout << "Char table:\n";
    for (auto count : char_table)
    {
        if (count != -1)
            std::cout << (char)i << " -> " << count << '\n';
        ++i;
    }


    std::cout << bad_char_rule(text, "lo", char_table) << '\n';
}

int main()
{
    test("helloworld", "lo");
}