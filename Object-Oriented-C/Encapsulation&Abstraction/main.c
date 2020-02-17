#include "student.h"
#include <stdio.h>

int main()
{
    Student me = new_Student("MKBHD", 20, Freshmen);
    /*
    add_grade(&me, 3, 3.3);
    print_student_record(&me);*/
    me.methods->add_grade(&me, 3, 3.3);
    me.methods->print_student_record(&me);
    delete_Student(&me);
    puts("Finished!");
}