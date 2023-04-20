#include"comp_player.h"

//Define templates that allow for both rvalue and lvalue strings as arguments

std::vector<std::string> permute(std::string str)
{
    //Given a string, returns a vector of all possible lengths of combinations with no repeats
    //String of 7 unique letters will have 7! 7 letter permutations, 6! 6 letter perms etc..
    //checks at most last 7 letters of vector to search for duplicates, if not found, add new shorter permutation to vector.

    std::vector<std::string> unique_strings;
    std::sort(str.begin(), str.end()); //Sort the string first
    int length = str.length();
    int perm_number{0};
    do {
        unique_strings.push_back(str); //Add permutation since unique
        for(int n{1}; n < str.length(); n++) { 
            //Check for unique permutations only in the last 7 or fewer iterations
            if(std::find(unique_strings.begin() + std::max((perm_number - length), 0), unique_strings.end(), str.substr(0, str.length()-n)) == unique_strings.end() ) {
                unique_strings.push_back(str.substr(0, str.length() - n));
            }
        }
        perm_number++;
    } while(std::next_permutation(str.begin(), str.end())); //Do all unique permutations of the string
    return unique_strings;
}

bool comp_player::take_turn(bag &_bag, board &_board)
{
    //Computer will try to play the highest scoring word, if not possible it will exchange tiles before passing.
    std::cout << name << "'s turn.\n";
    std::cout << name <<" is on " << score << " points\n";
    std::cout << "Computer is thinking...\n";
    players_tiles.fill_rack(_bag);
    if(this->play_word(_board)) {
        return true; //Plays a word, does not pass
    }
    this->exchange_tiles(_bag); //If computer cannot play a word with sufficient points, exchange tiles and pass
    return false;
}

