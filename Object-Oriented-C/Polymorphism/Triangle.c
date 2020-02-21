#include "Triangle.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
struct triangle_impl_struct
{
    int circumference;
    bool rotate;
};

static triangle_vtable global_vtable;
static bool vtable_init = false;
static void init_global_vtable();

/*
  *
 * *
* * *

*/
static void draw(Triangle* triangle)
{
    const int oneSide = triangle->triangle_impl_ptr->circumference / 3;
    if(triangle->triangle_impl_ptr->rotate)
    {
        for (int i = oneSide-1; i >= 0; --i)
        {
            for (int space = 0; space < oneSide - 1 - i; ++space)
                putchar(' ');
            for (int star = 0; star < i + 1; ++star)
            {
                putchar('*');
                putchar(' ');
            }
            putchar('\n');
        }
    }
    else
    {
        for (int i = 0; i < oneSide; ++i)
        {
            for (int space = 0; space < oneSide - 1 - i; ++space)
                putchar(' ');
            for (int star = 0; star < i + 1; ++star)
            {
                putchar('*');
                putchar(' ');
            }
            putchar('\n');
        }
    }
    putchar('\n');
}

static double getArea(Triangle* triangle)
{
    return pow(triangle->triangle_impl_ptr->circumference / 3, 2) * sqrt(3) / 4.0;
}

static double getCircumference(Triangle* triangle)
{
    return triangle->triangle_impl_ptr->circumference;
}

static void rotate(Triangle* triangle)
{
    //triangle->triangle_impl_ptr->rotate = !triangle->triangle_impl_ptr->rotate;
    //a more concise way
    triangle->triangle_impl_ptr->rotate ^= 1;
}

static Shape_type get_type(Triangle* triangle)
{
    return Triangle_type;
}

static Shape new_Shape()
{
    Shape shape;
    shape.shape_impl_ptr = malloc(sizeof(triangle_impl));
    shape.methods = &global_vtable.shape_methods;
    return shape;
}

Triangle new_Triangle(int sideLength)
{    
    if(vtable_init==false)
        init_global_vtable();
    Shape temp_shape = new_Shape();
    ((triangle_impl *)temp_shape.shape_impl_ptr)->circumference = sideLength * 3;

    Triangle temp_triangle;
    temp_triangle.triangle_impl_ptr = temp_shape.shape_impl_ptr;
    temp_triangle.methods = temp_shape.methods;
    temp_triangle.triangle_impl_ptr->rotate = false;
    return temp_triangle;
}

void delete_Triangle(Triangle* triangle)
{
    if(triangle!=NULL)
    {
        if(triangle->triangle_impl_ptr!=NULL)
            free(triangle->triangle_impl_ptr);
    }
}

static void init_global_vtable()
{
    global_vtable.shape_methods.draw = &draw;
    global_vtable.shape_methods.getArea = &getArea;
    global_vtable.shape_methods.getCircumference = &getCircumference;
    global_vtable.shape_methods.print_info = &print_info;
    global_vtable.rotate = &rotate;
    global_vtable.shape_methods.get_type = &get_type;

    vtable_init = true;
}