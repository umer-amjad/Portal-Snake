#include <thread>
#include <chrono>
#include <iostream>

#include <ncurses.h>

#include "View.h"

Input charToInput(int in) {
    switch (in) {
        case KEY_UP: return UP;
        case KEY_DOWN: return DOWN;
        case KEY_LEFT: return LEFT;
        case KEY_RIGHT: return RIGHT;
        case 'q': return QUIT;
        default: return INVALID; //anything else is invalid
    }

}

int main(int argc, char** argv) {
    int height = 30;
    int width = 40;
    int speed = 11;
    int starting_length = 3;
    int enlargement = 1;
    bool borders_on = false;
    bool invincible = true;
    int fps = 60;
    std::set<std::pair<Portal, Portal>> portals = {{{5, 5}, {15, 15}},
            {{25, 5}, {16, 15}},  
            {{10, 35}, {15, 14}}, 
            {{25, 25}, {14, 15}}, 
            {{5, 15}, {15, 16}}};
    Board b(height, width, speed, starting_length, enlargement, borders_on, invincible, portals);
    View v(height + 2, width * 2 + 2, &b, fps);
    std::thread refresh_screen(&View::displayScreen, &v);
    std::thread move_snake(&Board::moveSnake, &b);

    while (true) {
        int c = wgetch(v.getWindow());
        std::lock_guard<std::mutex> guard(b.board_update);

        if (b.isGameOver()) {
            v.setGameOver();
            break;
        }

        b.updateDirection(charToInput(c));
        if (c == 'q') {
            v.setGameOver();
            break;
        }
    }
    refresh_screen.join();
    move_snake.join();
    std::cout << " You lose!" << std::endl;
    return 0;
}
