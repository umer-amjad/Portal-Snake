#include <thread>
#include <chrono>

#include <ncurses.h>

#include "View.h"

Input charToInput(int in){
    switch(in){
        case KEY_UP: return UP;
        case KEY_DOWN: return DOWN;
        case KEY_LEFT: return LEFT;
        case KEY_RIGHT: return RIGHT;
        default: return INVALID; //anything else is invalid
    }

}


int main(int argc, char** argv){
	Board b(60, 30, 10, 1);
	View v(60+2, 30*2+2, &b, 60);
	
	std::thread refresh_t(&View::displayScreen, &v);
	std::thread move_snake(&Board::moveSnake, &b);
	while(true) {	
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		int c = wgetch(v.getWindow());
		std::lock_guard<std::mutex> guard(b.board_update);
		b.updateDirection(charToInput(c));
		if (c == 'q'){
			v.setGameOver();
			break;
		}
	}	
	refresh_t.join();
	move_snake.join();
	return 0;
}
