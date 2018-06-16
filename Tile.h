#ifndef TILE_H
#define TILE_H

class Tile {
    bool isSnake = false;
    bool isFood = false;
public:

    Tile() {
    }

    void setSnake() {
        isSnake = true;
    }

    void setFood() {
        isFood = true;
    }

    bool getFood() {
        return isFood;
    }

    void setEmpty() {
        isSnake = false;
        isFood = false;
    }

    char getOutput() {
        if (isSnake) return 'O';
	if (isFood) return 'X';
        return '-';
    }
};

#endif /* TILE_H */

