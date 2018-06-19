#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <deque>
#include <atomic>
#include <mutex>
#include <utility>
#include <map>
#include <set>

#include "Input.h"
#include "Tile.h"

struct Pos {
    int r;
    int c;
};

bool operator==(const Pos& p1, const Pos& p2);

bool operator<(const Pos& p1, const Pos& p2);

typedef Pos Portal;

class Board {
    const int height;
    const int width;

    const int enlarge;
    int length_buffer = 0;

    const bool borders_on;
    const bool invincible;

    int score = 0;

    std::vector<std::vector<Tile>> tiles;

    std::deque<Pos> snake;
    std::atomic<Input> direction;
    int sleep_time; //time before moving

    std::map<Portal, Portal> portals;

    void shiftUp(Pos& p);
    void shiftDown(Pos& p);
    void shiftLeft(Pos& p);
    void shiftRight(Pos& p);

    void generateFood();

    void createPortal(const Pos& enter, const Pos& exit, const int pair_num);

public:

    //width per 2 chars, height per 1 char, speed is number of moves per second 
    Board(int h, int w, int speed, int length, int enlargement, bool borders_on, bool invincible, std::set<std::pair<Portal, Portal>> portal_pairs);

    void advanceSnake();
    void moveSnake();
    void updateDirection(Input in);
    bool isGameOver();

    int getHeight();
    int getWidth();

    char output(int r, int c);
    int getScore();

    std::mutex board_update;
};

#endif /* BOARD_H */

