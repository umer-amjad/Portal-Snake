#ifndef TILE_H
#define TILE_H

#include <list>

struct Pos {
    int r;
    int c;
};

class Tile {
    //snake may overlap with itself many times
    //snake count keeps track
    int snakeCount = 0; 
    bool food = false;
    int portal = 0;
    wchar_t visual = 0;
public:
    Tile() {}
    
    void setSnake(wchar_t vis) {
        visual = vis;
        snakeCount++;
        food = false;
    }

    bool isSnake() {
        return (snakeCount > 0);
    }

    void setFood() {
        food = true;
    }

    bool isFood() {
        return food;
    }

    void setPortal(int pair_num) {
        portal = pair_num;
    }

    bool isPortal() {
        return portal > 0;
    }

    void removeSnake() {
        snakeCount--;
    }

    bool isEmpty() {
        return !food && snakeCount == 0 && portal == 0;
    }

    wchar_t getOutput() {
        if (snakeCount > 0) return visual;
        if (portal > 0) return portal;
        if (food) return 0x0001F34F;
        return ' ';
    }
};

#endif /* TILE_H */

