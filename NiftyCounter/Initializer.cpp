//
// Created by peter on 6/23/20.
//

#include "Initializer.h"
#include <new>
static char singletonStorage[sizeof(MyClass)];
MyClass& singleton= reinterpret_cast<MyClass&>(singletonStorage);
unsigned Initializer::count;
void Initializer::init()
{
    new(&singleton) MyClass{};
}

void Initializer::cleanUp()
{
    singleton.~MyClass();
}

Initializer::Initializer()
{
    if (count++ == 0)
        init();
}

Initializer::~Initializer()
{
    if (--count == 0)
        cleanUp();
}
