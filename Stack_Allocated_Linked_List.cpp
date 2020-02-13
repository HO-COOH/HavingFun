/*Description: Try to make a stack-allocated linked list in C/C++*/
/* Using a recursive function, we can make a stack-allocated linked list of any size
*  And because it is stack-allocated, there is no need to free the memory afterward.
*/

#include <iostream>
#include <random>
#include <chrono>
#include <forward_list>
#include <algorithm>
#include <iterator>

class Timer
{
    std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();
public:
    ~Timer() { std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - prev).count() << " microsec.\n\n"; }
};

int recursion_time;

template<typename T>
struct Node
{
    T data;
    Node* next;
};

Node<int> head;
auto current = &head;
int recursion = 0;
bool const enableOutput = false;

template<typename T>
void useList(Node<T>* head)
{
    auto cur = head;
    while (cur != nullptr)
    {
        if(enableOutput)
            std::cout << cur->data << " -> ";
        cur = cur->next;
    }
}

int makeData()  //just an example to produce data
{
    static std::mt19937 eg( std::chrono::steady_clock::now().time_since_epoch().count() );
    static std::uniform_int_distribution rd(0,100);
    return rd(eg);
}

template<typename T>
void buildLinkedList()
{
    Node<T> node{makeData(), nullptr};
    current->next = &node;
    current = current->next;
    if (recursion++ < recursion_time)
        buildLinkedList<T>();
    else
    {
        useList(&head);  //using the linked list
        current = &head;    //clean the current pointer so that we can start over again
    }
}

template<typename T>
void compare()
{
    {
        std::cout << "Using my stack-allocated linked list: \n";
        Timer t;
        buildLinkedList<T>();
    }
    {
        std::cout << "Using my std::forward_list: \n";
        Timer t;
        std::forward_list<T> l;
        auto count = recursion_time;
        while (count-- > 0)
            l.push_front(makeData());
        if(enableOutput)
            std::copy(l.cbegin(), l.cend(), std::ostream_iterator<int>(std::cout, " -> "));
    }
    {
        std::cout << "Using my heap-allocated linked list: \n";
        Timer t;
        /*build the list*/
        auto head = new Node<T>{ 0,nullptr };
        auto current = head;
        auto count = 0;
        while (count++ < recursion_time)
        {
            current->next = new Node<int>{ makeData(), nullptr };
            current = current->next;
        }
        useList(head);
        /*Clean*/
        current = head;
        auto next = current->next;
        while (next != nullptr)
        {
            delete current;
            current = next;
            next = current->next;
        }
    }
}

int main()
{
    recursion_time = 100000;
    
    for (auto i = 0; i < 10; ++i)
    {
        recursion = 0;
        compare<int>();
    }
}

/******************************Release Mode***********************************/

// Using my stack-allocated linked list:
// 5 microsec.

// Using my std::forward_list:
// 7 microsec.

// Using my heap-allocated linked list:
// 8 microsec.

// Using my stack-allocated linked list:
// 3 microsec.

// Using my std::forward_list:
// 7 microsec.

// Using my heap-allocated linked list:
// 7 microsec.

// Using my stack-allocated linked list:
// 2 microsec.

// Using my std::forward_list:
// 6 microsec.

// Using my heap-allocated linked list:
// 6 microsec.

// Using my stack-allocated linked list:
// 3 microsec.

// Using my std::forward_list:
// 6 microsec.

// Using my heap-allocated linked list:
// 7 microsec.

// Using my stack-allocated linked list:
// 2 microsec.

// Using my std::forward_list:
// 7 microsec.

// Using my heap-allocated linked list:
// 6 microsec.

// Using my stack-allocated linked list:
// 2 microsec.

// Using my std::forward_list:
// 6 microsec.

// Using my heap-allocated linked list:
// 7 microsec.

// Using my stack-allocated linked list:
// 3 microsec.

// Using my std::forward_list:
// 14 microsec.

// Using my heap-allocated linked list:
// 7 microsec.

// Using my stack-allocated linked list:
// 3 microsec.

// Using my std::forward_list:
// 6 microsec.

// Using my heap-allocated linked list:
// 6 microsec.

// Using my stack-allocated linked list:
// 7 microsec.

// Using my std::forward_list:
// 15 microsec.

// Using my heap-allocated linked list:
// 15 microsec.

// Using my stack-allocated linked list:
// 6 microsec.

// Using my std::forward_list:
// 13 microsec.

// Using my heap-allocated linked list:
// 9 microsec.


// C:\Users\Peter\source\repos\Random\x64\Release\Random.exe (process 16332) exited with code 0.
// Press any key to close this window . . .

/******************************Debug Mode***********************************/
// Using my stack-allocated linked list:
// 82 microsec.

// Using my std::forward_list:
// 209 microsec.

// Using my heap-allocated linked list:
// 117 microsec.

// Using my stack-allocated linked list:
// 103 microsec.

// Using my std::forward_list:
// 186 microsec.

// Using my heap-allocated linked list:
// 87 microsec.

// Using my stack-allocated linked list:
// 72 microsec.

// Using my std::forward_list:
// 179 microsec.

// Using my heap-allocated linked list:
// 91 microsec.

// Using my stack-allocated linked list:
// 70 microsec.

// Using my std::forward_list:
// 196 microsec.

// Using my heap-allocated linked list:
// 90 microsec.

// Using my stack-allocated linked list:
// 72 microsec.

// Using my std::forward_list:
// 177 microsec.

// Using my heap-allocated linked list:
// 90 microsec.

// Using my stack-allocated linked list:
// 72 microsec.

// Using my std::forward_list:
// 196 microsec.

// Using my heap-allocated linked list:
// 93 microsec.

// Using my stack-allocated linked list:
// 74 microsec.

// Using my std::forward_list:
// 176 microsec.

// Using my heap-allocated linked list:
// 87 microsec.

// Using my stack-allocated linked list:
// 71 microsec.

// Using my std::forward_list:
// 173 microsec.

// Using my heap-allocated linked list:
// 87 microsec.

// Using my stack-allocated linked list:
// 71 microsec.

// Using my std::forward_list:
// 192 microsec.

// Using my heap-allocated linked list:
// 94 microsec.

// Using my stack-allocated linked list:
// 72 microsec.

// Using my std::forward_list:
// 186 microsec.

// Using my heap-allocated linked list:
// 89 microsec.


// C:\Users\Peter\source\repos\Random\x64\Debug\Random.exe (process 13316) exited with code 0.
// Press any key to close this window . . .

/*Conclusion： As we can see, in the debug mode, averagely using the stack-allocated linked list is a little bit faster than heap-allocated ones.
* But the difference is more significant in the release mode.
* Also, std::forward_list is consistenly slower than these two, because of higher overhead.
*/