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
        temp.impl=NULL;
        temp.is_small=true;
        //TODO: init functions
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
        CString temp;
        fread(&temp, sizeof(temp), 1, f);
        if(temp.is_small)
            return temp;
        else
        {
            temp.impl=malloc(sizeof(CString_impl));
            fread(&temp.impl, sizeof(CString_impl), 1, f);
            temp.impl->data=malloc(temp.impl->capacity);
            fread(temp.impl->data, 1, temp.impl->capacity, f);
            return temp;
        }
    }
    return new_CString(NULL);
}
CString line_to_cstring(FILE *f)
{
    if(f!=NULL)
    {
        FILE* start=f;
        size_t count=0;
        while(fgetc(f)!='\n')
            ++count;
        CString temp;
        if(count<=SMALL_STRING)
        {
            temp.is_small=true;
            fread(temp.small_string, 1, count, start);
            temp.impl=NULL;
            //TODO: init functions
            return temp;
        }
        temp.is_small=false;
        temp.impl=malloc(sizeof(CString_impl));
        temp.impl->capacity=count+1;
        temp.impl->length=count;
        temp.impl->data=malloc(count+1);
        fread(temp.impl->data, 1, count, start);
        return temp;
    }
    return new_CString(NULL);
}

/*Conversions to CString*/
CString to_cstring(void *data, TYPE_ENUM type)
{
    switch(type)
    {
    case int_Type:
    {
        int digits = 0;
        int original = *(int *) (data);
        while ((original/=10)>1)
            ++digits;
        char num[digits];
        while(--digits)

        if (digits >= SMALL_STRING) //BIG String
        {
                for ()
        }
        {


        }
    }
    case float_Type:
    case double_Type:
    case unsigned_int_Type:
    case short_Type:
    }
}