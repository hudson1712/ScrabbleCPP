#include<vector>

#include"tile.h"

#ifndef SQUARE_H
#define SQUARE_H

enum class multiplier{none=1, double_letter, triple_letter, double_word, triple_word, centre};

class square
{
private:
    tile current_tile;
    multiplier premium;
public:
    square(multiplier m) {premium = m;}
    multiplier get_premium() {if(current_tile.get_character() == ' ') return premium; else return multiplier::none;} //Only return the premium if the tile is empty
    void set_tile(tile &t) {if(current_tile.get_character() == ' ') {current_tile.~tile(), current_tile = t;}} //If this square is empty, then allow its tile to be set once
    char get_character() {return current_tile.get_character();}
    int get_value() {return current_tile.get_value();}
};

#endif