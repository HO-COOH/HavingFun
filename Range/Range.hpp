#pragma once
#include <type_traits>
//class Range
//{
//	std::variant<int, double> current;
//	std::variant<int, double> end_v;
//	const std::variant<int, double> step;
//public:
//	explicit Range(int start, int end) :current(start), end_v(end), step(1) {}
//	explicit Range(int start, int end, int step) :current(start), end_v(end), step(step) {}
//	explicit Range(int start, int end, double step) :current(start), end_v(end), step(step) {}
//	explicit Range(double start, double end, double step): current(start), end_v(end), step(step) {}
//	
//	template<typename T>
//	friend class RangeIterator;
//
//	auto begin() 
//	{ 
//		if (current.index() == 0)
//		{
//			return RangeIterator(std::get_if<int>(&current));
//		}
//		if (current.index() == 1)
//		{
//			return RangeIterator(std::get_if<double>(&current));
//		}
//	}
//
//	auto end() 
//	{ 
//		if (current.index() == 0)
//		{
//			return RangeIterator(std::get_if<int>(&end));
//		}
//		if (current.index() == 1)
//		{
//			return RangeIterator(std::get_if<double>(&end));
//		}
//	}
//};
//
//class RangeIterator
//{
//	T* ptr;
//public:
//	RangeIterator(const T* value) :ptr(value) {}
//	T operator*() { return ptr; }
//	bool operator!=(const RangeIterator<T>& iter2) { return *ptr != iter2->ptr; }
//	void operator++() { ++(*ptr); }
//};


//When T is int and T2 is double, we need to change T -> double
template <typename T, typename T2=int, typename T3=typename std::conditional<std::is_integral<T>::value&&std::is_floating_point<T2>::value,double,int>::type>
class Range
{
	T3 current;
	T3 end_v;
	const T2 step;
public:
	explicit Range(T start, T end, T2 step=1) :current(static_cast<T3>(start)), end_v(static_cast<T3>(end)), step(step) {}
	class RangeIterator
	{
		T3* ptr;
		const T2& step;
	public:
		RangeIterator(T3* ptr, const T2& step) :ptr(ptr),step(step)  {}
		RangeIterator& operator++() { (*ptr) += step; return *this; }
		T3 operator*() { return (*ptr); }
		bool operator!=(const RangeIterator& iter) { return	abs((*ptr)) <= abs(*(iter.ptr)); }
	};
	auto begin() { return RangeIterator(&current, step); }
	auto end() { return RangeIterator(&end_v, step); }
};

