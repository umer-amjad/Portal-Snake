#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

enum Input {UP = 0, DOWN, LEFT, RIGHT, QUIT, INVALID};

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

