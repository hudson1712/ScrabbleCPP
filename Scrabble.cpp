//Scrabble project
//Samuel Hudson 08/04/22 - 13/05/22
//Scrabble game playable in console

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
#include"game.h"

int main()
{
    //Game loop allows for multiple games to be played
    while(true) {
        char play_new_game{' '};
        while((std::toupper(play_new_game) != 'Y') && (std::toupper(play_new_game)) != 'N') {
            std::cout << "Would you like to start a new game [y/n] ";
            std::cin >> play_new_game;
        }
        if(std::toupper(play_new_game) == 'Y') {
            game new_game;
            new_game.get_players();
            new_game.play();
        } else if(std::toupper(play_new_game) == 'N') {
            break;
        }
    }
    return 0;
}