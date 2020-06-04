#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <numeric>

class Timer
{
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
public:
	//microseconds = 1/1,000,000 seconds
	//because 1 milliseconds is too long for all algorithms here except the normal recursive version 
	~Timer() { std::cout << "Finished in " << (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-now)).count()<<" microsec\n"; }
};

/*We have a recursive version */
uint64_t fib(uint64_t i)
{
	return i == 0 ? 0 : (i == 1 ? 1 : (fib(i - 1) + fib(i - 2)));
}

/*We can make the recursive version better by using dynamic programming */
uint64_t fib_better_recursive_impl(std::vector<uint64_t>& result, uint64_t i)
{
	if (i == 0 || i == 1)
		return result[i];
	if (result[i] != 0)
		return result[i];
	result[i] = fib_better_recursive_impl(result, i - 1) + fib_better_recursive_impl(result, i - 2);
	return result[i];
}
uint64_t fib_better_recursive(uint64_t i)
{
	if (i == 0 || i == 1)
		return i;
	std::vector<uint64_t> result(i+1, 0);
	result[0] = 0;
	result[1] = 1;
	return fib_better_recursive_impl(result, i);
}

/*We have a non_recursive version */
uint64_t fib_non_recursive(uint64_t i)
{
	if (i == 0 || i == 1)
		return i;
	uint64_t cur{};
	uint64_t pre{ 1 };
	uint64_t pre_2{};
	while (--i)
	{
		cur = pre + pre_2;
		pre_2 = pre;
		pre = cur;
	}
	return cur;
}

/*std::adjacent_difference is also a non-recursive version */
uint64_t fib_std_adjacent_difference(uint64_t i)
{
	std::vector v(i, 1ull); //first 50 numbers of fibonacci
	std::adjacent_difference(v.begin(), v.end() - 1, v.begin() + 1, std::plus<>{});
	return v[i - 1];
}

/*Let's try compile time constant */
template<uint64_t I>
struct Fib
{
	static const uint64_t val = Fib<I - 1>::val + Fib<I - 2>::val;
};

template<>
struct Fib<0>
{
	static const uint64_t val = 0;
};

template<>
struct Fib<1>
{
	static const uint64_t val = 1;
};

template<size_t ...I>
uint64_t fib_impl(std::index_sequence<I...>, int i)
{
	constexpr static std::array<uint64_t, sizeof...(I)> fib_result{ Fib<I>::val... };
	return fib_result[i];
}

uint64_t fib_constexpr(int i)
{
	return fib_impl(std::make_index_sequence<70>(), i);
}

/*We can also do template meta programming using if-constexpr*/
template<unsigned long long N>
constexpr unsigned long long fib()
{
	if constexpr (N == 0)
		return 0ull;
	else if constexpr (N == 1)
		return 1ull;
	else
		return fib<N - 1>() + fib<N - 2>();
}


int main()
{
	uint64_t i = 0;
	std::cin >> i;
	{
		Timer t;
		std::cout <<"\nUsing Non-Recursive: " <<fib_non_recursive(i) << '\n';
	}
	{
		Timer t;
		std::cout <<"\nUsing compile time calculated values: " << fib_constexpr(static_cast<int>(i)) << '\n';
	}
	{
		Timer t;
		std::cout << "\nUsing template metaprogramming with constexpr if: " << fib<50>() << '\n';
	}
	{
		Timer t;
		std::cout << "\nUsing Dynamic Programming: " << fib_better_recursive(i) << '\n';
	}
	{
		Timer t;
		std::cout <<"\nUsing Normal Recursive: " << fib(i) << '\n';
	}
	{
		Timer t;
		std::cout << "\nUsing std::adjacent_difference" << fib_std_adjacent_difference(i) << '\n';
	}
}