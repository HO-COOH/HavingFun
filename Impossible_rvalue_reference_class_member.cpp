/*The idea comes from me implementing an example of a "State Design Pattern".
*I have a <Canvas> class and a inheritance hierachy of a <Tool> class.
*The <Canvas> class needs to behave differently according what tool is currently selected.
*An idea is to let <Canvas> class holding a reference to the current <Tool>, and it simply calls the virtual functions of <Tool>.
*/
#include <utility>
class Tool { };
class Pen : public Tool { };
class Eraser : public Tool { };
class Canvas
{
    Tool&& current_tool;
public:
    Canvas(Tool&& tool):current_tool(std::move(tool)){}
    void changeTool(Tool&& tool)
    {
        current_tool = std::move(tool);
    }
    //...
};
int main()
{
    Canvas canvas{Eraser{}};    //Dangling reference 1
    canvas.changeTool(Pen{});   //Dangling reference 2
}
/*However, this idea doesn't work.
*Reference members cannot be bound to temporaries in a member initializer list (Since C++14)
/struct A {
/    A() : v(42) { }  // Error
/    const int& v;
/};
*Note: Same applies to default member initializer
*Source: https://en.cppreference.com/w/cpp/language/initializer_list
*And:
*a temporary bound to a reference member in a constructor initializer list persists only until the constructor exits, not as long as the object exists. (note: such initialization is ill-formed as of DR 1696). (Until C++14)
*
*
*/
//And the changing the rvalue reference member <Tool&&> in <Canvas> to const lvalue& like the following doesn't work either.
class Canvas
{
    Tool const &current_tool;
public:
    //...
};

//The final solution would be to hold a pointer to <Tool>
#include <memory>
class Canvas
{
    std::unique_ptr<Tool> current_tool;
    //...
};
