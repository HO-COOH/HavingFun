#include <cstdio>
class MyClass
{
public:
    MyClass()
    {
        puts("Object constructed!");
    }
    ~MyClass()
    {
        puts("Object destroyed!");
    }
    MyClass(MyClass const& obj)
    {
        puts("Copy constructor!");
    }
    MyClass(MyClass&& obj)noexcept
    {
        puts("Move constructor!");
    }
    MyClass& operator=(MyClass const& obj)
    {
        puts("=Copy");
        return *this;
    }
    MyClass& operator=(MyClass&& obj)noexcept
    {
        puts("=Move");
        return *this;
    }
    void dostuff() const
    {
        puts("Doing stuff in MyClass");
    }
};

template<typename T>
class CreationCounter
{
    static inline unsigned copyConstructed{};
    static inline unsigned moveConstructed{};
    static inline unsigned copyAssigned{};
    static inline unsigned moveAssigned{};
public:
    CreationCounter() = default;
    CreationCounter(CreationCounter const& obj) { ++copyConstructed; }
    CreationCounter(CreationCounter&& obj)noexcept { ++moveConstructed; }
    CreationCounter& operator=(CreationCounter const& obj) { ++copyAssigned; return *this; }
    CreationCounter& operator=(CreationCounter&& obj)noexcept { ++moveAssigned; return *this; }
    ~CreationCounter() = default;
    static void printAll()
    {
        std::cout << "Copy construct/assigned: " << copyConstructed << " / " << copyAssigned << '\t' << "Move construct/assigned: " << moveConstructed << " / " << moveAssigned << '\n';
    }
};

template<>
class CreationCounter<void>
{
    unsigned copyConstructed{};
    unsigned moveConstructed{};
    unsigned copyAssigned{};
    unsigned moveAssigned{};
public:
    CreationCounter() = default;
    CreationCounter(CreationCounter const& obj) { ++copyConstructed; }
    CreationCounter(CreationCounter&& obj)noexcept { ++moveConstructed; }
    CreationCounter& operator=(CreationCounter const& obj) { ++copyAssigned; return *this; }
    CreationCounter& operator=(CreationCounter&& obj)noexcept { ++moveAssigned; return *this; }
    ~CreationCounter() = default;
    void printAll() const
    {
        std::cout << "Copy construct/assigned: " << copyConstructed << " / " << copyAssigned << '\t' << "Move construct/assigned: " << moveConstructed << " / " << moveAssigned << '\n';
    }
};