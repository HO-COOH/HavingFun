/* Descrpition: Make the best string implementation in C
* Mimicing the same small string optimization behavior in C++
* Using object-oriented programming paradigm
*/

/* Contribution: Open an issue and tell me the bug /
* Open an issue and tell me what new functionality to add
*/

/**Planing to have 2 branches for different search algorithm:
 * - traditional O(m*n) method
 * - boyer-moore algorithm (will be in experimental)
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define SMALL_STRING 24
#define CString_npos SIZE_MAX
typedef struct cstring_impl_t CString_impl;
typedef struct cstring_public_t CString_public;
struct cstring_impl_t
{
    char *ptr;
    size_t length;
    size_t capacity;
};
typedef union data_t{
    char small_string[SMALL_STRING];
    CString_impl impl;
} Data;
typedef enum TYPE_ENUM
{
    int_Type,
    float_Type,
    double_Type,
    unsigned_int_Type,
    short_Type,
}TYPE_ENUM;

/*Definition for CString */
typedef struct cstring_t
{
    Data data;
    CString_public *public;
    bool is_small;
    bool is_moved;
}CString;

typedef struct CString_Array_t
{
    int count;
    CString * arr;
}CStringArray;

struct cstring_public_t
{
    void (*assign_string)(CString* str, char *data);  //assign a char* string
    CString (*substr)(CString* str, size_t pos);  //return a substr staring at index pos
    void (*assign_cstring)(CString* dest, CString *src);  //assign a cstring
    bool (*empty)(CString *str);    //check whether the string is empty
    size_t (*length)(CString *str); //return the length of the string only, excluding '\0'
    size_t (*bytes)(CString *str);  //return the bytes of the string, including '\0'
    void (*resize)(CString *str, size_t newSize);   //resize the string, may truncate
    void (*to_upper)(CString *str);
    void (*to_lower)(CString *str);
    bool (*reserve_length)(CString *str, size_t length);    //reserve new space in length, return success or failure
    void (*append_cstring)(CString *dest, CString *src);    //dest += src
    void (*append_string)(CString *dest, char *src);    //dest += src
    void (*append_char)(CString *dest, char c); //dest += c
    void (*append_int)(CString *dest, int64_t num);
    void (*append_float)(CString* dest, double num, unsigned char digits);
    void (*swap_cstring)(CString *l, CString *r);       //l <-> r
    void (*swap_string)(CString *l, char *r);   //l <-> r
    void (*reverse)(CString* str);
    void (*reverse_range)(CString* str, size_t start, size_t end);  //reverse [start, end)
    size_t (*find)(CString *str, char *pattern);    //find the first occurrence of pattern string in str
    size_t (*rfind)(CString *str, char *pattern);   //find the last occurrence of pattern string in str
    size_t (*find_first_of)(CString* str, char *pattern);    //find the first occurrence of characters in pattern
    size_t (*find_last_of)(CString* str, char *pattern);     //find the last occurrence of characters in pattern
    size_t (*find_first_of_after_pos)(CString *str, char *pattern, size_t pos);//find the first occurrence of characters in pattern after pos position in str
    size_t (*find_last_of_before_pos)(CString *str, char *pattern, size_t pos); //find the last occurrence of characters in pattern before pos position in str
    size_t (*find_and_replace)(CString* str, char* to_replace);
    size_t (*count_occurrence)(CString *str, char c);    //count #times 'c' appeared in the string

    void (*replace_string)(CString *dest, char *original, char* to_replace);   //find pattern in dest and replace dest
    void (*replace_cstring)(CString *dest, CString *original, CString* to_replace);   //find pattern in dest and replace dest

    /*Compare*/
    bool (*is_equal)(CString* str1, CString* str2);     //str1 =?= str2
    bool (*is_bigger)(CString* str1, CString* str2);    //str1 >? str2
    bool (*is_smaller)(CString* str1, CString* str2);   //str <? str2

    /*split and return array of CString according to delim, and modify n => number of splited*/
    CString *(*split_by_char)(CString *str, char delim, int* n);
    CString *(*split_by_string)(CString *str, char *delim, int *n);
    CString *(*split_by_cstring)(CString *str, CString *delim, int *n);

    /*serialize*/
    bool (*serialize)(CString *str, FILE *f);
    bool (*serialize_to)(CString *str, char *fileName, bool append);

    /*print*/
    void (*puts)(CString *str);
};

/*Constructor*/
CString new_CString(const char *data);
CString empty_CString();
CString move_to_CString(char *data);
CString new_CString_by(char* start, char* end);

/**
 * @brief: Read one line from stdin
 * @return: a CString
 */
CString getline();

/**
 * @brief: Read from stdin, until [n] characters
 * @param n: numbers of chars to read from stdin, excluding null
 * @return: a CString
 */
CString get_char_n(size_t n);

/**
 * @brief: Read from stdin, until the character [stop_flag] is encountered
 * @param stop_flag: the character to stop
 * @return a CString
 */
CString get_until(char stop_flag);

/*Destructor*/
void delete_CString(CString *str);

/*Deserialize*/
CString CString_deserialize(FILE *f);
CString CString_deserialize_from(char* fileName);
CString line_to_CString(FILE *f);
CString line_to_CString_from(char *fileName);

/*Conversions to CString*/
CString to_cstring(void *data, TYPE_ENUM type);

/*Check CString info*/
void print_info(const CString* str);

/*TEST*/
bool TEST(const CString* src, const char* expected);