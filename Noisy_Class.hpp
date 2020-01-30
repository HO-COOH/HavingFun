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
    }
    MyClass& operator=(MyClass&& obj)noexcept
    {
        puts("=Move");
    }
};