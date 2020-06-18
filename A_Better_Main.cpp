/*Description: Make a better version of the main function that accepts command line arguments
* In Java, the main function looks like this:
        public static void main(String argv[]) {//...}
* The argv[] is an easy-to-use String array
* In C++, we can achieve a similar way to construct a vector of such arguments in string by using two iterators
* And it happens that a pointer can be used as an iterator.
* So we have &argv[0] as the begin iterator and &argv[argc] as the ending iterator
* Note:
*  &argv[0](begin) ->   argv[0] -> "SomeCommandLineString1"
*                       argv[1] -> "SomeCommandLineString2"
*                           ...
*                       argv[argc-1] -> "TheLastString"
*  &argv[argc](end) ->  pass end iterator
*/

#include <vector>
//#include <string>
#include <string_view>  //use C++17
#include <iterator>
#include <iostream>

int my_main(std::vector<std::string_view> args);

int main(int argc, char* argv[])
{
    return my_main(std::vector<std::string_view>{&argv[0], &argv[argc]});
}

#define main my_main
int main(std::vector<std::string_view> args) //fake main(), actually my_main()
{
    std::copy(args.cbegin(), args.cend(), std::ostream_iterator<std::string_view>{std::cout, "\n"});   //show the arguments
    //do other main() stuff here...
    return 0;
}
/*Possible Output:
C:\Users\Peter\Desktop\HavingFun>a.exe a b c d
a.exe
a
b
c
d
*/