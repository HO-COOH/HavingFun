#include <stdio.h>
typedef struct A
{
    char const *name;
} A;

void SayHi(A const *a)
{
    printf("Hello, %s", a->name);
}

int main()
{
    A a = {"world"};
    //SayHi(&a);
    a.SayHi();
}