#include <iostream>
#include <string>

void restore(std::istream& is)
{
    std::cin.clear();
    while (std::cin.get() != '\n')
        ;
}

template <typename T>
[[nodiscard]]T input(const char* prompt = nullptr, bool retry=false)
{
    T i;
    if(!retry)
    {
        if (prompt)
            std::cout << prompt;
        if (!std::cin)
            restore(std::cin);
        std::cin >> i;
    }
    else 
    {
        do
        {
            if (!std::cin)
                restore(std::cin);
            if (prompt)
                std::cout << prompt;
            if constexpr (std::is_integral_v<T>)
            {
                //handles negative number and convert to its absolute value when expected an unsigned
                if (std::cin.peek() == '-') 
                {
                    std::make_signed_t<T> i_abs;
                    std::cin >> i_abs;
                    if (!std::cin)
                        continue;
                    restore(std::cin);
                    return abs(i_abs);
                }
            }
            std::cin >> i;
        } while (!std::cin);
    }
    restore(std::cin);
    return i;
}

template <>
[[nodiscard]]std::string input(const char* prompt, bool retry)
{
    std::string s;
    if(!retry)
    {
        if (prompt)
            std::cout << prompt;
        if (!std::cin)
            restore(std::cin);
        std::getline(std::cin, s);
    }
    else
    {
        do {
            if (!std::cin)
                restore(std::cin);
            if (prompt)
                std::cout << prompt;
            std::getline(std::cin, s);
        } while (s.empty());
    }
    restore(std::cin);
    return s;
}

template<typename T>
T input(std::string const& prompt, bool retry = false)
{
    return input<T>(prompt.c_str(), retry);
}

#if __cplusplus >= 201703L
#include <string_view>
template<typename T>
T input(std::string_view prompt, bool retry = false)
{
    return input<T>(prompt.data(), retry);
}
#endif


/**************************FOR TESTING*****************************/
void printSeparator()
{
    std::cout << "----------------------------------";
}

template<typename Type>
void printResult(Type result)
{
    std::cout << "You entered: " << result << '\n';
}

template<typename Type>
void test()
{
    const auto value = input<Type>((std::string{ "Enter a " } + typeid(Type).name()).c_str(), true);
    printResult<Type>(value);
}

int main()
{
    test<int>();
    test<unsigned>();
    test<char>();
    test<double>();
    test<float>();
    test<std::string>();
    test<long long>();
    test<unsigned long long>();
}