bool comp_player::play_word(board &_board)
{
    //Tries to play all possible words from the tiles and letters on the board, if no play exists, then exchange tiles
    auto start = std::chrono::high_resolution_clock::now();
    bool first_turn{false};
    if(_board.get_square(8, 8)->get_character() == ' ') {first_turn = true;} //If the central square is empty, it is the first turn

    int best_score{0}; //Can be changed to only play a word if over a certain number of points
    std::string best_word; //Best word is used for output
    std::string best_perm; //Best permutation is used for actually playing tiles onto board
    std::pair<int, int> best_coords;
    char best_dir{};
    std::vector<square*> squares_to_occupy;
    
    //Puts all characters on board into strings to be added to later for checking crosswords
    std::string words_on_board_h{_board.get_horizontal_words()}; //horizontal words
    std::string words_on_board_v{_board.get_vertical_words()}; //vertical words
    int empty_squares{0};
    
    //Generate a vector of unique permutations of the rack of all lengths, ~6000 maximum possible permutations for a rack of 7 unique tiles
    std::vector<std::string> possible_permutations{permute(this->players_tiles.get_available_letters())};

    //Loop through all possible horizontal words and find highest scoring one
    for(int word_length{1}; word_length <= this->players_tiles.get_tiles_left(); word_length++) { //Start with shortest words first

        //Extract all words of word_length from the possible permutations to new vector
        std::vector<std::string> permutations_to_use;
        for(auto p : possible_permutations) {if(p.length() == word_length) permutations_to_use.push_back(p);}

        //Loop over every co-ordinate pair on the board
        for(int y{1}; y <= 15; y++) { 
            for(int x{1}; x <= 15; x++) {

                //First, get all letters on the row and column and determine the maximum continuous length of any word played here
                std::string inline_letters_x{words_on_board_h.substr(16*(y-1), 16)}; //Find all letters in this row
                std::string inline_letters_y{words_on_board_v.substr(16*(x-1), 16)}; //Find all letters in this column

                int empty_spaces_x{0};
                int empty_spaces_y{0};

                for(int i_x{x}; inline_letters_x[i_x - 1] == ' '; i_x++) {empty_spaces_x++;}
                for(int i_y{y}; inline_letters_y[i_y - 1] == ' '; i_y++) {empty_spaces_y++;}
                
                int max_length_x{std::min((16 - x), std::min(word_length, empty_spaces_x))};
                int max_length_y{std::min((16 - y), std::min(word_length, empty_spaces_y))};

                //try all permutations in these co ordinates for horizontal words
                for(auto perm : permutations_to_use) {

                    if(first_turn && y != 8) {continue;}

                    if(word_length > max_length_x) {continue;} //Skip any permutations that wont fit across
                    //if(!valid_placement_x) {continue;}
                    
                    int original_length = perm.length();
                    bool plays_off_word{false};

                    int offset_x{0}; //Offset required to get score from adding to an existing word
                    std::string str{perm}; //The word to play starts from the permutation of letters, used to find prefixes/suffixes and then check if it is valid

                    //Check for prefix/suffix of existing word/intersects with word on board
                    if(x > 1) {
                        int letters_to_left{0};
                        for(letters_to_left; inline_letters_x[x - 2 - letters_to_left] != ' '; letters_to_left++) {
                            str = inline_letters_x[x - 2 - letters_to_left] + str; //Insert characters to the left of the word
                            if((x - letters_to_left) == 0) {break;} //Stop if reaches end of board
                        }
                        if(letters_to_left > 0) {plays_off_word = true;}
                        offset_x = letters_to_left;
                    }
                    if(x < 15) {
                        int letters_to_right{0};
                        for(letters_to_right; inline_letters_x[x - 1 + original_length + letters_to_right] != ' '; letters_to_right++) {
                            str += inline_letters_x[x - 1 + original_length + letters_to_right]; //Add characters to the right of the word
                            letters_to_right++;
                        }
                        if(letters_to_right > 0) {plays_off_word = true;}
                    }
                    
                    //Check for new perpendicular words that dont intersect
                    for(int n{0}; n < original_length; n++) {
                        std::string temp_column{words_on_board_v.substr(16*(x - 1 + n), 16)};
                        if(((x + n) == 8) && (y == 8) && first_turn) {plays_off_word = true;} //On the first turn, if the word passes through 8,8 then it can be played
                        if((y > 1) && (temp_column[y-2] != ' ')) {plays_off_word = true;}
                        if((y < 15) && (temp_column[y] != ' ')) {plays_off_word = true;}
                    }

                    word temp{str};

                    if(temp.is_valid() && plays_off_word) { //Check the word is valid and attaches to another before checking crosswords

                        std::string temp_words_h{words_on_board_h};
                        std::string temp_words_v{words_on_board_v};

                        //Insert the permutation into a copy of the board so crosswords can be checked and score
                        for(int l{0}; l < original_length; l++) {
                            if(temp_words_h[(x-1) + (y-1)*16 + l] == ' ') {
                                //Each line is 16 chars long since a space was added to split words at end
                                temp_words_h[(x - 1) + (y - 1)*16 + l] = perm[l]; 
                                temp_words_v[(x - 1 + l)*16 + (y - 1)] = perm[l];
                            }
                        }
                        
                        //If the word and crosswords are valid and newly placed, then score them
                        if(validate_words(temp_words_h + temp_words_v) && temp_words_h != words_on_board_h) {

                            //std::cout << perm << ' ' << x << " " << y << "\t";
                            int word_multiplier{1};
                            int score_buffer{0};

                            for(int l{0}; l < str.length(); l++) {
                                int letter_score{temp.get_tile_value(l)};
                                int letter_multiplier{1};
                                switch(_board.get_square(x + l - offset_x, y)->get_premium()) { //Get the word or letter multiplier from the unused square
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
                                score_buffer += (letter_multiplier * letter_score);
                            }
                            score_buffer *= word_multiplier;

                            //Adds the score from all created crosswords, ignores the score of the new word and all premiums
                            score_buffer += get_crossword_score(temp_words_h + temp_words_v, words_on_board_h + words_on_board_v, str);
                            if(perm.length() == 7) {score_buffer += 50;} //50 points for all tiles

                            if(score_buffer > best_score) {
                                best_coords.first = x;
                                best_coords.second = y;
                                best_perm = perm;
                                best_word = str;
                                best_score = score_buffer;
                                best_dir = 'A'; //This loop was for all horizontal words
                            }
                        }
                    }
                }

                //Now repeat the above code for vertical words
                for(auto perm : permutations_to_use) {
                    
                    if(first_turn && x != 8) {continue;}

                    if(perm.length() > max_length_y) {continue;} //Skip any permutations that wont fit down
                    //if(!valid_placement_y) {continue;}
                    bool plays_off_word{false};

                    //bool plays_off_word{plays_on_word(std::make_pair(x, y), 'D', word_length, words_on_board_h, words_on_board_v)};
                    int original_length = perm.length();
                    int offset_y{0}; //Offset required to get score from adding to an existing word
                    std::string str{perm}; //The word to play starts from the permutation of letters, used to find prefixes/suffixes and then check if it is valid

                    //Check for prefix/suffix of existing word/intersects with word on board
                    if(y > 1) {
                        int letters_above{0};
                        for(letters_above; inline_letters_y[y - 2 - letters_above] != ' '; letters_above++) {
                            str = inline_letters_y[y - 2 - letters_above] + str; //Insert characters above the word
                            if((y - letters_above) == 0) {break;} //Stop if reaches end of board
                        }
                        if(letters_above > 0) {plays_off_word = true;}
                        offset_y = letters_above;
                    }
                    if(y < 15) {
                        int letters_below{0};
                        for(letters_below; inline_letters_y[y - 1 + original_length + letters_below] != ' '; letters_below++) {
                            str += inline_letters_y[y - 1 + original_length + letters_below]; //Add characters below the word
                            letters_below++;
                        }
                        if(letters_below > 0) {plays_off_word = true;}
                    }
                    
                    //Check for new perpendicular words that dont intersect but touch
                    for(int n{0}; n < original_length; n++) {
                        std::string temp_row{words_on_board_h.substr(16*(y - 1 + n), 16)};
                        if((x == 8) && ((y + n) == 8) && first_turn) {plays_off_word = true;} //On the first turn, if the word passes through 8, 8 then it can be played
                        if((x > 1) && (temp_row[x-2] != ' ')) {plays_off_word = true;}
                        if((x < 15) && (temp_row[x] != ' ')) {plays_off_word = true;}
                    }

                    word temp{str};

                    if(temp.is_valid() && plays_off_word) { //Check the word is valid and attaches to another before checking crosswords

                        std::string temp_words_h{words_on_board_h};
                        std::string temp_words_v{words_on_board_v};

                        //Insert the permutation into a copy of the board so crosswords can be checked and score
                        for(int l{0}; l < original_length; l++) {
                            if(temp_words_v[(x - 1)*16 + (y - 1) + l] == ' ') {
                                temp_words_h[(y - 1 + l)*16 + (x-1)] = perm[l];
                                temp_words_v[(x - 1)*16 + (y - 1) + l] = perm[l];
                            }
                        }

                        //If the word and crosswords are valid and newly placed, then score them
                        if(validate_words(temp_words_h + temp_words_v) && temp_words_h != words_on_board_h) {

                            //std::cout << perm << ' ' << x << " " << y << "\t";
                            int word_multiplier{1};
                            int score_buffer{0};

                            for(int l{0}; l < str.length(); l++) {
                                int letter_score{temp.get_tile_value(l)};
                                int letter_multiplier{1};
                                switch(_board.get_square(x, y + l - offset_y)->get_premium()) { //Get the word or letter multiplier from the unused square
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
                                score_buffer += (letter_multiplier * letter_score);
                            }
                            score_buffer *= word_multiplier;

                            //Adds the score from all created crosswords, ignores the score of the new word and all premiums
                            score_buffer += get_crossword_score(temp_words_h + temp_words_v, words_on_board_h + words_on_board_v, str);
                            if(perm.length() == 7) {score_buffer += 50;} //50 points for all tiles

                            if(score_buffer > best_score) {
                                best_coords.first = x;
                                best_coords.second = y;
                                best_perm = perm;
                                best_word = str;
                                best_score = score_buffer;
                                best_dir = 'D'; //This loop was for all vertical words
                            }
                        }
                    }
                }
            }
        }
    }
    
    //Output the time taken and the best word
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cout << "Total elapsed time: " << duration.count() << "ms\n";

    if(best_score != 0) {
        std::cout<<"The best word is: "<<best_word<<" at co ordinates "<<best_coords.first<<", "<<best_coords.second<<" for "<<best_score<<" points.\n";
    }
    
    //Grab pointers to squares that tiles will be placed on and then add the tiles to the board and points to player
    if(best_dir == 'A') {

        for(int l{0}; l < best_perm.length(); l++) {
            squares_to_occupy.push_back(_board.get_square(best_coords.first + l, best_coords.second)); //Add square* to vector
            squares_to_occupy[l]->set_tile(*players_tiles.get_tile(best_perm[l])); //Assign the tile to the square
            players_tiles.remove_tile(*players_tiles.get_tile(best_perm[l])); //Remove from rack
        }
        score += best_score; //Score has already been calculated, so just add it
        return true; //Successfully played a word

    } else if(best_dir == 'D') {

        for(int l{0}; l < best_perm.length(); l++) {
            squares_to_occupy.push_back(_board.get_square(best_coords.first, best_coords.second + l)); //Add square* to vector
            squares_to_occupy[l]->set_tile(*players_tiles.get_tile(best_perm[l])); //Assign the tile to the square
            players_tiles.remove_tile(*players_tiles.get_tile(best_perm[l])); //Remove from rack
        }
        score += best_score; //Score has already been calculated, so just add it
        return true; //Successfully played a word
    } else {
        return false; //Could not find a word that scores succifient points
    }
}

bool comp_player::exchange_tiles(bag &_bag)
{   
    //Exchanges tiles for the computer, using some rules that are generally useful in scrabble:
    //Hold onto high scoring tiles such as X (paired with E), Q (paired with U), Z, J, K etc..
    //Keep at least 1 vowel
    if(_bag.get_tiles_left() < 7) {
        std::cout << "Not enough tiles to exchange, Computer is passing\n";
        return false; //Pass turn
    }

    std::string available_letters{players_tiles.get_available_letters()};
    std::string letters_to_return{available_letters};
    std::string temp;
    std::string::iterator it;

    std::cout << "Computer is returning some tiles...\n";

    //Keep Q X Z J and K
    if(letters_to_return.find('Q') != std::string::npos) {letters_to_return.erase(letters_to_return.find('Q'), 1); }
    if(letters_to_return.find('X') != std::string::npos) {letters_to_return.erase(letters_to_return.find('X'), 1);}
    if(letters_to_return.find('Z') != std::string::npos) {letters_to_return.erase(letters_to_return.find('Z'), 1);}
    if(letters_to_return.find('J') != std::string::npos) {letters_to_return.erase(letters_to_return.find('J'), 1);}
    if(letters_to_return.find('K') != std::string::npos) {letters_to_return.erase(letters_to_return.find('K'), 1);}

    for(it = letters_to_return.begin(); it != letters_to_return.end(); it++) {
        _bag.add_tile(*players_tiles.get_tile(*it)); //Move to bag
        players_tiles.remove_tile(*players_tiles.get_tile(*it)); //Remove from rack
    }
    players_tiles.fill_rack(_bag); //Draw new tiles
    return false; //Pass
}