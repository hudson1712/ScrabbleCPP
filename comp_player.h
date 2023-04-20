#include"player.h"

#ifndef COMP_PLAYER_H
#define COMP_PLAYER_H

class comp_player : public player
{
public:
    comp_player(std::string name) : player(name) {};
    bool take_turn(bag& _bag, board& _board);
    bool play_word(board& _board);
    bool exchange_tiles(bag& _bag);
};

#endif