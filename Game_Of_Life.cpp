#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <array>

#include "Range.hpp"

class Buffer
{
    std::array<std::vector<bool>,2> buffer;
    sf::Uint8* imageBuffer;
    size_t height;
    size_t width;
    bool flag{};
    [[nodiscard]]short countNeighbor(int row, int col) const
    {
        
        return
            (*this)(row - 1, col - 1) + (*this)(row - 1, col) + (*this)(row - 1, col + 1)
            + (*this)(row, col - 1) + (*this)(row, col + 1)
            + (*this)(row + 1, col - 1) + (*this)(row + 1, col) + (*this)(row + 1, col + 1);
    }

    [[nodiscard]]bool isLive(size_t row, size_t col) const
    {
        auto const neighbors = countNeighbor(row, col);
        //Live cell with <2 || >3 neighbors die
        if((*this)(row, col))
            return (neighbors == 2 || neighbors == 3);
        //dead cell with exactly 3 live neighbors becomes live
        return neighbors == 3;
    }


public:
    auto update()
    {
        //auto* imageBufferPtr = reinterpret_cast<sf::Color*>(imageBuffer);
        int i{};
        for (auto [row, col] : Range({ 0 }, height) | Range({ 0 }, width))
        {
            auto const value = isLive(row, col);
            (*this)(row, col, false) = value;
            //*imageBufferPtr = (value ? sf::Color::Red : sf::Color::Black);
            //++imageBufferPtr;
            i += value;
        }
        flag = !flag;
        return i;
    }
    [[nodiscard]]std::vector<bool>::reference operator()(size_t row, size_t col, bool current=true)
    {
        return buffer[current ? flag : !flag][(row + 1) * (width + 2) + col + 1];
    }
    [[nodiscard]]bool operator()(size_t row, size_t col, bool current=true) const
    {
        return buffer[current ? flag : !flag][(row + 1) * (width + 2) + col + 1];
    }

    Buffer(size_t height, size_t width) :
        buffer{ std::vector<bool>((height+2) * (width+2)),std::vector<bool>((height+2) * (width+2)) },
        imageBuffer(new sf::Uint8[height*width*4]),
        height(height), width(width)
    {
        for (auto i : Range({}, width* height / 2))
        {
            (*this)(rand() % height, rand() % width) = true;
        }
    }

    ~Buffer()
    {
        delete[] imageBuffer;
    }

    operator sf::Image()
    {
        std::cout << update() << '\n';
        sf::Image image;
        image.create(width, height, imageBuffer);
        return image;
    }

};

int main()
{
    srand(time(nullptr));
    //sf::RenderWindow window{ sf::VideoMode{1800,1000}, "GOL" };
    Buffer buff{ 1920,1080 };
    while (true)
        std::cout << buff.update() << '\n';
    //sf::Texture texture;
    //texture.loadFromImage(buff);
    //sf::Sprite sprite{ texture };

    //while (window.isOpen())
    //{
    //    sf::Event event;
    //    while(window.pollEvent(event))
    //    {
    //        if (event.type == sf::Event::Closed)
    //            window.close();
    //    }
    //    window.clear();
    //    texture.loadFromImage(buff);
    //    window.draw(sprite);
    //    window.display();
    //}
}
