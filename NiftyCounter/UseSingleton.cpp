//
// Created by peter on 6/23/20.
//
#include <iostream>
#include "UseSingleton.h"
#include "Initializer.h"
void use()
{
    singleton.dostuff();
    std::cout<<"Address: "<<&singleton<<'\n';
}
