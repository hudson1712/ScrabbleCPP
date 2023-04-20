#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>

#include"tile.h"

#ifndef WORD_H
#define WORD_H

class word
{
private:
    static std::unordered_set<std::string> dictionary_hash_table; //initialised in word.cpp
    static std::unordered_map<char, int> letter_values; //initialised in word.cpp
    std::string word_uppercase;
    int basic_score{0}; //The total points in the word without any tile multipliers
public:
    word(std::string &input) {for(auto &c : input) {word_uppercase.push_back(std::toupper(c));} } //Converts input string to uppercase
    bool is_valid();
    int get_points();
    int get_tile_value(const int &l);
};

#endif