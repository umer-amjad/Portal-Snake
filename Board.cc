#include "Board.h"

#include <iostream>
#include <chrono>
#include <thread>

Board::Board(int h, int w, int speed, int length) : height(h), width(w), sleep_time(1000/speed), tiles(height){
    std::vector<Tile> row(width);
    for (auto& tiles_row : tiles) {
        tiles_row = row;
    }
    direction.store(INVALID);
    int i = 0;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            snake.push_front({r, c});
            tiles[r][c].setSnake();
            ++i;
            if (i == length) break;
        }
        if (i == length) break;
    }
    std::cout << "Height: " << tiles.size() << " Width: " << tiles[0].size() << std::endl;
}

void Board::advanceSnake(){
    Input dir = direction.load();
    Pos f = snake.front();
    switch(dir){
        case LEFT:
            --f.c;
            break;	
        case RIGHT:
            ++f.c;
            break;
        case UP:
            --f.r;
            break;
        case DOWN:
            ++f.r;
            break;
        case INVALID: 
        case QUIT: return;
    }
    f.r = (f.r + height) % height;
    f.c = (f.c + width) % width;
    Pos b = snake.back();
    tiles[b.r][b.c].setEmpty();
    snake.pop_back();
    snake.push_front(f);
    tiles[f.r][f.c].setSnake();
}

void Board::moveSnake(){
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        std::lock_guard<std::mutex> guard(board_update);
        if (direction.load() == QUIT) {
            break;
        }
        advanceSnake();
    }
}

void Board::updateDirection(Input in){
    if (in != INVALID){
        direction.store(in);
    }
}

int Board::getHeight() {
    return height;
}

int Board::getWidth() {
    return width;
}

char Board::output(int r, int c){
    //std::cout << "Get tile at " << r << ", " << c << std::endl;
    return tiles[r][c].getOutput();
}
