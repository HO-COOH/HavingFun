//
// Created by peter on 6/23/20.
//

#ifndef NIFTYCOUNTER_MYSINGLETON_H
#define NIFTYCOUNTER_MYSINGLETON_H

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

#endif //NIFTYCOUNTER_MYSINGLETON_H
