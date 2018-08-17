#include <thread>
#include <chrono>
#include <iostream>
#include <exception>

#include <ncurses.h>

#include "View.h"

Input charToInput(int in) {
    switch (in) {
        case KEY_UP: return UP;
        case KEY_DOWN: return DOWN;
        case KEY_LEFT: return LEFT;
        case KEY_RIGHT: return RIGHT;
        case 'q': return QUIT;
        case 'p': return PAUSE;
        default: return INVALID; //anything else is invalid
    }

}

struct Options {
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
};

//helper function gets number between bounds
int readInt(int lower, int upper){
    std::string input;
    std::getline(std::cin, input);
    int val = stoi(input);
    if (val < lower || upper < val) {
        throw (std::out_of_range("The number entered is out of range. Please enter a number between "
                + std::to_string(lower) + " and " + std::to_string(upper) + " (inclusive). "));
    }
    return stoi(input);
}

void setOptions(Options& o) {
    std::cout << "Current options. Enter the associated number to edit \n";
    std::cout << "0 - Height: " << o.height << '\n';
    std::cout << "1 - Width: " << o.width << '\n'; 
    std::cout << "2 - Snake speed: " << o.speed << '\n';
    std::cout << "3 - Snake starting length: " << o.starting_length << '\n';
    std::cout << "4 - Enlargement factor: " << o.enlargement << '\n';
    std::cout << "5 - Borders: " << o.borders_on << '\n';
    std::cout << "6 - Invincibility mode: " << o.invincible << '\n';
    std::cout << "7 - Frames per second: " << o.fps << '\n';
    std::cout << "8 - Portal pairs, " << o.portals.size() << ": \n";
    for (const auto& portal_pair : o.portals) {
        std::cout << "  (" << portal_pair.first.r << ", " 
                << portal_pair.first.c << "), "
                << "(" << portal_pair.second.r << ", " 
                << portal_pair.second.c << ") \n";
    }
    std::cout << "9 - Exit options\n"; 
    int option = -1;
    while (true) {
        try {
            option = readInt(0, 9);
            break;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << "Press 9 to exit options." << std::endl;
        }
    }
        
    switch(option){
        case 0: {
        
        } case 1: {
        
        } case 2: {
        
        } case 3: {
        
        } case 4: {
        
        } case 5: {
        
        } case 6: {
        
        } case 7: {
        
        } case 8: {
        
        } case 9: {
            return;
        }  
    }
}

int main(int argc, char** argv) {
    Options o;
    while (true) {
        std::cout << "Press p to start game, o to set options, or q to quit.\n";
        try {
            setOptions(o);
            break;
        } catch (const std::exception& e) {
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
    Board b(o.height, o.width, o.speed, o.starting_length, o.enlargement, o.borders_on, o.invincible, o.portals);
    View v(o.height, o.width, &b, o.fps);
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
