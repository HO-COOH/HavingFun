#include "cstring.h"
#define LOG_ERROR(X) fprintf(stderr, X)

void TEST_DEFINITION()
{
    puts(__FUNCTION__);
    printf("CString size: %d, \
     Union(CString_impl size: %d, Small_string size: %d \
     *vtable size: %d\
     bool size: %d\n",
           sizeof(CString), sizeof(CString_impl), SMALL_STRING, sizeof(CString_public*), sizeof(bool));
}

void TEST_NEW()
{
    puts(__FUNCTION__);
    CString test = new_CString("Hello world");
    test.public->puts(&test);
    delete_CString(&test);
}

void TEST_SERIALIZE()
{
    puts(__FUNCTION__);
    CString test = new_CString("Hello world");
    test.public->puts(&test);
    test.public->serialize_to(&test, "serialize_test.txt", true);

    CString test2 = CString_deserialize_from("serialize_test.txt");
    test2.public->puts(&test2);

    delete_CString(&test);
    delete_CString(&test2);
}

void TEST_TOGGLE_LETTER()
{
    puts(__FUNCTION__);
    CString test1 = new_CString("a ShORt sTRing");
    CString test2 = new_CString("A LONger sTRINg fOR TogGliNg LEtTeR tEst");
    puts("----------Original-----------");
    test1.public->puts(&test1);
    test2.public->puts(&test2);

    puts("---------After--------------");
    test1.public->to_lower(&test1);
    test2.public->to_upper(&test2);
    test1.public->puts(&test1);
    test2.public->puts(&test2);

    delete_CString(&test1);
    delete_CString(&test2);
}

void TEST_LINE_TO_CSTRING()
{
    puts(__FUNCTION__);
    CString test = line_to_CString_from("line_test.txt");
    test.public->puts(&test);

    delete_CString(&test);
}

void TEST_EMPTY()
{
    puts(__FUNCTION__);
    CString test1 = empty_CString();
    CString test2 = new_CString("");
    CString test3 = new_CString("Not empty");

    puts(test1.public->empty(&test1) ? "true" : "false");
    puts(test2.public->empty(&test2) ? "true" : "false");
    puts(test3.public->empty(&test3) ? "true" : "false");

    delete_CString(&test1);
    delete_CString(&test2);
    delete_CString(&test3);
}

void TEST_MOVE()
{
    puts(__FUNCTION__ );
    char* src="This is the source";
    CString test=move_to_CString(src);

    print_info(&test);
    delete_CString(&test);
}

void TEST_APPEND_CHAR()
{
    puts(__FUNCTION__ );
    CString test1=new_CString("A string to append a char");
    test1.public->puts(&test1);
    print_info(&test1);
    char c='!';

    test1.public->append_char(&test1, c);
    test1.public->puts(&test1);
    print_info(&test1);
    delete_CString(&test1);
}

void TEST_REVERSE()
{
    puts(__FUNCTION__ );
    CString test1=new_CString("A string to be reversed");
    test1.public->reverse(&test1);

    print_info(&test1);
    delete_CString(&test1);
}

void TEST_GET_UNTIL()
{
    puts(__FUNCTION__ );
    CString test=get_until(' ');

    print_info(&test);
    delete_CString(&test);
}

//void to_string(void* src, TYPE_ENUM type)
//{
//    char buffer[20];
//    switch (type)
//    {
//    case int_Type:
//        sprintf(buffer, "%d", *(int*)src);
//        break;
//    case float_Type:
//    case double_Type:
//    case unsigned_int_Type:
//    case short_Type:
//        ;
//    }
//    puts(buffer);
//}

void TEST_APPEND_INT()
{
    puts(__FUNCTION__ );

}

int main()
{
    CString str=new_CString("hello world");
    str.public.
}