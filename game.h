#include<vector>
#include<iostream>
#include<string>
#include<algorithm>

#include"tile.h"
#include"square.h"
#include"tile_pool.h"
#include"bag.h"
#include"board.h"
#include"word.h"
#include"player.h"
#include"rack.h"
#include"human_player.h"
#include"comp_player.h"

class game
{
private:
    board game_board;
    bag game_bag;
    std::vector<player*> players_in_game;
public:
    game() {};
    ~game();
    void get_players();
    void play();
};
