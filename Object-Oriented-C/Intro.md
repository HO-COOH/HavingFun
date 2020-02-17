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

/*student.cpp*/
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

Student_vtable global_vtable;
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

