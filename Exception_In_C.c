/*Let's simulate a similar exception mechanism seen in many other programming languages in C*/
#include <stdlib.h> //for malloc()
#include <string.h> //for string functions
#include <stdio.h>  //for puts()

typedef struct exception {
    char* what; //add more stuff if needed
}Exception;

typedef struct shouldReturnInt{
    int value;
    Exception* exception;
}ShouldReturnInt;

ShouldReturnInt func()
{
    //do stuff...
    
    //if error
    ShouldReturnInt temp;
    temp.exception = malloc(sizeof(Exception));
    temp.exception->what = malloc(strlen("some exception in func()") + 1);  //don't forget the additional byte for '\0'
    strcpy(temp.exception->what, "some exception in func()");
    return temp;
    
    //if good
    ShouldReturnInt temp2;
    temp2.value = 0;
    temp2.exception = NULL;
    return temp2;
}

int main()
{
    ShouldReturnInt result=func();      //similar to try{}
    if(result.exception!=NULL)          //similar to catch{}
    {/*...*/}  //don't forget to: free(result.exception->what); free(result.exception)
}