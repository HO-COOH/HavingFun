#include "cstring.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define SMALL_STRING_LENGTH SMALL_STRING-1

#define THROW_EXCEPTION(X) fprintf(stderr, (X))
#define THROW_NULL_PTR_EXCEPTION THROW_EXCEPTION("Exception: CString is not small_string, while pointer is NULL!\n")
#define THROW_NULL_PARAM_EXCEPTION THROW_EXCEPTION("Exception: CString passed in is NULL!\n")
#define THROW_FILE_IO_EXCEPTION THROW_EXCEPTION("Exception: File I/O Error!\n")
#define THROW_INVALID_ARGUMENT_PTR_EXCEPTION THROW_EXCEPTION("Exception: Argument CString pointer is invalid!\n")
#define THROW_INVALID_LONG_STR_EXCEPTION THROW_EXCEPTION("Exception: Long CString is invalid!\n")

CString_public global_vtable;

static bool global_vtable_init=false;
static void init_global_vtable();

static char* get_data(CString* str)
{
    if(str)
        return str->is_small? str->data.small_string : str->data.impl.ptr;
    THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
    return NULL;
}

/**
 * @brief: Internal function, get the pointer pointing to the end ('\0') of the string
 */
static char* get_end(CString* str)
{
    if(str)
    {
        if(str->is_small)
        {
            char const* start=get_data(str);
            char const* pos=start;
            while(*pos!='\0'&&pos-start<=SMALL_STRING)
                ++pos;
            return pos;
        }
        return &str->data.impl.ptr[str->data.impl.length-1];
    }
    THROW_NULL_PARAM_EXCEPTION;
    return NULL;
}

/*
 * */
static CString_public* get_vtable()
{
    if(!global_vtable_init)
        init_global_vtable();
    return &global_vtable;
}

static size_t length(CString* str)
{
    if(str)
    {
        if(str->is_small)
            return strlen(str->data.small_string);
        return str->data.impl.length-1;
    }
    THROW_NULL_PARAM_EXCEPTION;
    return 0;
}

/*Constructor*/

/**@brief: internal function 
 * @param length: must be a valid value
 */
static void copy_data(CString* temp, const char* src, int length)
{
    if(length<=SMALL_STRING_LENGTH)
    {
        temp->is_small = true;
        strcpy_s(temp->data.small_string, length+1, src);  
    }
    else
    {
        temp->is_small = false;
        temp->data.impl.capacity = length + 1;
        temp->data.impl.length = length + 1;
        temp->data.impl.ptr = malloc(length + 1);
        strcpy_s(temp->data.impl.ptr, length+1, src);
    }
    temp->public = &global_vtable;
}

CString new_CString(const char *data)
{
    CString temp;
    if(data!=NULL)
    {
        copy_data(&temp, data, strlen(data));
    }
    else
    {
        temp.data.small_string[0]='\0';
        temp.is_small=true;
    }
    if(!global_vtable_init)
        init_global_vtable();
    temp.public=&global_vtable;
    temp.is_moved=false;
    return temp;
}

CString empty_CString()
{
    return new_CString(NULL);
}

CString move_to_CString(char* data)
{
    CString temp;
    temp.is_small = false;
    temp.data.impl.ptr = data;
    temp.data.impl.length=strlen(data)+1;
    temp.data.impl.capacity = temp.data.impl.length;
    temp.public = &global_vtable;
    temp.is_moved=true;
    return temp;
}

CString new_CString_by(char* start, char* end)
{
    if(start==NULL || start==end)
        return empty_CString();
    else    //start != NULL
    {
        if(end==NULL)   //start != NULL, end == NULL
            return new_CString(start);
        if(end<start)   // end < start
            return new_CString_by(end, start);
        size_t length = end - start;
        CString temp;
        copy_data(&temp, start, length);
        temp.is_moved=false;
        return temp;
    }
}

CString getline()
{
    return get_until('\n');
}

/*Destructor*/
void delete_CString(CString *str)
{
    if(str!=NULL && !str->is_small && !str->is_moved)
    {
        if(str->data.impl.ptr)
        {
            free(str->data.impl.ptr);
            str->data.impl.capacity=0;
            str->data.impl.length = 0;
            str->is_small = true;
        }
        else    //not small string, but data is NULL
            THROW_NULL_PTR_EXCEPTION;
    }
}

