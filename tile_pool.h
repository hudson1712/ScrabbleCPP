#include<vector>
#include<string>
#include<iostream>
#include<random>
#include<memory>
#include<algorithm>

#include"tile.h"

#ifndef TILE_POOL_H
#define TILE_POOL_H

class tile_pool
{
protected:
    std::vector<tile> set;
public:
    tile_pool() {};
    //tile_pool(const tile_pool &tp);
    //tile_pool& operator=(const tile_pool &&tp); 
    bool is_empty() const {if(set.size() == 0) {return true;} else {return false;}}
    void remove_tile(tile &t);
    void add_tile(tile &t);
    void show_tiles();
    int get_tiles_left() {return set.size();};
    tile* get_tile(const char &letter);
    
};

#endif