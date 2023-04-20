#ifndef TILE_H
#define TILE_H

//Tile class member functions declared in header file as very compact
class tile
{
private:
    char letter;
    int value;
public:
    tile() {letter=' ', value=0;} //Default constructor is an empty tile
    tile(char &l, int &v) : letter{l}, value{v} {};
    tile(const tile &t) : letter{t.get_character()}, value{t.get_value()} {};
    char get_character() const {return letter;}
    int get_value() const {return value;}
    bool operator==(tile comparison) {if(comparison.letter == this->letter) return true; else return false;}
};

#endif