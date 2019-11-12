#include <thread>
#include <chrono>
#include <atomic>
#include <future>
#include <iostream>
#include <memory>

using namespace std;
static atomic<bool> flag = false;

size_t test()
{
	size_t i = 0;
	while (!flag)
	{
		auto ptr = make_unique<int>(1000);
		i++;
	}
	return i;
}

size_t test2()
{
	size_t i = 0;
	while (!flag)
	{
		int pt[1000];
		i++;
	}
	return i;
}

int main()
{	
	auto result_heap = async(launch::async, test);
	this_thread::sleep_for(chrono::seconds(1));
	flag = true;
	cout << "heap allocation=" << result_heap.get() << "/s";
    flag = false;
    auto result_stack = async(launch::async, test2);
    this_thread::sleep_for(chrono::seconds(1));
    flag = true;
    cout<<"\nstack allocation=" << result_stack.get() << "/s\n";
}

//In Visual Studio 2019 DEBUG:
// heap allocation=1436945/s
// stack allocation=9934232/s

//In GCC O0:
// heap allocation=1641543/s
// stack allocation=194357927/s

//In Visual Studio 2019 RELEASE:
// heap allocation=21740491/s
// stack allocation=2792612857/s

//In GCC O1:
// heap allocation=2308501/s
// stack allocation=2927207784/s

//In GCC O3
// heap allocation=2211887/s
// stack allocation=3217322418/s

//In GCC Ofast:
// heap allocation=2361879/s
// stack allocation=3256246411/s