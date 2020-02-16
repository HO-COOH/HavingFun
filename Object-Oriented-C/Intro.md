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


/*student.cpp*/
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