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
//heap allocation=10955000/s
//stack allocation=196030179/s

//In Visual Studio 2019 RELEASE:
// heap allocation=21740491/s
// stack allocation=2792612857/s

//In GCC O1:
// heap allocation=24767904/s
// stack allocation=3165869671/s

//In GCC O3
// heap allocation=25234282/s
// stack allocation=3193252804/s

//In GCC Ofast:
// heap allocation=24477634/s
// stack allocation=2780368205/s