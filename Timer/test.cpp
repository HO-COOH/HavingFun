#include "Timer.hpp"
#include <thread>
int main()
{
    std::cerr<<sizeof(size_t)<<'\n';
    Timer t{true, true};
    std::this_thread::sleep_for(std::chrono::seconds{1});
}