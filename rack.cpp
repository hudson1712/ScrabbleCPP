#include"rack.h"

void rack::fill_rack(bag &b) //Fills the rack with tiles from the bag using a random sample
{
    while((set.size() < 7) && !b.is_empty()) { //Stops when either the rack is full or the bag is empty
        tile new_tile{b.get_random_tile()};
        this->add_tile(new_tile);
        b.remove_tile(new_tile);
    }
}

std::string rack::get_available_letters()
{
    std::string avail_letters;
    for(auto t : set) {
        avail_letters.push_back(t.get_character());
    }
    return avail_letters;
}