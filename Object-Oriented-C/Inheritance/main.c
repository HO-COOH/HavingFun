#include "Square.h"
#include "Triangle.h"
#include <stdio.h>

int main() 
{
    Square square = new_Sqaure(4);
    puts("Drawing a square");
    square.methods->shape_methods.draw(&square);
    square.methods->shape_methods.print_info(&square);
    delete_Sqaure(&square);

    Triangle triangle = new_Triangle(5);
    puts("Drawing a triangle");
    triangle.methods->shape_methods.draw(&triangle);    
    triangle.methods->rotate(&triangle);
    triangle.methods->shape_methods.draw(&triangle);
    triangle.methods->shape_methods.print_info(&triangle);
    delete_Triangle(&triangle);
}