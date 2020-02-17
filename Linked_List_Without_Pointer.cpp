/*Description: Make a linked list in C++ using only reference*/
/* The challenge is if put std::optional<Node> element inside Node, there will be incomplete type error in the instantiation of std::optional<Node>
* So std::optional<Node> is impossible.
* And std::optional<Node&> is also impossible by design.
* So std::optional<std::reference_wrapper<Node>> seems the only solution.
* Futher reading about why optional of reference doesnot make into the standard:  https://www.fluentcpp.com/2018/10/05/pros-cons-optional-references/
*/
#include <functional>
#include <iostream>
#include <optional>

struct Node
{
    int data;
    std::optional<std::reference_wrapper<Node>> next;
};

int data = 0;

Node head{};
std::reference_wrapper<Node> current = head;

void makeNode()
{
    current.get().next=std::ref(*new Node{data++});
    current = current.get().next.value();
}

void loop(std::function<void(Node&)>visit)   //accepts a function object to apply to the Node in the list
{
    std::optional<std::reference_wrapper<Node>> cur = std::ref(head.next);
    while (cur.has_value())
    {
        auto next = cur->get().next;
        visit(cur->get());
        cur = next;
    }
}

int main()
{
    int const nodeCount = 10;
    for (auto j = 0; j < nodeCount; ++j)
        makeNode();
    loop([](Node& node) {std::cout << node.data << "->"; });    //print the element
    std::cout << '\n';
    loop([](Node& node) {std::cout << "deleting:" << node.data << '\n'; delete& node; });   //clean up
}

/*Output:
0->1->2->3->4->5->6->7->8->9->

deleting:0

deleting:1

deleting:2

deleting:3

deleting:4

deleting:5

deleting:6

deleting:7

deleting:8

deleting:9
*/