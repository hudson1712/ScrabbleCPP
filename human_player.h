#include"player.h"

#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

class human_player : public player
{
public:
    human_player(std::string &n) : player(n) {};
    bool take_turn(bag &_bag, board &_board);
    bool exchange_tiles(bag &_bag);
    bool play_word(board &_board);
};

#endif