#pragma once
#include "Shape.h"
typedef struct triangle_impl_struct triangle_impl;
typedef struct triangle_vtable_struct triangle_vtable;

typedef struct triangle
{
    triangle_impl* triangle_impl_ptr;
    triangle_vtable* methods;
}Triangle;

struct triangle_vtable_struct
{
    shape_vtable shape_methods; //Inherited from Shape class
    //... Add other function pointers as needed
    void (*rotate)(Triangle *); //This is the unique method of Triangle class
};

Triangle new_Triangle(int sideLength);
void delete_Triangle(Triangle *triangle);