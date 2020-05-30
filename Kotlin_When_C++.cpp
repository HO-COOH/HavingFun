/*Description: Make a kotlin style when in C++, using template
*/

#include <iostream>
#include <climits>
#include <string>

using namespace std::literals;

template <typename T, typename T2 = int, typename T3 = typename std::conditional<std::is_integral<T>::value && std::is_floating_point<T2>::value, double, int>::type>
class Range
{
    T3 start;
    T3 current;
    T3 end_v;
    const T2 step;

public:
    explicit Range(T start, T end, T2 step = 1) : start(static_cast<T3>(start)), current(static_cast<T3>(start)), end_v(static_cast<T3>(end)), step(step) {}
    class RangeIterator
    {
        T3 *ptr;
        const T2 &step;

    public:
        RangeIterator(T3 *ptr, const T2 &step) : ptr(ptr), step(step) {}
        RangeIterator &operator++()
        {
            (*ptr) += step;
            return *this;
        }
        T3 operator*() { return (*ptr); }
        bool operator!=(const RangeIterator &iter) { return abs((*ptr)) <= abs(*(iter.ptr)); }
    };
    auto begin() { return RangeIterator(&current, step); }
    auto end() { return RangeIterator(&end_v, step); }

    template <typename Num>
    friend bool operator==(Num number, Range const &rhs)
    {
        return number >= rhs.start && number <= rhs.end_v ? true : false;
    }
};

struct Else
{
};

template <typename T1, typename T2>
constexpr bool comparable = requires(T1 const &lhs, T2 const &rhs)
{
    lhs == rhs;
};

template <typename l>
struct is
{
    using type = std::remove_reference_t<l>;
};

template <typename T>
struct is_impl : std::false_type
{
};

template <typename T>
struct is_impl<is<T>> : std::true_type
{
};

auto when(auto expr, Else, auto ReturnResult)
{
    return ReturnResult;
}

auto when(auto expr, auto case1, auto ReturnResult)
{
    if constexpr (std::is_same_v<decltype(case1), bool>)
    {
        if (case1)
            return ReturnResult;
    }
    if constexpr (comparable<decltype(expr), decltype(case1)>)
    {
        if (expr == case1)
            return ReturnResult;
        else
            return decltype(ReturnResult){};
    }
    return when(expr, Else{}, ReturnResult);
}

template <typename is_type>
auto when(auto expr, is<is_type>, auto ReturnResult)
{
    if constexpr (std::is_same_v<decltype(expr), typename is<is_type>::type>)
        return ReturnResult;
    return decltype(ReturnResult){};
}

auto when(auto expr, auto case1, auto return1, auto case2, auto... args)
{
    if constexpr (std::is_same_v<decltype(case1), bool>)
    {
        if (case1)
            return return1;
    }
    if constexpr (comparable<decltype(expr), decltype(case1)>)
    {
        if (expr == case1)
            return return1;
        else
            return when(expr, case2, args...);
    }
    return when(expr, case2, args...);
}

template <typename is_type>
auto when(auto expr, is<is_type>, auto return1, auto case2, auto... args)
{
    if constexpr (std::is_same_v<decltype(expr), typename is<is_type>::type>)
        return return1;
    return when(expr, case2, args...);
}

int main()
{
    int temperature = 10;
    puts(when((temperature),
              Range(INT_MIN, 0), "freezing",
              Range(1, 15), "cold",
              Range(16, 20), "cool",
              Range(21, 25), "warm",
              Range(26, INT_MAX), "hot",
              Else(), "WTF?")); //cold
    // auto describe = [](auto&& obj) {
    //     return when((obj),
    //         1, "One"s,
    //         "hello"s, "Greeting"s,
    //         std::is_same_v<long, std::remove_reference_t<decltype(obj)>>, "long"s,
    //         !std::is_same_v<std::string, std::remove_reference_t<decltype(obj)>>, "Not a string"s,
    //         Else(), "Unknown string"s);
    // };
    auto describe = [](auto &&obj) {
        return when((obj),
                    1, "One"s,
                    "hello"s, "Greeting"s,
                    is<long>{}, "long"s,
                    !std::is_same_v<std::string, std::remove_reference_t<decltype(obj)>>, "Not a string"s,
                    Else(), "Unknown string"s);
    };
    std::cout << describe(1) << '\n';
    std::cout << describe("hello"s) << '\n';
    std::cout << describe(1000l) << '\n';
    std::cout << describe(2) << '\n';
    std::cout << describe("random string"s) << '\n';
}
//Output:
//cold

//One

//Greeting

//long

//Not a string

//Unknown string
