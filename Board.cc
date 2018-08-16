#include "Board.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

std::list<Pos> Tile::updated{};

bool operator==(const Pos& p1, const Pos& p2){
    return (p1.r == p2.r) && (p1.c == p2.c);
}

bool operator<(const Pos& p1, const Pos& p2) {
    if (p1.r == p2.r) {
        return p1.c < p2.c;
    }
    return p1.r < p2.r;
}

Board::Board(int h, int w, int speed, int length, int enlargement, bool borders_on, bool invincible, std::set<std::pair<Portal, Portal>> portal_pairs) : height(h), width(w), enlarge(enlargement), borders_on(borders_on), invincible(invincible), sleep_time(1000 / speed), tiles(height) {
    std::vector<Tile> row(width);
    for (auto& tiles_row : tiles) {
        tiles_row = row;
    }
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            tiles[r][c].pos = {r, c};
        }
    }
    direction.store(INVALID);
    last_direction.store(INVALID);
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
    int pair_num = 0;
    for (const auto& portal : portal_pairs) {
        createPortal(portal.first, portal.second, (pair_num % 8) + 1);
        ++pair_num;
    }
    srand(time(0)); //initialize seed for RNG
    generateFood();
}

//private helper functions:

void Board::shiftUp(Pos& p) {
    if (p.r == 0 && borders_on) return;
    --p.r;
    p.r = (p.r + height) % height;
    if (tiles[p.r][p.c].getPortal()) {
        p = portals[p];
        shiftUp(p);
    }
}

void Board::shiftDown(Pos& p) {
    if (p.r == height - 1 && borders_on) return;
    ++p.r;
    p.r = (p.r + height) % height;
    if (tiles[p.r][p.c].getPortal()) {
        p = portals[p];
        shiftDown(p);
    }
}

void Board::shiftLeft(Pos& p) {
   if (p.c == 0 && borders_on) return;
    --p.c;
    p.c = (p.c + width) % width;
    if (tiles[p.r][p.c].getPortal()) {
        p = portals[p];
        shiftLeft(p);
    }
}

void Board::shiftRight(Pos& p) {
   if (p.c == width - 1 && borders_on) return;
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
        if (!tiles[rand_r][rand_c].isSnake() && !tiles[rand_r][rand_c].getPortal()) {
            tiles[rand_r][rand_c].setFood();
            break;
        }
    }
}

void Board::createPortal(const Pos& enter, const Pos& exit, const int pair_num) {
    portals[enter] = exit;
    portals[exit] = enter;
    tiles[enter.r][enter.c].setPortal(pair_num);
    tiles[exit.r][exit.c].setPortal(pair_num);
}

//public functions:

void Board::advanceSnake() {
    Input dir = direction.load();
    Pos new_front = snake.front();
    switch (dir) {
        case UP:
            shiftUp(new_front);
            break;
        case DOWN:
            shiftDown(new_front);
            break;
        case LEFT:
            shiftLeft(new_front);
            break;
        case RIGHT:
            shiftRight(new_front);
            break;
        case INVALID:
        case QUIT: return;
    }

    if (!invincible && tiles[new_front.r][new_front.c].isSnake()) {
        direction.store(QUIT);
        return;
    }
    
    if (length_buffer == 0) {
        Pos b = snake.back();
        snake.pop_back();

        //not overlapping:
        tiles[b.r][b.c].setEmpty();
    } else {
        --length_buffer;
    }

    snake.push_front(new_front);

    if (tiles[new_front.r][new_front.c].isFood()) {
        length_buffer += enlarge;
        score += 10;
        generateFood();
    }
    
    tiles[new_front.r][new_front.c].setSnake();
    last_direction.store(dir);
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
        Input cur = last_direction.load();
        if ((cur == UP && in == DOWN) || 
            (cur == DOWN && in == UP) ||
            (cur == LEFT && in == RIGHT) ||
            (cur == RIGHT && in == LEFT)) {
            return;
        }
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

int Board::getScore() {
    return score;
} 
