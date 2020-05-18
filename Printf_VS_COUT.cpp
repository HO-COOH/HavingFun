#include <iostream>
#include <stdio.h>
#include <chrono>
#include <fstream>

/*Our good old Scope Timer */
class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> t;

public:
    Timer() : t(std::chrono::steady_clock::now()) {}
    ~Timer()
    {
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t).count() << "ms\n";
    }
};

void test1()
{
    printf("Using std::ifstream\n");
    Timer t;
    std::ofstream f{"File", std::ios_base::out};
    f.sync_with_stdio(false);
    for (auto i = 0; i < 1000000; ++i)
        f << i << '\n';
}

void test2()
{
    printf("Using C fprintf\n");
    Timer t;
    auto f = fopen("File", "w");
    for (auto i = 0; i < 1000000; ++i)
        fprintf(f, "%d\n", i);
}

void test3()
{
    printf("Using std::ostream\n");
    Timer t;
    std::cout.sync_with_stdio(false);
    for (auto i = 0; i < 10000; ++i)
        std::cout << i << '\n';
}

void test4()
{
    printf("Using C printf\n");
    Timer t;
    for (auto i = 0; i < 10000; ++i)
        printf("%d\n", i);
}

int main()
{
    test1();
    std::cin.get();
    test2();
    std::cin.get();
    test2();
    std::cin.get();
    test1();
}