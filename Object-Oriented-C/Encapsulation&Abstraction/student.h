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
