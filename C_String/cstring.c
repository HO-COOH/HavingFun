#include "cstring.h"
#include <string.h>
#include <stdlib.h>
struct cstring_impl_t
{
    char* data;
    size_t length;
    size_t capacity;
};

cstring_public global_vtable;

static bool global_vtable_init=false;
static void init_global_vtable();

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
    }
    if(!global_vtable_init)
        init_global_vtable();
    temp.public=&global_vtable;
    return temp;
}

CString new_CString_by(char* start, char* end)
{
    CString temp;
    if(start!=NULL && end!=NULL)
    {
        //TODO: here
    }
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
            if(!global_vtable_init)
                init_global_vtable();
            temp.public=&global_vtable;
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
        case short_Type:
        case int_Type:
        {
            int digits = 0;
            int original = *(int *) (data);
            while (original /= 10)
                ++digits;
            //12345 -> 4digits, so need to +1
            ++digits;
            original = *(int *) (data);
            char num[digits + 1];
            num[digits] = '\0';   //set last digit to null terminator
            sprintf(num, "%d", original);
            return new_CString(num);
        }
        case unsigned_int_Type:
        {
            int digits = 0;
            unsigned int original = *(unsigned int *) (data);
            while (original /= 10)
                ++digits;
            //12345 -> 4digits, so need to +1
            ++digits;
            original = *(unsigned int *) (data);
            char num[digits + 1];
            num[digits] = '\0';   //set last digit to null terminator
            sprintf(num, "%u", original);
            return new_CString(num);
        }

        case float_Type:
        {
            char num[10];
            float original = *(float *) (data);
            sprintf(num, "%9f", original);
            return new_CString(num);
        }
        case double_Type:
        {
            char num[20];
            double original = *(double *) (data);
            sprintf(num, "%19f", original);
            return new_CString(num);
        }
    }
}

static void assign_string(CString* str, char* data)
{
    if(str!=NULL && data!=NULL)
    {
        size_t data_length=strlen(data);
        if(data_length<=SMALL_STRING-1)
        {
            //if @data is small string but @str is not, free the internal of @str
            if(str->impl!=NULL)
            {
                if (str->impl->data != NULL)
                    free(str->impl->data);
            }
            free(str->impl);
            str->is_small=true;
            strcpy(str->small_string, data);
        }
        else
        {
            //if @data is not small string
            if(str->is_small)
            {
                //if @data is not small string, but @str is, allocate new space
                str->is_small=false;
                str->impl=malloc(sizeof(CString_impl));
                str->impl->data=malloc(data_length+1);
                strcpy(str->impl->data, data);
                str->impl->length=data_length;
                str->impl->capacity=data_length;
            }
            else
            {
                //if neither @data or @str is small string, resign data -> str.impl
                if(str->impl->capacity>=data_length)
                    str->impl->length=data_length;
                else
                {
                    str->impl->data=realloc(str->impl->data, data_length+1);
                    str->impl->capacity=data_length;
                    str->impl->length=data_length;
                }
                strcpy(str->impl->data, data);
            }
        }
    }
}

void assign_cstring(CString* dest, CString* src)
{
    if(dest!=NULL && src!=NULL)
    {
        if(src->is_small)
        {
            if(dest->is_small)
                memcpy(dest->small_string, src->small_string, SMALL_STRING);

        }
    }
}

bool empty(CString* str)
{
    if(str!=NULL)
    {
        if(str->is_small)
            return str->small_string[0]='\0';
        if(str->impl!=NULL)
        {
            if(str->impl->length!=0 && str->impl->data[0]=='\0')
                puts("Error! Str->impl->length=0 but Str->impl->data is not empty!");
            return (str->impl->length==0 && str->impl->data[0]=='\0');
        }
    }
}

size_t length(CString* str)
{
    if(str!=NULL)
    {
        if(str->is_small)
            return strlen(str->small_string);
        return str->impl->length;
    }
}

size_t bytes(CString* str)
{
    if(str!=NULL)
    {
        if(str->is_small)
            return strlen(str->small_string)+1;
        return str->impl->length+1;
    }
}

void resize(CString* str, size_t newSize)
{
    if(str!=NULL)
    {
        if(newSize<=SMALL_STRING-1)
        {
            if(str->is_small)
                return;
            memcpy(str->small_string, str->impl->data, newSize);
            str->small_string[SMALL_STRING-1]='\0';
        }
    }
}

