#include"game.h"

void game::get_players()
{
    int number_of_players{0};
    int number_of_computers{0};
    int total_players{0};

    //Determine the number of players
    while(true) {
        std::cout << "Please enter the number of human players in the game: ";
        std::cin >> number_of_players;
        std::cout << "Please enter the number of computer players in the game: ";
        std::cin >> number_of_computers;
        total_players = number_of_computers + number_of_players;
        if(total_players >= 1 && total_players <= 4) {
            break;
        } else {
            std::cout << "There can only be a maximum of 4 players\n";
        }
    }

    for(int n{0}; n < number_of_players; n++) {
        std::string name;
        std::cout << "Enter the name of player " << n+1 << ": ";
        std::cin >> name;
        players_in_game.push_back(new human_player(name));
    }

    for(int n{0}; n < number_of_computers; n++) {
        std::stringstream name;
        name << "Computer " << n+1;
        players_in_game.push_back(new comp_player(name.str()));
    }
}

void game::play()
{
    int consecutive_passes{0};
    bool ongoing{true};

    //Main game loop
    while(ongoing) {
        
        for(auto player : players_in_game) {
            game_board.show_board();
            if(!player->take_turn(game_bag, game_board)) {
                consecutive_passes++;//player->take_turn returns true if they pass
                if(consecutive_passes == (2 * players_in_game.size())) { //If all players pass twice, end the game
                    ongoing = false;
                    break;
                }
                continue;
            }
            consecutive_passes = 0; //If at least 1 player does not pass, then reset the count
        }
    }

    std::cout << "All players have passed twice, ending the game.\n";
    player* winner;
    int highest_score{0};
    for(auto player : players_in_game) {
        if(player->get_score() >= highest_score) {
            winner = player;
            highest_score = player->get_score();
        }
    }
    std::cout << "The winner is " << winner->get_name() << " with " << winner->get_score() << " points.\n";
}

game::~game()
{
    for(auto p : players_in_game) {
        delete p; //Delete the base class pointers in the player vector
    }
}