#include "Board.h"

#include <chrono>
#include <thread>

Board::Board(int h, int w, int speed, int length) : height(h), width(w), sleep_time(1000/speed), tiles(height){
    std::vector<Tile> row(width);
    for (auto& tiles_row : tiles) {
        tiles_row = row;
    }
    direction.store(INVALID);
    int i = 0;
    for (int r = 0; r < tiles.size(); r++) {
        for (int c = 0; c < tiles[0].size(); c++) {
            snake.push_front({r, c});
            tiles[r][c].setSnake();
            ++i;
            if (i == length) break;
        }
        if (i == length) break;
    }
}

void Board::advanceSnake(){
    Input dir = direction.load();
    Pos f = snake.front();
    Pos new_f{0, 0};
    switch(dir){
        case LEFT:
        case RIGHT:
            new_f = {f.r, (f.c + dir)};
            break;
        case UP:
        case DOWN:
            new_f = {f.r + dir, f.c};
            break;
        case INVALID: 
        case QUIT: return;
    }
    new_f.r = (new_f.r + height) % height;
    new_f.c = (new_f.c + width) % width;
    Pos b = snake.back();
    tiles[b.r][b.c].setEmpty();
    snake.pop_back();
    snake.push_front(newFront);
    tiles[new_f.r][new_f.c].setSnake();
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


