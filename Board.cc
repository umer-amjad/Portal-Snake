#include "Board.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>

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
            empties.insert({r, c});
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

    direction.store(INVALID);
    last_direction.store(INVALID);

    Pos firstEmpty = *empties.begin();
    updateSnakeFront(firstEmpty);

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

void Board::updateSnakeFront(Pos& front) {
    snake.push_front(front);
    tiles[front.r][front.c].setSnake();

    empties.erase(front);
    updated.push_back(front);
}

void Board::updateSnakeBack() {
    if (length_buffer == 0) {
        Pos back = snake.back();
        snake.pop_back();
        tiles[back.r][back.c].removeSnake();

        if (tiles[back.r][back.c].isEmpty()) {
            empties.insert(back);
            updated.push_back(back);
        }
    } else {
        --length_buffer;
    }
}

void Board::generateFood() {
    Pos chosen;
    std::sample(empties.begin(), empties.end(), &chosen, 1, rngForEmpty);
    tiles[chosen.r][chosen.c].setFood();

    empties.erase(chosen);
    updated.push_back(chosen);
}

void Board::createPortal(const Pos& enter, const Pos& exit, const int pair_num) {
    portals[enter] = exit;
    portals[exit] = enter;
    tiles[enter.r][enter.c].setPortal(pair_num);
    tiles[exit.r][exit.c].setPortal(pair_num);

    empties.erase(enter);
    empties.erase(exit);
    updated.push_back(enter);
    updated.push_back(exit);

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
    
    updateSnakeBack();

    if (tiles[new_front.r][new_front.c].isFood()) {
        length_buffer += enlarge;
        score += 10;
        generateFood();
    }
    updateSnakeFront(new_front);

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
    switch (in) {
        case QUIT: direction.store(in);
        case INVALID: return;
    }
    Input last = last_direction.load();
    Input curr = direction.load();
    if (curr == PAUSE && in == PAUSE) {
        direction.store(last);
    } else if (curr == PAUSE || // if paused or input is reverse of curr direction
               (in != PAUSE && (in + 2) % 4 == last)) {
        return; // do nothing
    } else {
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
    // if (empties.find({r, c}) != empties.end() && !tiles[r][c].isEmpty()) {
    //     return '*'; // debug error condition
    // }
    return tiles[r][c].getOutput();
}

int Board::getScore() {
    return score;
} 