/*Deserialize*/
CString CString_deserialize(FILE *f)
{
    if(f!=NULL)
    {
        CString temp;
        temp.is_moved=false;
        if(fread(&temp, sizeof(temp), 1, f)!=1)
            THROW_FILE_IO_EXCEPTION;
        if(global_vtable_init==false)
            init_global_vtable();
        temp.public = &global_vtable;
        if(!temp.is_small)
        {
            const size_t capacity = temp.data.impl.capacity;
            temp.data.impl.ptr=malloc(capacity);
            fread(temp.data.impl.ptr, 1, temp.data.impl.capacity, f);
        }
        return temp;
    }
    THROW_FILE_IO_EXCEPTION;
    return empty_CString();
}

CString CString_deserialize_from(char* fileName)
{
    if(fileName)
        return CString_deserialize(fopen(fileName, "rb"));
    THROW_FILE_IO_EXCEPTION;
    return empty_CString();
}

CString line_to_CString(FILE *f)
{
    if(f!=NULL)
    {
        long pos = ftell(f);
        size_t count=0;
        while(fgetc(f)!='\n')
            ++count;
        fseek(f, pos, SEEK_SET);
        CString temp;
        temp.is_moved=false;
        if(count<=SMALL_STRING_LENGTH)
        {
            temp.is_small=true;
            fread(temp.data.small_string, 1, count, f);
            temp.data.small_string[count] = '\0';
        }
        else
        {
            temp.is_small=false;
            temp.data.impl.capacity=count+1;
            temp.data.impl.length=count+1;
            temp.data.impl.ptr=malloc(count+1);
            fread(temp.data.impl.ptr, 1, count, f);
            temp.data.impl.ptr[count] = '\0';
        }            
        if(!global_vtable_init)
                init_global_vtable();
        temp.public=&global_vtable;
        fclose(f);
        return temp;
    }
    fclose(f);
    return new_CString(NULL);
}

CString line_to_CString_from(char* fileName)
{
    if(fileName)
        return line_to_CString(fopen(fileName, "r"));
    THROW_FILE_IO_EXCEPTION;
    return empty_CString();
}



// static void assign_string(CString* str, char* data)
// {
//     if(str!=NULL && data!=NULL)
//     {
//         size_t data_length=strlen(data);
//         if(data_length<=SMALL_STRING-1)
//         {
//             //if @data is small string but @str is not, free the internal of @str
//             if(str->impl!=NULL)
//             {
//                 if (str->impl->data != NULL)
//                     free(str->impl->data);
//             }
//             free(str->impl);
//             str->is_small=true;
//             strcpy(str->small_string, data);
//         }
//         else
//         {
//             //if @data is not small string
//             if(str->is_small)
//             {
//                 //if @data is not small string, but @str is, allocate new space
//                 str->is_small=false;
//                 str->impl=malloc(sizeof(CString_impl));
//                 str->impl->data=malloc(data_length+1);
//                 strcpy(str->impl->data, data);
//                 str->impl->length=data_length;
//                 str->impl->capacity=data_length;
//             }
//             else
//             {
//                 //if neither @data or @str is small string, resign data -> str.impl
//                 if(str->impl->capacity>=data_length)
//                     str->impl->length=data_length;
//                 else
//                 {
//                     str->impl->data=realloc(str->impl->data, data_length+1);
//                     str->impl->capacity=data_length;
//                     str->impl->length=data_length;
//                 }
//                 strcpy(str->impl->data, data);
//             }
//         }
//     }
// }

// void assign_cstring(CString* dest, CString* src)
// {
//     if(dest!=NULL && src!=NULL)
//     {
//         if(src->is_small)
//         {
//             if(dest->is_small)
//                 memcpy(dest->small_string, src->small_string, SMALL_STRING);

//         }
//     }
// }



static void swap_cstring(CString* l, CString* r)
{
    if(l && r)
    {
        CString temp=*l;
        *l=*r;
        *r=temp;
    }
    else
        THROW_NULL_PARAM_EXCEPTION;
}

static bool empty(CString* str)
{
    if(str!=NULL)
    {
        if(str->is_small)
            return str->data.small_string[0]=='\0';
        if(str->data.impl.ptr!=NULL)
        {
            if(str->data.impl.length==0 && str->data.impl.ptr[0]=='\0')
                return true;
        }
        //str is not small, but nullptr
        THROW_INVALID_LONG_STR_EXCEPTION;
        return false;
    }
    THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
    return false;
}



 size_t bytes(CString* str)
 {
     if(str)
     {
         if(str->is_small)
             return SMALL_STRING;
         return str->data.impl.capacity;
     }
     THROW_NULL_PARAM_EXCEPTION;
     return 0;
 }

