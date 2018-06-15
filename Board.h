#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <deque>
#include <atomic>
#include <mutex>

#include "Input.h"
#include "Tile.h"

struct Pos {
    int r;
    int c; 
};

class Board {
    const int height;
    const int width;

    std::vector<std::vector<Tile>> tiles;
    
    std::deque<Pos> snake;
    std::atomic<Input> direction;
    int sleep_time; //time before moving
public:
    
    //width per 2 chars, height per 1 char, speed is number of moves per second 
    Board(int h, int w, int speed, int length);
    void advanceSnake();
    void moveSnake();
    void updateDirection(Input in);

    int getHeight();
    int getWidth();
    
    char output(int r, int c);
    
    std::mutex board_update;
};

#endif /* BOARD_H */

