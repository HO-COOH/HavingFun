#include "Shape.h"
#include "Square.h"
#include "Triangle.h"
void processShape(Shape* shape)
{
    switch(shape->methods->get_type(shape))
    {
    case Square_type:
        {
            puts("Processing a square: ");
            break;
        }
    case Triangle_type:
        {
            puts("Processing a triangle: ");
            break;
        }
    }
    puts("Finish processing!");
}