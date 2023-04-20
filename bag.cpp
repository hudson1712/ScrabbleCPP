#include"bag.h"

bag::bag() : tile_pool() //Initialise the bag to contain all tiles, not including blank tiles
{   
    std::vector<char> letters{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'}; //Blank tiles are not implemented
    std::vector<int> points{1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10}; //Score of each tile
    std::vector<int> amounts{9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1}; //Amount of each tile

    std::vector<char>::iterator l{letters.begin()};
    std::vector<int>::iterator a{amounts.begin()};
    std::vector<int>::iterator p{points.begin()};
    
    for(l; l < letters.end(); l++, a++, p++) {
        for(int n{0}; n < *a; n++) {
            set.push_back(tile(*l, *p));
        }
    }
}

tile bag::get_random_tile() const //Returns a randomly sampled tile
{
    tile* random_tile;
    std::sample(set.begin(), set.end(), random_tile, 1, std::mt19937{std::random_device{}()}); //uses the a Mersenne Twister generator for samples
    return *random_tile;
}