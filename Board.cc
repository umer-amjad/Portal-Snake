#include "Board.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

bool operator==(const Pos& p1, const Pos& p2){
    return (p1.r == p2.r) && (p1.c == p2.c);
}

bool operator<(const Pos& p1, const Pos& p2) {
    if (p1.r == p2.r) {
        return p1.c < p2.c;
    }
    return p1.r < p2.r;
}

Board::Board(int h, int w, int speed, int length) : height(h), width(w), sleep_time(1000 / speed), tiles(height) {
    std::vector<Tile> row(width);
    for (auto& tiles_row : tiles) {
        tiles_row = row;
    }
    direction.store(INVALID);
    int i = 0;
    int r = 0;
    int c = 0;
    while (i < length) {
        tiles[r][c].setSnake();
        snake.push_front({r, c});
        if ((r % 2 == 0 && c == width - 1) || 
            (r % 2 == 1 && c == 0)) {
            ++r;
        } else if (r % 2 == 0) {
            ++c;
        } else {
            --c;
        }
        ++i;
    }
    portals.insert({{5, 5}, {15, 15}});
    portals.insert({{15, 15}, {5, 5}});
    tiles[5][5].setPortal();
    tiles[15][15].setPortal();
    generateFood();
}

//private helper functions:

void Board::shiftUp(Pos& p) {
    --p.r;
    p.r = (p.r + height) % height;
    if (tiles[p.r][p.c].getPortal()) {
        p = portals[p];
        shiftUp(p);
    }
}

void Board::shiftDown(Pos& p) {
    ++p.r;
    p.r = (p.r + height) % height;
    if (tiles[p.r][p.c].getPortal()) {
        p = portals[p];
        shiftDown(p);
    }
}

void Board::shiftLeft(Pos& p) {
    --p.c;
    p.c = (p.c + width) % width;
    if (tiles[p.r][p.c].getPortal()) {
        p = portals[p];
        shiftLeft(p);
    }

}

void Board::shiftRight(Pos& p) {
    ++p.c;
    p.c = (p.c + width) % width;
    if (tiles[p.r][p.c].getPortal()) {
        p = portals[p];
        shiftRight(p);
    }

}

void Board::generateFood() {
    while (true) {
        int rand_r = rand() % height; 
        int rand_c = rand() % width;
        if (!tiles[rand_r][rand_c].getSnake() && !tiles[rand_r][rand_c].getPortal()) {
            tiles[rand_r][rand_c].setFood();
            break;
        }
    }
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

    if (std::find(snake.begin(), snake.end(), f) != snake.end()) {
        direction.store(QUIT);
        return;
    }
    
    if (length_buffer == 0) {
        Pos b = snake.back();
        snake.pop_back();

        //not overlapping:
        if (std::find(snake.begin(), snake.end(), b) == snake.end()) {
            tiles[b.r][b.c].setEmpty();
        }
    } else {
        --length_buffer;
    }

    snake.push_front(f);

    if (tiles[f.r][f.c].getFood()) {
        length_buffer += enlarge;
        generateFood();
    }

    tiles[f.r][f.c].setEmpty();
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

bool Board::isGameOver() {
    return (direction.load() == QUIT);
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