static CString substr(CString* str, size_t pos)
{
    if(str)
    {
        if(pos>=length(str))
        {
            THROW_EXCEPTION("Exception: invalid position for substr()!\n");
            return empty_CString();
        }
        else
        {
            if((str->is_small && pos<=SMALL_STRING) || (!str->is_small))
                return new_CString(get_data(str)+pos);
        }
    }
    else
    {
        THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
        return empty_CString();
    }
}

 static void resize(CString* str, size_t newSize)
 {
     if(str!=NULL)
     {
         if(newSize<=SMALL_STRING)
         {
             if(str->is_small)
                 return;
             //long string -> small string
             strcpy_s(str->data.small_string, SMALL_STRING_LENGTH ,str->data.impl.ptr);
             str->is_small=true;
         }
         else   //long string -> long string
         {
             if(!str->data.impl.ptr)
             {
                 THROW_NULL_PTR_EXCEPTION;
                 return;
             }
             str->data.impl.ptr=realloc(str->data.impl.ptr, newSize);
             str->data.impl.length=strlen(str->data.impl.ptr)+1;
             str->data.impl.capacity=newSize;
         }
     }
     else
         THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
 }

/**@brief: Internal function for toggling upper and lower case
 */
static void toggle_letter(CString* str, bool upper)
{
    if(str!=NULL)
    {
        char* start, *end;
        if(str->is_small)
        {
            start=str->data.small_string;
            end=str->data.small_string+SMALL_STRING;
        }
        else
        {
            start=str->data.impl.ptr;
            end=str->data.impl.ptr + str->data.impl.length;
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
             str->is_small=false;
             if(str->is_small)
             {
                 char temp[SMALL_STRING];
                 memcpy(temp, str->data.small_string, SMALL_STRING);
                 str->is_small=false;
                 str->data.impl.ptr=malloc(bytes);
                 if(str->data.impl.ptr) //malloc success
                 {
                     str->data.impl.capacity = bytes;
                     strcpy(str->data.impl.ptr, temp);
                     return true;
                 }
                 return false;
             }
             else
             {
                 if(str->data.impl.capacity>=bytes)
                     return true;
                 str->data.impl.ptr = realloc(str->data.impl.ptr, bytes);
                 if(str->data.impl.ptr)
                 {
                     str->data.impl.capacity = bytes;
                     return true;
                 }
                 return false;
             }
         }
         return true;
     }
     THROW_NULL_PARAM_EXCEPTION;
     return false;
 }

/**
 * @brief: Internal function
 * @param dest
 * @param size
 */
static void expand(CString* dest, int size)
{
    if(dest->is_small && length(dest)+size <=SMALL_STRING)
        return;
    dest->data.impl.ptr=realloc(dest->data.impl.ptr, dest->data.impl.length+size);
    dest->data.impl.capacity+=size;
}

static void append_char(CString* dest, char c)
{
    if(dest && c!='\0')
    {
        resize(dest, length(dest)+2);
        char* previous_end=get_end(dest);
        *previous_end=c;
        ++dest->data.impl.length;
        *(previous_end+1)='\0';
    }
}

static void append_string(CString* dest, char* src)
{
    const int to_append_length=strlen(src);
    if(dest && src && strlen(src)!=0)
    {
        expand(dest, to_append_length);
        strcpy(get_data(dest), src);
        dest->data.impl.length+=to_append_length;
    }
}

static void reverse(CString* str)
{
    if(str)
    {
        char* data=get_data(str);
        const size_t str_length=length(str);
        for(size_t i=0; i<str_length/2; ++i)
        {
            const char tmp=data[i];
            data[i]=data[str_length-i-1];
            data[str_length-i-1]=tmp;
        }
        return;
    }
    THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
}

static void reverse_range(CString* str, size_t start, size_t end)
{
    if(str && start < end)
    {
        char* data=get_data(str);
        if(end-start>=length(str))
        {
            THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
            return;
        }
        for(size_t i=start, j=0; i<(end-start)/2; ++i, ++j)
        {
            const char tmp=data[i];
            data[i]=data[end-j-1];
            data[end-j-1]=tmp;
        }
        return;
    }
    THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
}

static void append_int(CString* dest, int64_t num)
{
    if(dest)
    {
//        char digits = 0;
//        int64_t temp = num;
//        while (temp /= 10)
//            ++digits;
        //char buffer[digits + 1 + (num >= 0 ? 0 : 1)];    //VLA warning!
        char buffer[25];    //ENOUGH for any integer
        sprintf(buffer, "%lld", num);
        append_string(dest, buffer);
        return;
    }
    THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
}

