#include "Board.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

Board::Board(int h, int w, int speed, int length) : height(h), width(w), sleep_time(1000 / speed), tiles(height) {
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
}

//private helper functions:

void Board::shiftUp(Pos& p) {
    --p.c;
    p.c = (p.c + width) % width;
}

void Board::shiftDown(Pos& p) {
    ++p.c;
    p.c = (p.c + width) % width;
}

void Board::shiftLeft(Pos& p) {
    --p.r;
    p.r = (p.r + height) % height;
}

void Board::shiftRight(Pos& p) {
    ++p.r;
    p.r = (p.r + height) % height;
}

//public functions:

void Board::advanceSnake() {
    Input dir = direction.load();
    Pos f = snake.front();
    switch (dir) {
        case UP:
            shiftUp(f);
            break;
        case DOWN:
            shiftDown(f);
            break;
        case LEFT:
            shiftLeft(f);
            break;
        case RIGHT:
            shiftRight(f);
            break;
        case INVALID:
        case QUIT: return;
    }
    Pos b = snake.back();
    snake.pop_back();

    //not overlapping:
    if (std::find(snake.begin(), snake.end(), b) == snake.end()) {
        tiles[b.r][b.c].setEmpty();
    }

    snake.push_front(f);
    tiles[f.r][f.c].setSnake();
}

void Board::moveSnake() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        std::lock_guard<std::mutex> guard(board_update);
        if (direction.load() == QUIT) {
            break;
        }
        advanceSnake();
    }
}

void Board::updateDirection(Input in) {
    if (in != INVALID) {
        direction.store(in);
    }
}

int Board::getHeight() {
    return height;
}

int Board::getWidth() {
    return width;
}

char Board::output(int r, int c) {
    return tiles[r][c].getOutput();
}
