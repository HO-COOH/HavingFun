/*Use "-fno-elide-constructors" flag*/
#include <iostream>
#include <vector>
#include <array>

using namespace std;

/*We have a class that doesn't have a move constructor!*/
class NoMove
{
private:
    int data;
public:
    NoMove(int value):data(value){cout<<"Explicit constructor!\n";}
    ~NoMove() {cout<<"Destructor!\n";}
    NoMove(const NoMove& a):data(a.data) {cout<<"Copy constructor!\n";}
    NoMove& operator=(const NoMove& a){data=a.data; cout<<"Copy = assignment!\n"; return *this;}
    void dostuff(){++data;}
};

/*We have another class that has a move constructor!*/
class HasMove
{
private:
    int data;
public:
    HasMove(int value):data(value){cout<<"Explicit constructor!\n";}
    ~HasMove() {cout<<"Destructor!\n";}
    HasMove(const HasMove& a):data(a.data) {cout<<"Copy constructor!\n";}
    HasMove& operator=(const HasMove& a){data=a.data; cout<<"Copy = assignment!\n"; return *this;}
    void dostuff(){++data;}

    HasMove(HasMove&& a) noexcept:data(a.data){cout<<"Move constructor!\n";}                     //The important move constructor
    HasMove& operator=(HasMove&& a) noexcept{data=a.data; cout<<"Move = assignment!\n"; return *this;}        //The important move assignment
};

NoMove Return_Temp_NoMove()
{
    NoMove temp{0};
    temp.dostuff();
    return temp;
}

void Test_NoMove()
{
    //NoMove obj_1{1};    //call explicit constructor
    NoMove obj_2{Return_Temp_NoMove()}; //call 1.explicit constructor   2.copy temp -> unamed temporary     3. copy unmaed temporary -> obj_2
}

vector<NoMove> Return_Temp_Vec_NoMove()
{
    vector<NoMove> vec{1,2,3,4,5};
    cout<<"----Finish constructing NoMove----\n";
    return vec;
}

void Test_Vector_NoMove()
{
    vector<NoMove> vec=Return_Temp_Vec_NoMove();
}


//------------------------------------------------------------------------------------------------------------------------------------------------
HasMove Return_Temp_HasMove()
{
    HasMove temp{0};
    temp.dostuff();
    return temp;
}

void Test_HasMove()
{
    HasMove obj_2{Return_Temp_HasMove()};//call 1. explicit constructor  2.move temp -> unamed temporary    3. move unmaed temporary -> obj_2
}

vector<HasMove> Return_Temp_Vec_HasMove()
{
    vector<HasMove> vec{1,2,3,4,5};
    cout<<"----Finish constructing HasMove----\n";
    return vec;
}

void Test_Vector_HasMove()
{
    vector<HasMove> vec=Return_Temp_Vec_HasMove();
}


int main()
{
    cout<<"------------------------------------------Testing NoMove------------------------------------------\n";
    //Test_Vector_NoMove();
    //array<NoMove, 5> arr_NoMove{1,2,3,4,5};
    //vector<NoMove> vec_NoMove{1,2,3,4,5};
    NoMove carr_NoMove[5]={1,2,3,4,5};
    cout<<"------------------------------------------Testing HasMove-----------------------------------------\n";
    //Test_Vector_HasMove();
    //array<HasMove, 5> arr_HasMove{1,2,3,4,5};
    //vector<HasMove> vec_HasMove{1,2,3,4,5};
    HasMove carr_HasMove[5]={1,2,3,4,5};
}