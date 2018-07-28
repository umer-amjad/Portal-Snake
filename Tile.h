#ifndef TILE_H
#define TILE_H

class Tile {
    bool isSnake = false;
    bool isFood = false;
    int portal = 0;
public:

    Tile() {
    }

    void setSnake() {
        isSnake = true;
        isFood = false;
    }

    bool getSnake() {
        return isSnake;
    }

    void setFood() {
        isFood = true;
    }

    bool getFood() {
        return isFood;
    }

    void setPortal(int pair_num) {
        portal = pair_num;
    }

    bool getPortal() {
        return portal > 0;
    }

    void setEmpty() {
        isSnake = false;
        isFood = false;
    }

    char getOutput() {
        if (isSnake) return 'O';
        if (portal > 0) return portal;
	if (isFood) return 'F';
        return '-';
    }
};

#endif /* TILE_H */

