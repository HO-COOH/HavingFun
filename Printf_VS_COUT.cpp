#include <iostream>
#include <stdio.h>
#include <chrono>

/*Our good old Scope Timer */
class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> t;
public:
    Timer():t(std::chrono::steady_clock::now()) {}
    ~Timer()
    {
        std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-t).count()<<"ms\n";
    }
};

void test1()
{
    Timer t;
    //std::cout.sync_with_stdio(false);
    for(auto i=0; i<100000; ++i)
        std::cout<<i<<'\n';
}

void test2()
{
    Timer t;
    for(auto i=0; i<100000; ++i)
        printf("%d\n", i);
}

int main()
{
    test1();
    std::cin.get();
    test2();
}