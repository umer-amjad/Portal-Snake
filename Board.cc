#include "Board.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>

std::list<Pos> Tile::updated{};

bool operator==(const Pos& p1, const Pos& p2) {
    return (p1.r == p2.r) && (p1.c == p2.c);
}

bool operator<(const Pos& p1, const Pos& p2) {
    if (p1.r == p2.r) {
        return p1.c < p2.c;
    }
    return p1.r < p2.r;
}

Board::Board(int h, int w, int speed, int length, int enlargement, bool borders_on, bool invincible, std::set<std::pair<Portal, Portal>> portal_pairs) :
    height(h), width(w), enlarge(enlargement), length_buffer(length - 1), borders_on(borders_on), invincible(invincible), sleep_time(1000 / speed), tiles(height), rngForEmpty(time(0)) {
    std::vector<Tile> row(width);
    for (auto& tiles_row : tiles) {
        tiles_row = row;
    }
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            tiles[r][c].pos = {r, c};
        }
    }

    int pair_num = 1;
    for (const auto& portal : portal_pairs) {
        createPortal(portal.first, portal.second, pair_num);
        ++pair_num;
        if (pair_num == 8) {
            pair_num = 1;
        }
    }
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            if (tiles[r][c].isEmpty()) {
                empties.insert({r, c});
            }
        }
    }

    direction.store(INVALID);
    last_direction.store(INVALID);
    auto firstEmpty = empties.begin();
    tiles[firstEmpty->r][firstEmpty->c].setSnake();
    snake.push_front(*firstEmpty);
    empties.erase(*firstEmpty);

    generateFood();
}

//private helper functions:

void Board::shiftUp(Pos& p) {
    if (p.r == 0 && borders_on) return;
    --p.r;
    p.r = (p.r + height) % height;
    if (tiles[p.r][p.c].isPortal()) {
        p = portals[p];
        shiftUp(p);
    }
}

void Board::shiftDown(Pos& p) {
    if (p.r == height - 1 && borders_on) return;
    ++p.r;
    p.r = (p.r + height) % height;
    if (tiles[p.r][p.c].isPortal()) {
        p = portals[p];
        shiftDown(p);
    }
}

void Board::shiftLeft(Pos& p) {
   if (p.c == 0 && borders_on) return;
    --p.c;
    p.c = (p.c + width) % width;
    if (tiles[p.r][p.c].isPortal()) {
        p = portals[p];
        shiftLeft(p);
    }
}

void Board::shiftRight(Pos& p) {
   if (p.c == width - 1 && borders_on) return;
    ++p.c;
    p.c = (p.c + width) % width;
    if (tiles[p.r][p.c].isPortal()) {
        p = portals[p];
        shiftRight(p);
    }
}

void Board::generateFood() {
    Pos chosen;
    std::sample(empties.begin(), empties.end(), &chosen, 1, rngForEmpty);
    tiles[chosen.r][chosen.c].setFood();
    empties.erase(chosen);
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
    if (dir == PAUSE) {
        return;
    }
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
        tiles[b.r][b.c].removeSnake();
        if (tiles[b.r][b.c].isEmpty()) {
            empties.insert({b.r, b.c});
        }
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
    empties.erase({new_front.r, new_front.c});
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
        Input last = last_direction.load();
        if ((last == UP && in == DOWN) || 
            (last == DOWN && in == UP) ||
            (last == LEFT && in == RIGHT) ||
            (last == RIGHT && in == LEFT)) {
            return;
        }
        if (direction.load() == PAUSE && in == PAUSE) {
            direction.store(last);
        } else {
            direction.store(in);
        }
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
    // if (empties.find({r, c}) != empties.end() && !tiles[r][c].isEmpty()) {
    //     return '*'; // debug error condition
    // }
    return tiles[r][c].getOutput();
}

int Board::getScore() {
    return score;
} 
