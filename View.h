#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include <atomic>

#include "Board.h"

class View {
    WINDOW* game_window;
    Board* board;
    int sleep_time;
    std::atomic_bool game_over;

    void outputTile(int row, int col, char out);
    void initializeScreen();
    void refreshScreen();

public:
    View(int height, int width, Board* board, int speed); //speed is in fps

    void displayScreen();

    WINDOW* getWindow();
    void setGameOver();
    ~View();
};

#endif /* VIEW_H */