static void append_float(CString* dest, double num, unsigned char digits)
{
    /*naive implementation, only works if the integer part can be stored in a int64_t */
    if(dest)
    {
        char buffer[digits + 1];
        //first extract the integer part
        int64_t int_part=(int64_t)num;
        append_int(dest, int_part);
        double float_part=num-(double)int_part;
        buffer[0]='.';
        sprintf(buffer, "%lld", (int64_t) (float_part*pow(10, digits)));
        append_string(dest, buffer);
    }
    THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
}

// static void append_string(CString* dest, char* src)
// {
//     if(dest!=NULL && src!=NULL)
//     {
//         size_t appended_bytes=strlen(dest->small_string)+strlen(src)+1;
//         if(dest->is_small)
//         {
//             if(appended_bytes <= SMALL_STRING)
//                 strcat(dest->small_string, src);
//             else
//             {
//                 dest->is_small=false;
//                 dest->impl=malloc(sizeof(CString_impl));
//                 dest->impl->data=malloc(appended_bytes);
//                 strcpy(dest->impl->data, dest->small_string);
//                 strcat(dest->impl->data, src);
//                 dest->impl->capacity=appended_bytes-1;
//                 dest->impl->length=appended_bytes-1;
//             }
//         }
//         else
//         {
//             if(dest->impl->capacity < dest->impl->length+strlen(src))
//             {
//                 dest->impl->data = realloc(dest->impl->data, appended_bytes);
//                 dest->impl->capacity=appended_bytes-1;
//             }
//             strcat(dest->impl->data, src);
//             dest->impl->length=appended_bytes-1;
//         }
//     }
// }

size_t count_occurrence(CString* str, char c)
{
    if(str)
    {
        char* start=get_data(str);
        char* end=get_end(str);
        size_t count=0;
        while(start++!=end)
        {
            if(*start==c)
                ++count;
        }
        return count;
    }
    THROW_INVALID_ARGUMENT_PTR_EXCEPTION;
    return 0;
}


static size_t * preprocess(const char* pattern)
{
    const size_t length=strlen(pattern)+1;
    size_t * lps=malloc(sizeof(size_t)*length);
    lps[0]=0;
    size_t lps_prev=0;
    size_t i=1;
    while (i<length)
    {
        if (pattern[i]==pattern[lps_prev])
        {
            ++lps_prev;
            lps[i]=lps_prev;
            ++i;
        }
        else
        {
            if(lps_prev!=0)
                lps_prev=lps[lps_prev-1];
            else
            {
                lps[i]=0;
                ++i;
            }
        }
    }
    return lps;
}

size_t find(CString* str, char* pattern)
{
    if(str && pattern)
    {
        const size_t pattern_length=strlen(pattern);
        const size_t text_length=length(str);
        size_t *lps=preprocess(pattern);

        size_t i=0; //for text
        size_t j=0; //for pattern

        char* ptr=get_data(str);
        while(i<text_length)
        {
            if(pattern[j]==ptr[i])
            {
                ++i;
                ++j;
            }
            else
            {
                if(j == pattern_length)
                    return i-pattern_length;
                else
                {
                    if(j!=0)
                        j=lps[j-1];
                    else
                        ++i;
                }
            }
        }
        free(lps);
    }
    return CString_npos;
}

static void replace_string(CString* dest, char* original, char* to_replace)
{
    size_t index=find(dest, original);
    if(index!=CString_npos)
    {

    }
}

/*Serialize*/
static bool serialize(CString* str, FILE* f)
{
    if(str && f)
    {
        if(fwrite(str, sizeof(CString), 1, f)!=1)
        {    
            THROW_FILE_IO_EXCEPTION;
            fclose(f);
            return false;
        }
        if(!str->is_small)
        {
            if(str->data.impl.ptr || fwrite(str->data.impl.ptr, 1, str->data.impl.capacity, f)!=str->data.impl.capacity)
            {
                THROW_FILE_IO_EXCEPTION;
                fclose(f);
                return false;
            }
        }
        fclose(f);
        return true;
    }
    THROW_FILE_IO_EXCEPTION;
    fclose(f);
    return false;
}
static bool serialize_to(CString* str, char* fileName, bool append)
{
    if(fileName)
    {
        FILE *f = fopen(fileName, append? "ab":"wb");
        return serialize(str, f);
    }
    THROW_FILE_IO_EXCEPTION;
    return false;
}

