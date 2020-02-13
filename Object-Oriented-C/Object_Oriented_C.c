/*Description: Trying to simulate object-oriented programming in C
*Introduction:
*   Object-oriented programming is a programming paradigm based on the concepts of "objects", which can contain:
*       data(fields, attributes, properties)
*       code(procedures, methods)
*   4 main features of OOP are:
*       encapsulation
*       abstraction
*       inheritance
*       polymorphism
*/
/*In this .c file, I demonstrate how to simulate the first 2 features using C*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _Year{freshman, sophomore, junior, senior} Year;

typedef struct _Student
{
    size_t id;
    char name[30];
    Year year;
    void (*report)(Student*);
}Student;

void Report(Student* student)
{
    Student self=*student;
    printf("%d\n", self.id);
    printf("%s\n", self.name);
    switch (self.year)
    {
    case freshman:
        /* code */
        break;
    
    default:
        break;
    }
}

Student* new_Student(size_t id, char* name, Year year)
{
    Student* new_s=malloc(sizeof(Student));
    new_s->id=id;
    strcpy(new_s->name, name);
    new_s->year=year;
    return new_s;
}



int main()
{

}
