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
The first generation is created by applying the above rules simultaneously to every cell in the seed—births and deaths occur simultaneously,
and the discrete moment at which this happens is sometimes called a tick (in other words, each generation is a pure function of the preceding one).
The rules continue to be applied repeatedly to create further generations.
*/
#include <iostream>
#include <chrono>
#include <thread>


//#include <cstdlib>
#include <SFML/Graphics.hpp>

const int gridsize = 300; //Making this a global constant to avoid array issues.


void Display(sf::RenderWindow& window, bool grid[gridsize+1][gridsize+1]) {
    window.clear(sf::Color::Black);
    for (std::size_t a = 1; a < gridsize- 1; ++a){
        for (std::size_t b = 1; b < gridsize - 1; ++b){
            if (grid[a][b]) {
                sf::RectangleShape rectangle(sf::Vector2f{3, 3});
                rectangle.setPosition(3*a, 3*b);
                window.draw(rectangle);
            }
        }
    }
    window.display();
}

/*
void Display(bool grid[gridsize+1][gridsize+1]){
    for(int a = 1; a < gridsize; a++){
        for(int b = 1; b < gridsize; b++){
            if(grid[a][b] == true){
                std::cout << " *";
            }
            else{
                std::cout << "  ";
            }
            if(b == gridsize-1){
                std::cout << " \n";
            }
        }
    }
}
*/
//This copy's the grid for comparision purposes.
void CopyGrid (bool grid[gridsize+1][gridsize+1],bool grid2[gridsize+1][gridsize+1]){
    for(int a =0; a < gridsize; a++){
        for(int b = 0; b < gridsize; b++){grid2[a][b] = grid[a][b];}
    }
}
//Calculates Life or Death
void liveOrDie(bool grid[gridsize+1][gridsize+1]){
    bool grid2[gridsize+1][gridsize+1] = {};
    CopyGrid(grid, grid2);
    for(int a = 1; a < gridsize; a++){
        for(int b = 1; b < gridsize; b++){
            int life = 0;
        for(int c = -1; c < 2; c++){
            for(int d = -1; d < 2; d++){
                if(!(c == 0 && d == 0)){
                    if(grid2[a+c][b+d]) {++life;}
                }
            }
        }
            if(life < 2) {grid[a][b] = false;}
            else if(life == 3){grid[a][b] = true;}
            else if(life > 3){grid[a][b] = false;}
        }
    }
}


int main(){

    sf::RenderWindow window(sf::VideoMode(gridsize*3+100, gridsize*3+100), "Conway's Game of Life");
    std::cout.sync_with_stdio(false);
    //const int gridsize = 50;
    bool grid[gridsize+1][gridsize+1] = {};

    //Still have to manually enter the starting cells.
    int md = gridsize/2;
    grid[md/2][md/2] = true;
    grid[md/2-2][md/2-1] = true;
    grid[md/2-3][md/2-1] = true;
    grid[md/2+1][md/2-1] = true;
    grid[md/2+2][md/2-1] = true;
    grid[md/2+3][md/2-1] = true;
    grid[md/2-2][md/2+1] = true;



    while (true){
        int counter = 0;
        //The following copies our grid.

        Display(window, grid);     //This is our display.
        liveOrDie(grid); //calculate if it lives or dies.
//        ++counter;
//        system("CLS");
    std::this_thread::sleep_for(std::chrono::nanoseconds{'100'});

    }
}
