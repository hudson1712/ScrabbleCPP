#include<iostream>
#include<string>
#include<vector>
#include<limits>
#include<algorithm>
#include<chrono>

#include"rack.h"
#include"tile.h"
#include"bag.h"
#include"word.h"
#include"board.h"

#ifndef PLAYER_H
#define PLAYER_H

template<typename T> bool validate_words(T&& current_board) //Takes rvalue and lvalue reference as arguments
{
    if(typeid(current_board) != typeid(std::string)) {
        return false;
    }
    std::stringstream sstream{current_board};
    std::string temp;
    
    while(sstream >> temp) {
        if(temp.length() >= 2) { //Ignore single characters
            word temp_word{temp};
            if(!temp_word.is_valid()) {return false;} //Break if a single word is found to be invalid
        }
    }
    return true; //All words are valid
}
template<typename T> int get_crossword_score(T&& new_board, T&& old_board, std::string &played_word) 
{
    //Must pass 2 valid boards as strings, they should have both been validated before calling this function
    std::stringstream sstream_old{old_board};
    std::stringstream sstream_new{new_board};
    std::vector<std::string> new_words;
    std::vector<std::string> old_words;
    std::string temp;
    int score{0};

    while(sstream_old >> temp) { //Skips over whitespace
        if(temp.length() >= 2) { //Ignore single characters
            old_words.push_back(temp);
        }
    }
    while(sstream_new >> temp) { //Skips over whitespace
        if(temp.length() >= 2 && old_words.end() == std::find(old_words.begin(), old_words.end(), temp)) {
            if(temp == played_word) {continue;} //Skip the newly placed word as it can have multipliers
            new_words.push_back(temp);
        }
    }
    for(auto w : new_words) {
        word temp_word{w};
        score += temp_word.get_points();
    }
    return score;
}

class player
{
protected:
    std::string name;
    rack players_tiles;
    int score{0};
public:
    player(std::string n) {name = n;};
    virtual ~player() {};
    virtual bool take_turn(bag &_bag, board &_board)=0;
    virtual bool exchange_tiles(bag &_bag)=0;
    virtual bool play_word(board &_board)=0;
    int get_score() {return score;}
    std::string get_name() {return name;}
};

#endif