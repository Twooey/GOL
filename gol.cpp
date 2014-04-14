/*
The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells,
each of which is in one of two possible states, alive or dead. Every cell interacts with its eight neighbours,
which are the cells that are horizontally, vertically, or diagonally adjacent.
At each step in time, the following transitions occur:

    1)Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    2)Any live cell with two or three live neighbours lives on to the next generation.
    3)Any live cell with more than three live neighbours dies, as if by overcrowding.
    4)Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

The initial pattern constitutes the seed of the system.
The first generation is created by applying the above rules simultaneously to every cell in the seed births and deaths occur simultaneously,
and the discrete moment at which this happens is sometimes called a tick (in other words, each generation is a pure function of the preceding one).
The rules continue to be applied repeatedly to create further generations.
*/

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

    std::size_t width() const {
        return width_;
    }

    std::size_t height() const {
        return cells_.size() / width_;
    }
};

void display(sf::RenderWindow& window, Grid const& grid) {
    window.clear(sf::Color::Black);
    for (std::size_t a = 1; a < grid.width() - 1; ++a){
        for (std::size_t b = 1; b < grid.height() - 1; ++b){
            if (grid.get(a, b)) {
                sf::RectangleShape rectangle(sf::Vector2f{10, 10});
                rectangle.setPosition(10*a, 10*b);
                window.draw(rectangle);
            }
        }
    }
    window.display();
}

void update(Grid& grid) {
    Grid copy = grid;
    for (std::size_t a = 1; a < grid.width() - 1; ++a) {
        for (std::size_t b = 1; b < grid.height() - 1; ++b) {
            int life = 0;

            for(int c = -1; c < 2; ++c)
                for(int d = -1; d < 2; ++d)
                    if (c != 0 || d != 0)
                        if (copy.get(a+c, b+d))
                            ++life;

            if (life < 2 || life > 3)
                grid.unset(a, b);
            else if (life == 3)
                grid.set(a, b);
        }
    }
}

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

        display(window, grid);

        update(grid);

        std::this_thread::sleep_for(std::chrono::nanoseconds{100'000});
    }
}

