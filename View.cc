#include "View.h"

#include <thread>
#include <chrono>
#include <mutex>

View::View(int height, int width, Board* board, int speed) : board(board), sleep_time(1000 / speed) {
    initscr(); //initialize n_curses 
    noecho(); //don't echo user input
    cbreak(); //pass on input directly, no need for newline or space

    //indent game screen:
    int startx = 1;
    int starty = 1;

    game_window = newwin(height, width, starty, startx);
    keypad(game_window, TRUE);
    mvprintw(0, 0, "Use arrow keys to move. Press q to quit.");
    refresh();
    refreshScreen();
}

void View::refreshScreen() {
    box(game_window, 0, 0);
    for (int row = 0; row < board->getHeight(); ++row) {
        //std::string test(r.begin(), r.end());
        //mvwprintw(game_window, y, x, "%s", test.data());
        //++y;
        for (int col = 0; col < board->getWidth(); ++col) {
            mvwprintw(game_window, row + 1, 2 * col + 1, "%c", board->output(row, col));
            mvwprintw(game_window, row + 1, 2 * col + 2, "%c", ' ');
        }
    }
    wrefresh(game_window);
    refresh();
}

void View::displayScreen() {
    while (true) {
        if (game_over.load()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        std::lock_guard<std::mutex> guard(board->board_update);
        refreshScreen();
    }
}

WINDOW* View::getWindow() {
    return game_window;
}

void View::setGameOver(){
    game_over.store(true);
}

View::~View(){
    clrtoeol();
    refresh();
    endwin();
}
