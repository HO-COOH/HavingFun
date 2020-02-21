#include "Square.h"
#include "Triangle.h"
#include "Shape.h"
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
    triangle.methods->rotate(&triangle);    //Triangle class only method
    triangle.methods->shape_methods.draw(&triangle);
    triangle.methods->shape_methods.print_info(&triangle);

    processShape(&triangle);    //simulate polymorphism
    processShape(&square);
    delete_Triangle(&triangle);
}
/*Output:
Drawing a square
* * * *
*     *
*     *
*     *
*     *
* * * *
This polygon has C=16.000000, S=16.000000
Drawing a triangle
    *
   * *
  * * *
 * * * *
* * * * *

* * * * *
 * * * *
  * * *
   * *
    *

This polygon has C=15.000000, S=10.825318
Processing a triangle:
Finish processing!
Processing a square:
Finish processing!
*/