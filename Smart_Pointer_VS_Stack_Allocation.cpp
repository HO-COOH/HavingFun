#include <iostream>
#include <memory>
#include <functional>

/* We have two classes*/
class Base
{
public:
    virtual void func()
    {
        std::cout<<"Func in base!\n";
    }
};
//Derived class is inherited from Base
class Derived:public Base
{
public:
    void func() override
    {
        std::cout<<"Func in derived!\n";
    }
};

/*Some functions accepting a raw pointer or smart pointers of Base class and call func() memberfunction */
void Process(Base* basePointer)
{
    basePointer->func();
}

//deleter template for unique_ptr
template<typename T>
struct deleter_do_nothing
{
    void operator()(T* p){}
};

int main()
{
    Base base_stack_obj;            //a stack allocated base object
    Derived derived_stack_obj;      //a stack allocated derived object
    Process(&derived_stack_obj);    //call Derived::func()

    //Use a smart pointer pointing a stack-allocated object
    std::shared_ptr<Base> derived_stack_obj_ptr{&derived_stack_obj, [](Base*){}};   //manual deleter, doing nothing, so it won't delete the stack object
    Process(derived_stack_obj_ptr.get());   //call Derived::func()

    std::unique_ptr<Base, deleter_do_nothing<Base>> derived_stack_obj_ptr2{&derived_stack_obj};    //unique_ptr need a template type of a deleter
    Process(derived_stack_obj_ptr2.get()); //call Derived::func()

    std::weak_ptr<Base> derived_stack_obj_ptr3=derived_stack_obj_ptr;
    std::cout<<derived_stack_obj_ptr3.use_count()<<"\n";    //1
    Process(derived_stack_obj_ptr3.lock().get());           //call Derived::func()
}