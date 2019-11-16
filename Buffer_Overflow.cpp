#include <iostream>
#include <cstring>
int main()
{
    using std::cout;
    char a[20]="My original";
    char b[10];
    cout<<(void*)&a[0]<<' '<<(void*)&a[19]<<'\n'<<(void*)&b[0]<<' '<<(void*)&b[9]<<"\n";
    strcpy(b,"A string overflowing b[] to a[]");
    cout<<"string b: "<<b<<"\nstring a: "<<a<<"\n";
}

//Memory layout:
//b[0]  ...  b[9]a[0]  ...  a[19]
//^low        ->            high^

//A possible output

// 0x23fe30 0x23fe3b
// 0x23fe26 0x23fe2f
// string b: A string overflowing b[] to a[]
// string a: verflowing b[] to a[]