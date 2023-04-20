#include"tile_pool.h"
#include"bag.h"

#ifndef RACK_H
#define RACK_H

class rack : public tile_pool
{
public:
    rack() : tile_pool() {};
    void fill_rack(bag &b);
    std::string get_available_letters();
};

#endif