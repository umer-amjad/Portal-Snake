#include <thread>
#include <chrono>
#include <iostream>

#include <ncurses.h>

#include "View.h"

Input charToInput(int in){
    switch(in){
        case KEY_UP: return UP;
        case KEY_DOWN: return DOWN;
        case KEY_LEFT: return LEFT;
        case KEY_RIGHT: return RIGHT;
        case 'q': return QUIT;
        default: return INVALID; //anything else is invalid
    }

}


int main(int argc, char** argv){
	Board b(30, 30, 10, 1);
	//View v(30+2, 30*2+2, &b, 60);
        Board* board = &b;
	b.updateDirection(RIGHT);
	b.advanceSnake();
	b.advanceSnake();
	b.advanceSnake();
	for (int row = 0; row < board->getHeight(); ++row) {
           for (int col = 0; col < board->getWidth(); ++col) {
                std::cout << board->output(row, col); 
           }
           std::cout << std::endl;
        }	
        //return 0;
	View v(30+2, 30*2+2, &b, 60);
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
