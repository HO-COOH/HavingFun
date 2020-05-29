#include <iostream>
#include <vector>
#include <Windows.h>

template <typename T>
struct MyAllocator : std::allocator<T>
{

    auto allocate(size_t size)
    {
        std::cout << "Allocating: " << size << " elements\n";
        return new T[size];
    }

    void deallocate(T *ptr, size_t size)
    {
        std::cout << "Free: " << size << " elements\n";
        delete[] ptr;
    }

    /*uncomment the following for GCC or clang*/
    template <typename U>
    struct rebind
    {
        using other = MyAllocator<U>;
    };
};

template <typename T, size_t N>
void print_arr(T (&arr)[N])
{
    std::cout << "Element type: " << typeid(T).name() << ", size=" << N << '\n';
}

void *operator new(size_t size)
{
    puts("Malloc called");
    return malloc(size);
}

#include <string>
using namespace std::literals;

int main()
{
    int *null_int = nullptr;
}