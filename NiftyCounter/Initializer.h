//
// Created by peter on 6/23/20.
//

#ifndef NIFTYCOUNTER_INITIALIZER_H
#define NIFTYCOUNTER_INITIALIZER_H

#include "MySingleton.h"
extern MyClass& singleton;
class Initializer
{
    static unsigned count;
    static void init();
    static void cleanUp();

public:
    Initializer();
    ~Initializer();
};
static Initializer init;

#endif //NIFTYCOUNTER_INITIALIZER_H
