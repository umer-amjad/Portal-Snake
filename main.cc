#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>

#include <ncurses.h>

static const int WIDTH = 30;
static const int HEIGHT = 25;

std::mutex refresh_mutex;
std::atomic_bool quit;
std::atomic_int input;

void continouslyMove(std::vector<std::vector<char>>& screen, int& char_x, int& char_y){
	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::lock_guard<std::mutex> guard(refresh_mutex);
                int old_x = char_x;
                int old_y = char_y;

		int c = input.load();
                switch(c){
                        case KEY_UP:
                                --char_y;
                                break;
                        case KEY_DOWN:
                                ++char_y;
                                break;
                        case KEY_LEFT:
                                --char_x;
                                break;
                        case KEY_RIGHT:
                                ++char_x;
				break;
			default: continue; //ignore anything else
		}
                if (quit.load()) {
                        break;
                }
                char_y = (char_y + HEIGHT-2) % (HEIGHT - 2);
                char_x = (char_x + WIDTH-2) % (WIDTH - 2);
                screen[char_y][char_x] = 'O';
                screen[old_y][old_x] = 'X';
	}
}

void printScreen(WINDOW *game_window, std::vector<std::vector<char>>& screen){
        int x = 1;
        int y = 1;
        box(game_window, 0, 0);
        for(int row = 0; row < screen.size(); ++row) {
                std::vector<char>& r = screen[row];
                //std::string test(r.begin(), r.end());
                //mvwprintw(game_window, y, x, "%s", test.data());
                //++y;
                for(int col = 0; col < r.size(); ++ col) {
			mvwprintw(game_window, row + 1, 2*col + 1, "%c", screen[row][col]);
			mvwprintw(game_window, row + 1, 2*col + 2, "%c", ' ');
                }
        }
        wrefresh(game_window);
        refresh();
}


void refreshScreen(WINDOW *game_window, std::vector<std::vector<char>>& screen){
	while(true) {
		if (quit.load()) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		std::lock_guard<std::mutex> guard(refresh_mutex);
		printScreen(game_window, screen);
	}
}

int main(int argc, char** argv){
	WINDOW *game_window;
	quit.store(false);
	input.store(0);
	std::vector<std::vector<char>> screen;
	std::vector<char> row(WIDTH - 1, '-');
	for (int i = 0; i < HEIGHT - 2; ++i){
		screen.push_back(row);
	}
	int char_x = 0;
	int char_y = 0;
	screen[char_y][char_x] = 'O';
	int highlight = 1;
	int choice = 0;
	int c = 0; //input char

	initscr(); //initialize n_curses 
	noecho(); //don't echo user input
	cbreak(); //pass on input directly, no need for newline or space
	int startx = 2;
	int starty = 1;
		
	game_window = newwin(HEIGHT, WIDTH*2, starty, startx);
	keypad(game_window, TRUE);
	mvprintw(0, 0, "Use arrow keys to move. Press q to quit.");
	refresh();
	printScreen(game_window, screen);
	wrefresh(game_window);
	std::thread refresh_t(refreshScreen, game_window, std::ref(screen));
	std::thread move_snake(continouslyMove, std::ref(screen), std::ref(char_x), std::ref(char_y));
	while(1) {	
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		int old_x = char_x;
		int old_y = char_y;
		c = wgetch(game_window);
		std::lock_guard<std::mutex> guard(refresh_mutex);
		if (c == KEY_UP || c == KEY_DOWN ||
		    c == KEY_LEFT || c == KEY_RIGHT){
			input.store(c);
		}
		if (c == 'q'){
			quit.store(true);
		}
		if (quit.load()) {
			break;
		}
		char_y = (char_y + HEIGHT-2) % (HEIGHT - 2);
		char_x = (char_x + WIDTH-2) % (WIDTH - 2);
		screen[char_y][char_x] = 'O';
		screen[old_y][old_x] = 'X'; 
	}	
	refresh_t.join();
	move_snake.join();
	clrtoeol();
	refresh();
	endwin();
	return 0;
}
