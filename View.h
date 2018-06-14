#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include <atomic>

#include "Board.h"

class View {
    Window* game_window;
    Board* board;
    int sleep_time;
    std::atomic_bool game_over;
public:
    View(int height, int width, Board* board, int speed); //speed is in fps
    Window* getWindow();
    void refreshScreen();
    void displayScreen();
    void setGameOver();
};

#endif /* VIEW_H */

