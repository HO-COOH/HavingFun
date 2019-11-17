#include <vector>
#include <array>
#include <iostream>

using std::cout;

template<typename T, int Size=-1>  //if Size given, use std::array stuff
class MyArray:public std::array<T, Size>
{
public:
	void func() { cout << "2 args\n"; }
};

template<typename T> //if Size is not given, use std::vector stuff
class MyArray<T, -1>:public std::vector<T>
{
public:
	void func() { cout << "1 args\n"; }
};
int main()
{
	MyArray<int, 3> a{ 1,2,3 };
	a.func();
	MyArray<int>b;
	b.func();
}