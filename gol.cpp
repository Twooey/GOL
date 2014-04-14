#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <SFML/Graphics.hpp>

class Grid {
    // not using vector<bool> because it is rather strange
    std::vector<int> cells_;
    std::size_t width_;
    // height is implicit in vector size

    int& get_cell(std::size_t x, std::size_t y) {
        return cells_[y*width_ + x];
    }

    int const& get_cell(std::size_t x, std::size_t y) const {
        return cells_[y*width_ + x];
    }

public:
    Grid(std::size_t width, std::size_t height)
        : cells_(width*height)
        , width_(width)
    {}

    void set(std::size_t x, std::size_t y) {
        get_cell(x, y) = 1;
    }

    void unset(std::size_t x, std::size_t y) {
        get_cell(x, y) = 0;
    }

    void put(std::size_t x, std::size_t y, bool value) {
        get_cell(x, y) = value;
    }

    bool get(std::size_t x, std::size_t y) const {
        return get_cell(x, y);
    }

    bool width() const {
        return width_;
    }

    bool height() const {
        return cells_.size() / width_;
    }
};

int main() {
    const int gridsize = 50;
    Grid grid(gridsize+1, gridsize+1);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game of Life");

    std::vector<std::pair<int, int>> starting = {
        {gridsize/2, gridsize/2},
        {gridsize/2 - 1, gridsize/2},
        {gridsize/2, gridsize/2 + 1},
        {gridsize/2, gridsize/2 - 1},
        {gridsize/2 + 1, gridsize/2 + 1}
    };

    for (auto e : starting)
        grid.set(e.first, e.second);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        Grid copy = grid;

        window.clear(sf::Color::Black);
        //This is our display.
        for (int a = 1; a < gridsize; ++a){
            for (int b = 1; b < gridsize; ++b){
                if (grid.get(a, b)) {
                    sf::RectangleShape rectangle(sf::Vector2f{10, 10});
                    rectangle.setPosition(10*a, 10*b);
                    window.draw(rectangle);
                }
            }
        }
        window.display();

        for (int a = 1; a < gridsize; ++a) {
            for (int b = 1; b < gridsize; ++b) {
                int life = 0;

                for(int c = -1; c < 2; ++c)
                    for(int d = -1; d < 2; ++d)
                        if (c != 0 || d != 0)
                            if (copy.get(a+c, b+d)) ++life;

                if (life < 2 || life > 3)
                    grid.unset(a, b);
                else if (life == 3)
                    grid.set(a, b);
            }
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds{100'000});
    }
}
