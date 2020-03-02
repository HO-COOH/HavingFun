#include "cstring.h"
#include <string.h>
#include <stdlib.h>
struct cstring_impl_t
{
    char* data;
    size_t length;
    size_t capacity;
};
/*Constructor*/
CString new_CString(char *data)
{
    CString temp;
    if(data!=NULL)
    {
        if (strlen(data)<=SMALL_STRING-1)
        {
            strcpy(temp.small_string, data);
            temp.is_small=true;
        }
        else
        {
            temp.impl=malloc(sizeof(CString_impl));
            temp.impl->length=strlen(data);
            temp.impl->capacity=temp.impl->length+1;
            temp.impl->data=malloc(temp.impl->capacity);
            strcpy(temp.impl->data, data);
            temp.is_small=false;
        }
    }
    else
    {
        temp.small_string[0]='\0';
        temp.is_small=true;
    }
    return temp;
}

/*Destructor*/
void delete_CString(CString *str)
{
    if(str!=NULL && str->is_small==false)
    {
        if(str->impl!=NULL)
        {
            free(str->impl->data);
            free(str->impl);
        }
    }
}

/*Deserialize*/
CString CString_deserialize(FILE *f)
{
    if(f!=NULL)
    {

    }
}
CString line_to_cstring(FILE *f)
{

}

/*Conversions to CString*/
CString to_cstring(void *data, TYPE_ENUM type)
{

}