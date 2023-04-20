#include"human_player.h"

//Define templates that allow for both rvalue and lvalue strings as arguments

bool human_player::take_turn(bag &_bag, board &_board)
{   
    //_board.show_board(); //Display the board at the start of the players turn
    while(true) { //Repeat until a successful word is played, tiles are exchanged or turn is passed
        //Allow the player to choose whether to play a word, exchange tiles or pass after viewing their rack
        char decision{' '};

        std::cout << name << "'s turn" << std::endl;
        std::cout << "You have " << score << " points" << std::endl;
        players_tiles.fill_rack(_bag); //Fills the rack if possible before each turn
        std::cout << "Your tiles are: ";
        players_tiles.show_tiles();

        while((std::toupper(decision) != 'W') && (std::toupper(decision) != 'E') && (std::toupper(decision) != 'P')) {
            std::cout << "Would you like to play a word [w], exchange tiles [e], or pass [p]: ";
            std::cin >> decision;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if(std::toupper(decision) == 'W') { //Play word

            if(this->play_word(_board)) return true; //If a word is successfully played, return true to player taken turn

        } else if(std::toupper(decision) == 'E') { //Return tiles to bag and draw new ones

            this->exchange_tiles(_bag);
            return false; //Exchanging tiles counts as pass

        } else if(std::toupper(decision) == 'P'){
            
            return false; //Player passes and nothing happens
        }
    }
}

bool human_player::play_word(board &_board)
{
    int score_buffer{0};
    int word_multiplier{1};
    std::string word_attempt;
    std::pair<int, int> coords;
    char direction{};
    bool first_turn{false};
    if(_board.get_square(8, 8)->get_character() == ' ') {first_turn = true;} //Check if the centre square has a tile on it, if not then it is the first turn

    //Find coordinates of the words first letter
    while(true) { 
        std::cout << "Please enter the x-y co-ordinates, separated by a space, on which to place the first tile of your word: ";
        std::cin >> coords.first >> coords.second;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(coords.first >= 1 && coords.first <= 15 && coords.second >= 1 && coords.second <= 15) { //Ensures co ordinates are within range of the board
            break;
        } else {
            std::cout << "Requires 2 values between 1 and 15 inclusive" << std::endl;
        }
    }

    //Word must be played either across or down from the coords
    while((std::toupper(direction) != 'A') && (std::toupper(direction) != 'D')) {
        std::cout << "Do you wish to place this word across or down? [a/d] ";
        std::cin >> direction;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if((std::toupper(direction) != 'A') && (std::toupper(direction) != 'D')) {std::cout << "Please try again" << std::endl;}
    }
    
    std::cout << "Now enter the word to be played, including any existing tiles on the board: ";
    std::cin >> word_attempt;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string word_uppercase;
    for(auto c : word_attempt) {word_uppercase.push_back(std::toupper(c));}
    
    std::vector<square*> squares_to_occupy;
    std::string exisiting_letters;

    //Grab pointers to squares that tiles will be placed on and create string of existing letters to compare against
    if((std::toupper(direction) == 'A') && ((coords.first-1) + word_uppercase.length() <= 15)) { //Ensures word fits on x axis

        for(int l{0}; l < word_uppercase.length(); l++) {
            squares_to_occupy.push_back(_board.get_square(coords.first + l, coords.second)); //Add square* to vector
            exisiting_letters.push_back(squares_to_occupy[l]->get_character()); //Add character to existing letters, ' ' is returned for empty squares
        }
    } else if((std::toupper(direction) == 'D') && ((coords.second-1) + word_uppercase.length() <= 15)) { //Do same for words going down

        for(int l{0}; l < word_uppercase.length(); l++) {
            squares_to_occupy.push_back(_board.get_square(coords.first, coords.second + l));
            exisiting_letters.push_back(squares_to_occupy[l]->get_character()); 
        }
    } else {
        std::cout << "That word does not fit on the board.\n";
        return false;
    }

    std::string available_letters{players_tiles.get_available_letters() + exisiting_letters}; //All possible letters that can make up the word

    //Check word can played with available letters on the board and in rack
    std::string::iterator it;
    int i{0};
    for(it = word_uppercase.begin(); it != word_uppercase.end(); it++, i++) {
        size_t found_tile = available_letters.find(*it); //Find letter in the avaiable letters(rack and tiles on board)
        if(found_tile == std::string::npos) { //If letter cant be found in rack or board
            std::cout << "Could not find the letter " << *it << " in your rack or on the board!\n";
            return false; //Breaks out of nested for loop and skips rest of code
        } else {
            available_letters.erase(found_tile, 1);
        }
    }
    
    //Puts all characters on board into strings that can then be checked to ensure word would be valid if played
    std::string words_on_board_h{_board.get_horizontal_words()}; //horizontal words
    std::string words_on_board_v{_board.get_vertical_words()}; //vertical words
    std::string previous_board{words_on_board_h + words_on_board_v}; //Creates a copy of the board before it is edited to prevent trying to play words that are already on the board

    //Now insert the new word into the exisiting board
    for(int l{0}; l < word_uppercase.length(); l++) {
        if(std::toupper(direction) == 'A') {
            if(words_on_board_h[(coords.first-1) + (coords.second-1)*16 + l] == ' ') {
                //Each line is 16 chars long since a space was added to split words at either end of board
                words_on_board_h[(coords.first-1) + (coords.second-1)*16 + l] = word_uppercase[l]; 
                words_on_board_v[(coords.first-1 + l)*16 + (coords.second-1)] = word_uppercase[l];
            }
        } else if(std::toupper(direction) == 'D') {
            if(words_on_board_v[(coords.first-1)*16 + (coords.second-1) + l] == ' ') {
                words_on_board_h[(coords.second-1 + l)*16 + (coords.first-1)] = word_uppercase[l];
                words_on_board_v[(coords.first-1)*16 + (coords.second-1) + l] = word_uppercase[l];
            }
        }
    }

    //Check that the word touches another, or passes through the centre on the first turn
    bool plays_off_word{false};
    if(std::toupper(direction) == 'A') {

        std::string temp_row{words_on_board_h.substr(16*(coords.second - 1), 16)};
        if((coords.first > 1) && (temp_row[coords.first-2] != ' ')) {plays_off_word = true;}
        if(((coords.first + word_uppercase.length()) < 15) && (temp_row[coords.first + word_uppercase.length()] != ' ')) {plays_off_word = true;}

        for(int n{0}; n < word_uppercase.length(); n++) {
            std::string temp_column{words_on_board_v.substr(16*(coords.first - 1 + n), 16)};
            if(((coords.first + n) == 8) && (coords.second == 8) && first_turn) {plays_off_word = true;} //On the first turn, if the word passes through 8,8 then it can be played
            if((coords.second > 1) && (temp_column[coords.second-2] != ' ')) {plays_off_word = true;}
            if((coords.second < 15) && (temp_column[coords.second] != ' ')) {plays_off_word = true;}
        }
    } else if(std::toupper(direction) == 'D') {

        std::string temp_column{words_on_board_v.substr(16*(coords.first - 1), 16)};
        if((coords.second > 1) && (temp_column[coords.second-2] != ' ')) {plays_off_word = true;}
        if(((coords.second + word_uppercase.length())< 15) && (temp_column[coords.second + word_uppercase.length()] != ' ')) {plays_off_word = true;}

        for(int n{0}; n < word_uppercase.length(); n++) {
            std::string temp_row{words_on_board_h.substr(16*(coords.second - 1 + n), 16)};
            if((coords.first == 8) && ((coords.second + n) == 8) && first_turn) {plays_off_word = true;} //On the first turn, if the word passes through 8,8 then it can be played
            if((coords.first > 1) && (temp_row[coords.first-2] != ' ')) {plays_off_word = true;}
            if((coords.first < 15) && (temp_row[coords.first] != ' ')) {plays_off_word = true;}
        }
    }
    
    if(!plays_off_word) {
        if(first_turn) {
            std::cout << "You must play across the centre tile on the first turn.\n";
            return false;
        } else {
            std::cout << "You must play off an existing word.\n";
            return false;
        }
    }
    
    //Check all horizontal and vertical words are valid
    if(validate_words(words_on_board_h + words_on_board_v) && plays_off_word) { 
        
        //Prevent cheating by entering a word that is already on the board to score its points
        if(words_on_board_h + words_on_board_v == previous_board) { 
            std::cout << "You must place at least one tile." << std::endl;
            return false;
        }

        //Score and set tiles at same time
        std::string::iterator it;
        int i{0};
        int tiles_used{0};
        for(it = word_uppercase.begin(); it != word_uppercase.end(); it++, i++) {

            if(*it == exisiting_letters[i]) {

                score_buffer += squares_to_occupy[i]->get_value(); //Add the points of the existing tiles without any multipliers

            } else { //Move any letters that aren't already on the board from the rack to the board and score points

                int letter_multiplier{1};
                switch(squares_to_occupy[i]->get_premium()) { //Get the word or letter multiplier from the unused square
                    case multiplier::centre :
                        word_multiplier *= 2;
                        break;
                    case multiplier::triple_word :
                        word_multiplier *= 3;
                        break;
                    case multiplier::double_word :
                        word_multiplier *= 2;
                        break;
                    case multiplier::triple_letter :
                        letter_multiplier = 3;
                        break;
                    case multiplier::double_letter :
                        letter_multiplier = 2;
                        break;
                    default :
                        break;
                }
                squares_to_occupy[i]->set_tile(*players_tiles.get_tile(*it)); //Assign the tile to the square   
                players_tiles.remove_tile(*players_tiles.get_tile(*it)); //Remove it from the rack

                score_buffer += letter_multiplier * squares_to_occupy[i]->get_value();
                tiles_used++;
            }
        }
        score_buffer *= word_multiplier;

        //Crossword points from newly created words that aren't the new word
        score_buffer += get_crossword_score(words_on_board_h + words_on_board_v, std::move(previous_board), word_uppercase);
        if(tiles_used == 7) {score_buffer += 50;} //50 bonus points for using all 7 tiles
        score += score_buffer;
        std::cout << name << " played the word " << word_uppercase << " for " << score_buffer << " points" << std::endl;
        return true;

    } else {
        std::cout << "That word is not in the dictionary, please try again\n";
        return false;
    }
}

bool human_player::exchange_tiles(bag &_bag)
{   
    if(_bag.get_tiles_left() < 7) {
        std::cout << "There must be more than 7 tiles left in the bag for an exchagnge to happen\n";
        return false;
    }
    std::string available_letters{players_tiles.get_available_letters()};
    std::string letters_to_return;
    std::string temp;
    std::string::iterator it;

    std::cout << "Please enter the tiles you wish to redraw from the bag as a single string: ";
    std::cin >> temp;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for(auto &c : temp) {
        letters_to_return.push_back(std::toupper(c));
    }

    for(it = letters_to_return.begin(); it != letters_to_return.end(); it++) {
        size_t found_tile = available_letters.find(*it); //Search for tile
        if(found_tile != std::string::npos) { //If tile is found in rack proceed to move it to the bag
            _bag.add_tile(*players_tiles.get_tile(*it)); //Move to bag
            players_tiles.remove_tile(*players_tiles.get_tile(*it)); //Remove from rack
            available_letters.erase(found_tile, 1); //Remove from the available letters
        } else {
            std::cout << "One or more letters were not found in your rack so they could not be removed: ";
            std::cout << *it;
        }
    }
    players_tiles.fill_rack(_bag); //Draw new tiles and show them
    std::cout << "\nYour new tiles are: "; 
    players_tiles.show_tiles();
    return false;
}