static void toggle_letter(CString* str, bool upper)
{
    if(str!=NULL)
    {
        char* start, *end;
        if(str->is_small)
        {
            start=str->small_string;
            end=str->small_string+SMALL_STRING;
        }
        else
        {
            start=str->impl->data;
            end=str->impl->data+str->impl->length+1;
        }
        while(start!=end)
        {
            if(upper)
            {
                if(*start<='z'&& *start>='a')
                    *start-=('a'-'A');
            }
            else
            {
                if(*start<='Z' && *start>='A')
                    *start+=('a'-'A');
            }
            ++start;
        }
    }
}

static void to_upper(CString* str)
{
    toggle_letter(str, true);
}

static void to_lower(CString* str)
{
    toggle_letter(str, false);
}


static bool reserve_bytes(CString* str, size_t bytes)
{
    if(str!=NULL)
    {
        if(bytes>=SMALL_STRING)
        {
            if(str->is_small)
            {
                str->impl=malloc(sizeof(CString_impl));
                str->impl->data=malloc(sizeof(bytes));
                str->impl->data[0]='\0';
                str->impl->length=0;
                str->impl->capacity=bytes;
            }
            else
            {
                if(str->impl->capacity>=bytes)
                    return true;
                str->impl->data = realloc(str->impl->data, data_length + 1);
                str->impl->capacity=bytes;
            }
            str->is_small=false;
        }
        return true;
    }
    return false;
}

static bool reserve_length(CString* str, size_t length)
{
    return reserve_bytes(str, length+1);
}

static void append_string(CString* dest, char* src)
{
    if(dest!=NULL && src!=NULL)
    {
        size_t appended_bytes=strlen(dest->small_string)+strlen(src)+1;
        if(dest->is_small)
        {
            if(appended_bytes <= SMALL_STRING)
                strcat(dest->small_string, src);
            else
            {
                dest->is_small=false;
                dest->impl=malloc(sizeof(CString_impl));
                dest->impl->data=malloc(appended_bytes);
                strcpy(dest->impl->data, dest->small_string);
                strcat(dest->impl->data, src);
                dest->impl->capacity=appended_bytes-1;
                dest->impl->length=appended_bytes-1;
            }
        }
        else
        {
            if(dest->impl->capacity < dest->impl->length+strlen(src))
            {
                dest->impl->data = realloc(dest->impl->data, appended_bytes);
                dest->impl->capacity=appended_bytes-1;
            }
            strcat(dest->impl->data, src);
            dest->impl->length=appended_bytes-1;
        }
    }
}

static void put_cstring(CString* str)
{
    if(str!=NULL)
    {
        if(str->is_small)
            puts(str->small_string);
        else
            puts(str->impl->data);
    }
}

static char* get_data(CString* str)
{
    return str->is_small? str->small_string : str->impl->data;
}


static bool is_equal(CString* str1, CString* str2)
{
    if((str1==NULL || str2==NULL) || str1->is_small!=str2->is_small)
        return false;
    if(str1->is_small)  //both are small string
        return strcmp(str1->small_string, str2->small_string);
    return strcmp(str1->impl->data, str2->impl->data)==0;   //both are long string
}

static bool is_smaller(CString* str1, CString* str2)    //str1 < str2
{
    if(str1==NULL || str2==NULL)
        return false;
    return strcmp(get_data(str1), get_data(str2))<0;
}

static bool is_bigger(CString* str1, CString* str2)
{
    if(str1==NULL || str2==NULL)
        return false;
    return strcmp(get_data(str1), get_data(str2))>0;
}

static CString* split_by_char(CString* str, char delim, int* n)
{
    if(str!=NULL && n!=NULL)
    {
        char* current=get_data(str);
        int occurence=0;
        while(*current!='\0')
        {
            if(*current==delim)
                ++occurence;
            ++current;
        }
        CString* splitted=malloc(sizeof(CString)*occurence);
        int i=0;
        while(occurence--)
        {
            char* start=get
        }
    }
}

void init_global_vtable()
{
    global_vtable_init=true;
    global_vtable.assign_string=&assign_string;
    global_vtable.assign_cstring=&assign_cstring;

    global_vtable.puts=&put_cstring;
}