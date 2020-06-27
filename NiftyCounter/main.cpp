#include <iostream>
#include "UseSingleton.h"
#include "Initializer.h"
int main()
{
    use();
    std::cout<<"In main, address: "<<&singleton<<'\n';
}
