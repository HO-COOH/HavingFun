/*student.cpp*/
#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct student_impl_struct
{
    char* name;
    int age;
    Year year;
    int credit_hours;
    float GPA;
};
//Function implementations...
//...
Student new_Student(char* name, int age, Year year)
{
    Student student;
    student.student_impl_ptr = malloc(sizeof(student_impl));
    if(name==NULL)
    {
        student.student_impl_ptr->name = malloc(1);
        student.student_impl_ptr->name[0] = '\0';
    }
    else
    {
        student.student_impl_ptr->name = malloc(strlen(name)+1);
        strcpy(student.student_impl_ptr->name, name);
    }
    student.student_impl_ptr->age = age;
    student.student_impl_ptr->year = year;
    student.student_impl_ptr->credit_hours = 0;
    student.student_impl_ptr->GPA = 4.0;
    return student;
}

void add_grade(Student* student, int credit, float grade_point)
{
    const float orgin_total = student->student_impl_ptr->credit_hours * student->student_impl_ptr->GPA;
    student->student_impl_ptr->credit_hours += credit;
    student->student_impl_ptr->GPA = (orgin_total + credit * grade_point) / (float)(student->student_impl_ptr->credit_hours);
}

int get_age(Student const *student)
{
    return student->student_impl_ptr->age;
}

Year get_year(Student const *student)
{
    return student->student_impl_ptr->year;
}

int get_credit_hours(Student const *student)
{
    return student->student_impl_ptr->credit_hours;
}

float get_GPA(Student const *student)
{
    return student->student_impl_ptr->GPA;
}

static void _print_age(Year year)
{
    putc('\t', stdout);
    switch (year)
    {
    case Freshmen:
        puts("Freshmen");
        break;
    case Sophomore:
        puts("Sophomore");
        break;
    case Junior:
        puts("Junior");
        break;
    case Senior:
        puts("Senior");
        break;
    }
}

void print_student_record(Student const *student)
{
    printf("Student: %s\n", student->student_impl_ptr->name);
    printf("\tage: %d\n", student->student_impl_ptr->age);
    _print_age(student->student_impl_ptr->year);
    printf("\tCredit hours: %d \tGPA: %f\n", student->student_impl_ptr->credit_hours, student->student_impl_ptr->GPA);
}

void delete_Student(Student const *student)
{
    if(student!=NULL)
    {
        if(student->student_impl_ptr!=NULL)
        {
            free(student->student_impl_ptr->name);
            free(student->student_impl_ptr);
        }
    }
}
