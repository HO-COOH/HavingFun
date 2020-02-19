# Description: Trying to simulate object-oriented programming in C
## Introduction:
Object-oriented programming is a programming paradigm based on the concepts of "objects", which can contain:
- data(fields, attributes, properties)
- code(procedures, methods)
## 4 main features of OOP are:
- encapsulation
- abstraction
- inheritance
- polymorphism

## Encapsulation
From Wikipedia: https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)
> In object oriented programming languages, encapsulation refers to one of two related but distinct notions, and sometimes to the combination thereof:[3][4] <br> 
> - A language mechanism for restricting direct access to some of the object's components.[5][6] <br>
> - A language construct that facilitates the bundling of data with the methods (or other functions) operating on that data.[1][7]

In C, we only have ``struct``. ``struct`` is ``public``, and all of the data can be accessed. To achieve the 1st point with ``struct``, we need to figure out a way to block from direct access to the data member. We have two ways of doing it:  
1. Define a *public* ``struct``, which contains only a pointer to a ``impl_struct``. The public ``struct`` is defined in a **header file**, so that we can use this struct in the client code. The ``impl_struct`` is defined in a **source file**, so that it is invisible to client code. Any operation to access the data is done by a function.  
Let's demonstrate with a student *class*.
```c
/*student.h*/
typedef struct student_impl_struct student_impl;
typedef struct student_struct
{
    student_impl* student_impl_ptr;
}Student;
typedef enum year_enum{Freshmen, Sophomore, Junior, Senior} Year;
//Functions
Student new_Student(char* name, int age, Year year);

void add_grade(Student* student, int credit, float grade_point);

int get_age(Student const *student);
Year get_year(Student const *student);
int get_credit_hours(Student const *student);
float get_GPA(Student const *student);
void print_student_record(Student const *student);

void delete_Student(Student const *student);


/*student.c*/
#include "student.h"
struct student_impl_struct
{
    char* name;
    int age;
    enum Year year;
    int credit_hours;
    float GPA;
};
//Function implementations...
//...
```
## Abstraction
From Wikipedia: https://en.wikipedia.org/wiki/Abstraction_principle_(computer_programming)
>In software engineering and programming language theory, the abstraction principle (or the principle of abstraction) is a basic dictum that aims to reduce duplication of information in a program (usually with emphasis on code duplication) whenever practical by making use of abstractions provided by the programming language or software libraries[citation needed].  
>When read as recommendation to the programmer, the abstraction principle can be generalized as the "don't repeat yourself" principle, which recommends avoiding the duplication of information in general, and also avoiding the duplication of human effort involved in the software development process.

This means we need to achieve code reuse for different object and parts in our code. Followed with [Encapsulation](#encapsulation), we want to reuse our code which operates with ``Student`` to other objects(maybe later in inheritance).
Because of the convention in C, that we put function declarations in a ``.h`` file and definitions in ``.c`` file, we will have to figure out a way to let other part of the code to be able to use the ``Student`` functions, whiling keeping ``student.h`` a encapsulated package. We can put all the functions inside a global ``struct``, and each instance of ``Student`` keeps a pointer pointing to these functions. So that later we want to use these funcitons, we simply access them using the pointer, instead of individual functions.   
We can improve the ``student`` class as follows:

```c
/*student.h*/
typedef struct student_impl_struct student_impl;
typedef struct student_vtable Student_vtable;

typedef struct student_struct
{
    student_impl* student_impl_ptr;
    Student_vtable *methods;
}Student;
typedef enum year_enum{Freshmen, Sophomore, Junior, Senior} Year;
//Functions struct
struct student_vtable{
    void (*add_grade)(Student *, int, float);
    int (*get_age)(Student const *);
    Year (*get_year)(Student const *);
    int (*get_credit_hours)(Student const *);
    float (*get_GPA)(Student const *);
    void (*print_student_record)(Student const *);
};
Student new_Student(char* name, int age, Year year);
void delete_Student(Student const *student);

/*student.c*/
#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
struct student_impl_struct
{
    char* name;
    int age;
    Year year;
    int credit_hours;
    float GPA;
};

static Student_vtable global_vtable;    //so only visible to this file
static bool vtable_init = false;
static void init_global_vtable();
//Function definitions
//...
static void init_global_vtable()
{
    global_vtable.add_grade = &add_grade;
    global_vtable.get_age = &get_age;
    global_vtable.get_credit_hours = &get_credit_hours;
    global_vtable.get_GPA = &get_GPA;
    global_vtable.get_year = &get_year;
    global_vtable.print_student_record = &print_student_record;
    vtable_init = true;
}
```

## Inheritance
From Wikipedia: https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)
>In object-oriented programming, inheritance is the mechanism of basing an object or class upon another object (prototype-based inheritance) or class (class-based inheritance), retaining similar implementation. Also defined as deriving new classes (sub classes) from existing ones (super class or base class) and forming them into a hierarchy of classes. <br>
>In most class-based object-oriented languages, an object created through inheritance (a "child object") acquires all the properties and behaviors of the parent object (except: constructors, destructor, overloaded operators and friend functions of the base class).   

In C, we can achieve inheritance by calling corresponding functions defined in base class in derived class's implementation.  
However, all the interface needs to be redefined in derived class. As usual convention, we put different **class** into different source files. And we can even control a particular class to be **abstract** by not declaring a ``new`` or ``delete`` function inside that corresponding ``.h`` file.   
**Note:** There will be function redefined problems if multiple functions using the same name. For example, ``draw()`` is defined in both ``"Sqaure.c"`` and ``"Triangle.c"``, so it needs to be defined as ``static``. Same with ``print_info()`` in ``"Shape.h"``.
Here we use ``struct Shape`` as an example to demonstrate inheritance.
```c
/*Shape.h*/
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
    double (*getCircumference)(Shape*); //These functions should be defined in derived "Shape"
    void (*print_info)(Shape *);        //We don't want to change the behavior of this function, so it is defined in this file
};
static void print_info(Shape* shape)    //A method that is inherited by all derived "Shape", and they don't modifiy this function
{
    printf("This polygon has C=%f, S=%f\n", shape->methods->getCircumference(shape), shape->methods->getArea(shape));
}
//Shape is an abstract class so no new and delete functions here


/*Square.h*/
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

/*Square.c*/
#include "Square.h"
struct square_impl_struct   //private part in Square class
{
    int circumference;
};
static square_vtable global_vtable;
static bool vtable_init = false;
static void init_global_vtable();
//... Other Square functions
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
    //...
}
static void init_global_vtable()
{
    global_vtable.shape_methods.draw = &draw;
    global_vtable.shape_methods.getArea = &getArea;
    global_vtable.shape_methods.getCircumference = &getCircumference;
    global_vtable.shape_methods.print_info = &print_info;
    vtable_init = true;
}


/*Triangle.h*/
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

/*Triangle.c*/
//... Same as Square.h
static void rotate(Triangle* triangle)
{
    triangle->triangle_impl_ptr->rotate = !triangle->triangle_impl_ptr->rotate;
}
static void init_global_vtable()
{
    //... Init other function pointers
    global_vtable.rotate = &rotate; //The unique function for Triangle class
    //...
}
```  
## Polymorphism
