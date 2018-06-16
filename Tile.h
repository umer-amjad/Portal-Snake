#ifndef TILE_H
#define TILE_H

class Tile {
    bool isSnake = false;
    bool isFood = false;
    bool isPortal = false;
public:

    Tile() {
    }

    void setSnake() {
        isSnake = true;
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

    void setPortal() {
        isPortal = true;
    }

    bool getPortal() {
        return isPortal;
    }

    void setEmpty() {
        isSnake = false;
        isFood = false;
    }

    char getOutput() {
        if (isSnake) return 'O';
        if (isPortal) return 'X';
	if (isFood) return 'F';
        return '-';
    }
};

#endif /* TILE_H */

