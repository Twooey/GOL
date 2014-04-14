#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <chrono>
#include <vector>

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

    std::vector<std::pair<int, int>> starting = {
        {gridsize/2, gridsize/2},
        {gridsize/2 - 1, gridsize/2},
        {gridsize/2, gridsize/2 + 1},
        {gridsize/2, gridsize/2 - 1},
        {gridsize/2 + 1, gridsize/2 + 1}
    };

    for (auto e : starting) {
        grid.set(e.first, e.second);
        std::cout << e.first << " " << e.second << "\n";
    }

    while (true){
        Grid copy = grid;

        //This is our display.
        for (int a = 1; a < gridsize; ++a){
            for (int b = 1; b < gridsize; ++b){
                if (grid.get(a, b))
                    std::cout << " *";
                else
                    std::cout << "  ";
            }
            std::cout << std::endl;
        }

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
        // Don't use system("CLS"), that's not portable.
        std::cout << "\n\n\n";
        std::this_thread::sleep_for(std::chrono::nanoseconds{1'000});
    }
}
