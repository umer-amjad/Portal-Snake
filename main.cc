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
    Board b(30, 30, 10, 5);
    View v(30 + 2, 30 * 2 + 2, &b, 60);
    std::thread refresh_t(&View::displayScreen, &v);
    std::thread move_snake(&Board::moveSnake, &b);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        int c = wgetch(v.getWindow());
        std::lock_guard<std::mutex> guard(b.board_update);

        if (b.isGameOver()) {
            std::cout << "Game over" << std::endl;
            v.setGameOver();
            break;
        }

        b.updateDirection(charToInput(c));
        if (c == 'q') {
            v.setGameOver();
            break;
        }
    }
    refresh_t.join();
    move_snake.join();
    std::cout << " You lose!" << std::endl;
    return 0;
}
