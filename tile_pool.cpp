#include"tile_pool.h"

void tile_pool::remove_tile(tile &t)
{
    if(!this->is_empty()) {
        std::vector<tile>::iterator ptr{set.begin()};
        for(ptr; ptr < set.end(); ptr++) {
            if(*ptr == t) {set.erase(ptr); break;} //Break after first instance of tile with same character, uses overloaded operator== for comparing tiles
        }
    }
}

void tile_pool::add_tile(tile &t)
{
    if(set.size() < 7) set.push_back(tile(t));
}

void tile_pool::show_tiles()
{
    if(!this->is_empty()) {
        for(auto it : set) {std::cout << it.get_character() << " ";}
        std::cout << std::endl;
    } else {std::cout << "This pool is empty";}
}

tile* tile_pool::get_tile(const char &letter)
{
    for(auto &c : set) {
        if(c.get_character() == letter) {
            return &c;
        }
    }
    return nullptr; //Desired tile not found
}
