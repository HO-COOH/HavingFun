/* Descrpition: Make the best string implementation in C
* Mimicing the same small string optimization behavior in C++
* Using object-oriented programming paradigm
*/

/* Contribution: Open an issue and tell me the bug /
* Open an issue and tell me what new functionality to add
*/
#include <stdio.h>
#include <stdbool.h>
#define SMALL_STRING 16
typedef struct cstring_impl_t CString_impl;
typedef struct cstring_public_t cstring_public;
typedef enum TYPE_ENUM
{
    int_Type,
    float_Type,
    double_Type,
    unsigned_int_Type,
    short_Type,
}TYPE_ENUM;
typedef struct cstring_t
{
    char small_string[SMALL_STRING];
    CString_impl *impl;
    cstring_public *public;
    bool is_small;
}CString;

struct cstring_public
{
    void (*assign_string)(char *data);  //assign a char* string
    CString (*substr)(size_t pos);  //return a substr staring at index pos
    void (*assign_cstring)(CString *data);  //assign a cstring
    bool (*empty)(CString *str);    //check whether the string is empty
    size_t (*length)(CString *str); //return the length of the string only, excluding '\0'
    size_t (*bytes)(CString *str);  //return the bytes of the string, including '\0'
    void (*resize)(CString *str);   //resize the string, may truncate
    void (*to_upper)(CString *str);
    void (*to_lower)(CString *str);
    bool (*reserve_bytes)(CString *str, size_t bytes);  //reserve new space in bytes, return success or failure
    bool (*reserve_length)(CString *str, size_t length);    //reserve new space in length, return success or failure
    void (*append_cstring)(CString *dest, CString *src);    //dest += src
    void (*append_string)(CString *dest, char *src);    //dest += src
    void (*append_char)(CString *dest, char c); //dest += c
    void (*swap_cstring)(CString *l, CString *r);       //l <-> r
    void (*swap_string)(CString *l, char *r);   //l <-> r
    size_t (*find)(CString *str, char *pattern);    //find the first occurrence of pattern string in str
    size_t (*rfind)(CString *str, char *pattern);   //find the last occurrence of pattern string in str
    size_t (*find_first_of)(CString* str, char *pattern);    //find the first occurrence of characters in pattern
    size_t (*find_last_of)(CString* str, char *pattern);     //find the last occurrence of characters in pattern
    size_t (*find_first_of_after_pos)(CString *str, char *pattern, size_t pos);//find the first occurrence of characters in pattern after pos position in str
    size_t (*find_last_of_before_pos)(CString *str, char *pattern, size_t pos); //find the last occurrence of characters in pattern before pos position in str
    size_t (*count_occurrence)(CString *str, char c);    //count #times 'c' appeared in the string

    bool (*replace_string)(CString *dest, char *pattern);   //find pattern in dest and replace dest
    bool (*replace_cstring)(CString *dest, CString *pattern);   //find pattern in dest and replace dest
    
    /*split and return array of CString according to delim, and modify n => number of splited*/
    CString *(*split_by_char)(CString *str, char delim, int* n);
    CString *(*split_by_string)(CString *str, char *delim, int *n);
    CString *(*split_by_cstring)(CString *str, CString *delim, int *n);

    /*serialize*/
    bool (*serialize)(CString *str, FILE *f);

    /*print*/
    void (*puts)(CString *str);
};

/*Constructor*/
CString new_CString(char *data);

/*Destructor*/
void delete_CString(CString *str);

/*Deserialize*/
CString CString_deserialize(FILE *f);
CString line_to_cstring(FILE *f);

/*Conversions to CString*/

CString to_cstring(void *data, TYPE_ENUM type);