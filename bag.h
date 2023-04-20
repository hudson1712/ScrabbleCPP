#include"tile_pool.h"

#ifndef BAG_H
#define BAG_H

class bag : public tile_pool
{
public:
    bag();
    tile get_random_tile() const;
};

#endif