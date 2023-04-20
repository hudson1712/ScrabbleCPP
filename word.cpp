#include"word.h"

bool word::is_valid()
{
    //extern std::unordered_set<std::string> dictionary_hash_table; //Use the globally declared dictionary from Scrabble.cpp
    if(dictionary_hash_table.find(word_uppercase) != dictionary_hash_table.end()) {return true;}
    else return false;
}

int word::get_points()
{
    //Finds the sum of all letter values in the word, useful for checking crosswords that can have no multipliers applied.
    if(basic_score == 0) {
        for(auto l : word_uppercase) {
            basic_score += letter_values.find(l)->second;
        }
    }
    return basic_score;
}

int word::get_tile_value(const int &l)
{
    return letter_values[word_uppercase[l]];
}

std::unordered_set<std::string> initialise_dictionary()
{
    std::unordered_set<std::string> hash_table;
    std::fstream dict("dictionary_collins.txt");
    std::string str;
    while(std::getline(dict, str)) {
        hash_table.insert(str); //Assigns each word(key) in dictionary.txt to the hash table
    }
    dict.close();
    return hash_table;
}

std::unordered_map<char, int> initiliaise_letter_values()
{
    std::unordered_map<char, int> hash_map;
    std::vector<char> letters{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'}; //Blank tiles are not implemented
    std::vector<int> points{1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10}; //Score of each tile
    std::vector<int>::iterator p{points.begin()};
    for(auto l : letters) {
        hash_map[l] = *p;
        p++;
    }
    return hash_map;
}

//Initialise the static map and set of the word class
std::unordered_set<std::string> word::dictionary_hash_table{initialise_dictionary()};
std::unordered_map<char, int> word::letter_values{initiliaise_letter_values()};