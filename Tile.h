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
public:
    Pos pos;
    static std::list<Pos> updated;
    
    Tile() {
    }
    
    void setSnake() {
        snakeCount++;
        food = false;
        updated.push_back(pos);
    }

    bool isSnake() {
        return (snakeCount > 0);
    }

    void setFood() {
        food = true;
        updated.push_back(pos);
    }

    bool isFood() {
        return food;
    }

    void setPortal(int pair_num) {
        portal = pair_num;
        updated.push_back(pos);
    }

    bool getPortal() {
        return portal > 0;
    }

    void setEmpty() {
        snakeCount--;
        food = false;
        updated.push_back(pos);
    }

    char getOutput() {
        if (snakeCount > 0) return 'O';
        if (portal > 0) return portal;
	if (food) return '#';
        return ' ';
    }
};

#endif /* TILE_H */

