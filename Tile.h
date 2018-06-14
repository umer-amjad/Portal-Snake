#ifndef TILE_H
#define TILE_H

class Tile {
    bool isSnake = false;
public:
    Tile();
    void setSnake() {
        isSnake = true;
    }
    void setEmpty() {
        isSnake = false;
    }
    char getOutput() {
        if (isSnake) return 'O';
        return '-';
    }
};

#endif /* TILE_H */