static void put_cstring(CString* str)
{
    if(str!=NULL)
    {
        if(str->is_small)
            puts(str->data.small_string);
        else
        {
            if(str->data.impl.ptr!=NULL)
                puts(str->data.impl.ptr);
            else
                THROW_NULL_PTR_EXCEPTION;
        }
    }
}




 static bool is_equal(CString* str1, CString* str2)
 {
    return strcmp(get_data(str1), get_data(str2))==0;
 }

 static bool is_smaller(CString* str1, CString* str2)    //str1 < str2
 {
    return strcmp(get_data(str1), get_data(str2)) <0;

 }

 static bool is_bigger(CString* str1, CString* str2)
 {
    return strcmp(get_data(str1), get_data(str2))>0;
 }

 static CStringArray split_by_char(CString* str, char delim)
 {
     if(str!=NULL && delim!='\0')
     {
         const char* start=get_data(str);
         const char* current=start;
         int occurrence=0;
         while(*current!='\0')
         {
             if(*current==delim)
                 ++occurrence;
             ++current;
         }
         CStringArray array={.count=occurrence, .arr=malloc(sizeof(CString) * occurrence)};
         current=start;
         int i=0;
         while(*current!='\0')
         {
             if (*current == delim)
                 array.arr[i++] = new_CString_by(start, current);
             ++current;
             start = current;
         }
         return array;
     }
     CStringArray empty_result={.count=0, .arr=NULL};
     return empty_result;
 }

void init_global_vtable()
{
    global_vtable_init=true;
    // global_vtable.assign_string=&assign_string;
    // global_vtable.assign_cstring=&assign_cstring;
    global_vtable.substr=&substr;
    global_vtable.to_upper = &to_upper;
    global_vtable.to_lower = &to_lower;
    global_vtable.serialize = &serialize;
    global_vtable.serialize_to = &serialize_to;
    global_vtable.empty = &empty;
    global_vtable.bytes=&bytes;
    global_vtable.length=&length;
    global_vtable.swap_cstring=&swap_cstring;
    global_vtable.resize=&resize;
    global_vtable.is_equal=&is_equal;
    global_vtable.is_bigger=&is_bigger;
    global_vtable.is_smaller=&is_smaller;
    global_vtable.split_by_char=&split_by_char;
    global_vtable.append_char=&append_char;
    global_vtable.append_string=&append_string;
    global_vtable.count_occurrence=&count_occurrence;
    global_vtable.reverse=&reverse;
    global_vtable.reverse_range=&reverse_range;

    global_vtable.puts=&put_cstring;
}

void print_info(const CString* str)
{
    if(str)
        printf("CString: %s\t length/capacity(include terminator): %d / %d moved: %s\n", get_data(str), length(str)+1 , bytes(str), str->is_moved? "true": "false");
}

bool TEST(const CString* src, const char* expected)
{
    if(memcmp(get_data(src), expected, strlen(expected)+1)!=0)
    {
        THROW_EXCEPTION("Error!");
        return false;
    }
    fprintf(stderr, "Passed!");
    return true;
}

CString get_char_n(size_t n)
{
    CString temp;
    size_t i=0;
    temp.is_moved=false;
    if(n>SMALL_STRING_LENGTH)
    {
        temp.is_small=false;
        temp.data.impl.ptr=malloc(n+1);
        temp.data.impl.length=n+1;
        temp.data.impl.capacity=n+1;
        while(i<n)
            temp.data.impl.ptr[i++]=(char)getchar();
    }
    else
    {
        temp.is_small=true;
        while(i<n)
            temp.data.small_string[i++]=(char)getchar();
    }
    temp.public=get_vtable();
    return temp;
}

CString get_until(char stop_flag)
{
    CString temp;
    size_t count=0;
    bool stop=false;
    temp.is_moved=false;
    temp.is_small=true;
    while(count<SMALL_STRING_LENGTH)
    {
        char c=(char)getchar();
        temp.data.small_string[count++]=c;
        if(c==stop_flag)
        {
            stop=true;
            break;
        }
    }
    if(!stop)   //big string
    {
        //first copy the small string part into dynamic memory
        char* data=malloc(2*SMALL_STRING);
        memcpy(data, temp.data.small_string, SMALL_STRING);
        temp.data.impl.capacity=2*SMALL_STRING;
        temp.data.impl.ptr=data;
        temp.is_small=false;

        //Then, the capacity will doubled everytime it is full
        while(true)
        {
            if(count==temp.data.impl.capacity-1)
            {
                temp.data.impl.ptr = realloc(temp.data.impl.ptr, 2 * (count + 1));
                temp.data.impl.capacity*=2;
            }
            char c = (char) getchar();
            if (c == stop_flag)
                break;
            temp.data.impl.ptr[count++] = c;
        }
        temp.data.impl.ptr[count+1]='\0';
        temp.data.impl.length=strlen(temp.data.impl.ptr);
    }
    else
        temp.data.small_string[count+1]='\0';
    temp.public=get_vtable();
    return temp;
}
