#include<vector>
#include<iostream>
#include<sstream>

#include"tile_pool.h"
#include"square.h"
#include"tile.h"
#include"word.h"

#ifndef BOARD_H
#define BOARD_H

//Define colours for use in displaying the different board squares
//Works in powershell, wsl bash
#define DEFAULT "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m" 
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"
#define PINK    "\033[95m"

class board
{
private:
    std::vector<square*> board_squares;
public:
    board();
    ~board();
    void show_board();
    square* get_square(int x, int y);
    std::string get_horizontal_words();
    std::string get_vertical_words();
};
#endif