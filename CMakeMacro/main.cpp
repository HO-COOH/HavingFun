#include <iostream>

int main(int, char**) 
{
    #ifdef MODE=Debug
    std::cout << "DEBUG" << std::endl;
    #endif // DEBUG
    std::cout<<"Release"<<std::endl;
}
