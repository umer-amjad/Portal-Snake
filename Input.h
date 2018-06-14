#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

enum Input { UP = -1, DOWN = 1, LEFT = -1, RIGHT = 1, QUIT, INVALID = 0};

Input charToInput(int in){
    switch(in){
        case KEY_UP: return UP;
        case KEY_DOWN: return DOWN;
        case KEY_LEFT: return LEFT;
        case KEY_RIGHT: return RIGHT;
        default: return INVALID; //anything else is invalid
    }

}





#endif /* INPUT_H */

