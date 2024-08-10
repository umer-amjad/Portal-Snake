#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <deque>
#include <atomic>
#include <mutex>
#include <utility>
#include <map>
#include <set>
#include <random>

#include "Input.h"
#include "Tile.h"

bool operator==(const Pos& p1, const Pos& p2);

bool operator<(const Pos& p1, const Pos& p2);

typedef Pos Portal;

class Board {
    const int height;
    const int width;

    const int enlarge;
    int length_buffer;

    const bool borders_on;
    const bool invincible;

    int score = 0;

    std::vector<std::vector<Tile>> tiles;
    std::set<Pos> empties;
    std::mt19937 rngForEmpty;

    std::deque<Pos> snake;
    std::atomic<Input> direction;
    std::atomic<Input> last_direction;
    int sleep_time; //time before moving

    std::map<Portal, Portal> portals;

    void shiftUp(Pos& p);
    void shiftDown(Pos& p);
    void shiftLeft(Pos& p);
    void shiftRight(Pos& p);

    void updateSnakeFront(Pos& front);
    void updateSnakeBack();
    void advanceSnake();

    void generateFood();

    void createPortal(const Pos& enter, const Pos& exit, const int pair_num);

public:

    //width per 2 chars, height per 1 char, speed is number of moves per second 
    Board(int h, int w, int speed, int length, int enlargement, bool borders_on, bool invincible, std::set<std::pair<Portal, Portal>> portal_pairs);

    void moveSnake();
    void updateDirection(Input in);
    bool isGameOver();

    int getHeight();
    int getWidth();

    char output(int r, int c);
    int getScore();

    std::mutex board_update;
    std::list<Pos> updated;
};

#endif /* BOARD_H */

