#include "Square.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
struct square_impl_struct   //private part in Square class
{
    int circumference;
};

static square_vtable global_vtable;
static bool vtable_init = false;
static void init_global_vtable();

//... Other Square functions
static void draw(Square* square)
{
    const int oneSide = square->square_impl_ptr->circumference / 4;

    //draw top
    for (int i = 0; i < oneSide; ++i)
        putchar('-');
    putchar('\n');

    //draw left and right
    for (int i = 0; i < oneSide; ++i)
    {
        putchar('|');
        for (int j = 0; j < oneSide-1; ++j)
            putchar(' ');
        putchar('|');
        putchar('\n');
    }

    //draw bottom
    for (int i = 0; i < oneSide; ++i)
        putchar('-');

    putchar('\n');
}

static double getArea(Square* square)
{
    return pow(square->square_impl_ptr->circumference/4.0, 2);
}

static double getCircumference(Square* square)
{
    return square->square_impl_ptr->circumference;
}

static Shape new_Shape()    //define new_Shape here, so it is not visible to client code
{
    Shape shape;
    shape.shape_impl_ptr = malloc(sizeof(square_impl));
    shape.methods = &global_vtable.shape_methods;
    return shape;
}

Square new_Sqaure(int sideLength)
{    
    if(vtable_init==false)
        init_global_vtable();
    Shape temp_shape = new_Shape(); //call Base class constructor
    ((square_impl *)temp_shape.shape_impl_ptr)->circumference = sideLength * 4;

    Square temp_square;
    temp_square.square_impl_ptr = temp_shape.shape_impl_ptr;
    temp_square.methods = temp_shape.methods;
    return temp_square;
}

void delete_Sqaure(Square* square)
{
    if(square!=NULL)
    {
        if(square->square_impl_ptr!=NULL)
            free(square->square_impl_ptr);
    }
}

static void init_global_vtable()
{
    global_vtable.shape_methods.draw = &draw;
    global_vtable.shape_methods.getArea = &getArea;
    global_vtable.shape_methods.getCircumference = &getCircumference;
    global_vtable.shape_methods.print_info = &print_info;
    vtable_init = true;
}