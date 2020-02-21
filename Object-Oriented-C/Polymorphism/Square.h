#pragma once
#include "Shape.h"
typedef struct square_impl_struct square_impl;
typedef struct square_vtable_struct square_vtable;

typedef struct square
{
    square_impl* square_impl_ptr;
    square_vtable* methods;
}Square;

struct square_vtable_struct
{
    shape_vtable shape_methods;
    //... Add other function pointers as needed
};

Square new_Sqaure(int sideLength);
void delete_Sqaure(Square *square);