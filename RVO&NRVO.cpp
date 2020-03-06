#include <algorithm>
#include <cstdio>
int i = 0;
class MyClass
{
public:
    MyClass(){puts("Object constructed!");}
    ~MyClass(){puts("Object destroyed!");}
    MyClass(MyClass const& obj){puts("Copy constructor!");}
    MyClass(MyClass&& obj)noexcept{puts("Move constructor!");}
    MyClass& operator=(MyClass const& obj){puts("=Copy");return *this;}
    MyClass& operator=(MyClass&& obj)noexcept{puts("=Move");return *this;}
    void dostuff() const { puts("Doing stuff in MyClass"); }
};
struct Nested
{
    MyClass inner;
};
class NestedPrivate
{
    MyClass m_obj;
public:
    MyClass const& getNested() const& { return m_obj; }
};

MyClass return_move_const()
{
    MyClass const a;        //constructed
    return std::move(a);    //cannot move from const, so copy constructed
}
MyClass return_move()
{
    MyClass b;              //constructed
    return std::move(b);    //move constructed
}
MyClass return_named()
{
    MyClass c;  //in-place constructed
    return c;
}
MyClass return_const_named()
{
    MyClass const d;//in-place constructed
    return d;
}
MyClass return_const_named_with_condition(bool condition)
{
    if(condition)
    {
        MyClass const e;
        return e;
    }
    else
    {
        MyClass const e;
        ++i;
        return e;
    }
}
MyClass return_named_with_condition(bool condition)
{
    if (condition)
    {
        MyClass f;
        return f;
    }
    else
    {
        MyClass f;
        ++i;
        return f;
    }
}
MyClass return_from_nested()
{
    Nested g;
    return g.inner;
}
MyClass return_move_from_nested()
{
    Nested h;
    return std::move(h.inner);
}

void test_return()
{
    auto obj = return_move_const(); //copy elision     -> 1 default construct, 1 copy construct
    auto obj2 = return_move();  //copy elision    -> 1 default construct, 1 move construct
    auto obj3 = return_named();//copy elision -> 1 construct
    auto obj4 = return_const_named();//same as return_named()
    auto obj5 = return_const_named_with_condition(true);  //MSVC debug, GCC -> 1 default construct + 1 copy construct MSVC release, Clang -> copy elision: 1 default construct
        
    auto obj6 = return_named_with_condition(true);  //MSVC debug, GCC -> 1 default construct + 1 move construct  MSVC release, Clang -> copy elision: 1 default construct
    auto obj7 = return_from_nested();   //copy elision -> 1 default construct, 1 copy construct
    auto obj8 = return_move_from_nested();  //copy elision -> 1 default construct, 1 move construct
}


MyClass const& return_ref(MyClass const& obj)
{
    return obj; //always dangling
}
decltype(auto) return_forwarding()
{
    return MyClass{};
}
decltype(auto) return_named_by_ref()
{
    MyClass a;
    return (a);
}

void test_lifetime()
{
    /*A temporary bound to a reference parameter in a function call (5.2.2) persists until the completion of the full expression containing the call */
    /*A temporary bound to the returned value in a function return statement (6.6.3) persists until the function exits */
    //auto const& obj1 = return_ref(Nested{}.inner);  //constructed and then destroied at this line
    //obj1.dostuff(); //dangling reference
    //auto const& obj2 = return_ref(MyClass{});       //constructed and then destroied at this line
    //obj2.dostuff(); //dangling reference
    
    //auto const& obj3 = return_forwarding();
    //obj3.dostuff(); //OK

    auto const& obj4 = return_named_by_ref();
    obj4.dostuff();
}

int main()
{
    test_lifetime();
}