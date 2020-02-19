#pragma once
#include <stdio.h>
typedef struct shape_impl_struct shape_impl;        //as above, define an impl struct, but it has no definition
typedef struct shape_vtable_struct shape_vtable;    //as above, define the vtable(function pointers struct)

typedef struct shape
{
    shape_impl* shape_impl_ptr;
    shape_vtable* methods;
}Shape;

struct shape_vtable_struct  //like interface, all derived "Shape" should support these operations
{
    void (*draw)(Shape*);
    double (*getArea)(Shape*);
    double (*getCircumference)(Shape*);
    void (*print_info)(Shape *);
};

static void print_info(Shape* shape)    //A method that is inherited by all derived "Shape"
{
    printf("This polygon has C=%f, S=%f\n", shape->methods->getCircumference(shape), shape->methods->getArea(shape));
}

//Shape is an abstract class so no new and delete functions here