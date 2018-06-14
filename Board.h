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
    std::vector<std::vector<Tile>> tiles;
    
    const int height;
    const int width;
    
    std::deque<Pos> snake;
    std::atomic<Input> direction;
    int sleep_time; //time before moving
public:
    
    //width per 2 chars, height per 1 char, speed is number of moves per second 
    Board(int h, int w, int speed, int length);
    void advanceSnake();
    void moveSnake();
    void updateDirection(Input in);
    
    std::mutex board_update;
};

#endif /* BOARD_H */

