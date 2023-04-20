#include"board.h"

void print_line()
{
    std::cout << "\t+";
    for(int x{1}; x <= 15; x++) {std::cout << "----+";}
    std::cout << std::endl;
}

board::board() //Initialises the board to contain all 225 standard scrabble squares and their respective multipliers
{
    for(int y{1}; y <= 15; y++) {
        for(int x{1}; x <= 15; x++) {
            if((x==8) && (y==8)) { //Central square

                board_squares.push_back(new square(multiplier::centre));

            } else if((x==1||x==8||x==15) && (y==1||y==8||y==15)) { //Triple word squares

                board_squares.push_back(new square(multiplier::triple_word));

            } else if((x==2||x==14) && (y==2||y==14) ||//double word squares
                    (x==3||x==13) && (y==3||y==13) ||
                    (x==4||x==12) && (y==4||y==12) ||
                    (x==5||x==11) && (y==5||y==11)) { 

                board_squares.push_back(new square(multiplier::double_word));

            } else if((x==2||x==6||x==10||x==14) && (y==2||y==6||y==10||y==14)) { //Triple letters

                board_squares.push_back(new square(multiplier::triple_letter));

            } else if(((x==3||x==7||x==9||x==13) && (y==3||y==7||y==9||y==13)) || //Double letters
                        (x==1||x==4||x==8||x==12||x==15) && (y==1||y==4||y==8||y==12||y==15)) { 

                board_squares.push_back(new square(multiplier::double_letter));

            } else {
                
                board_squares.push_back(new square(multiplier::none));
            }
        }
    }
}

board::~board()
{
    for(auto sq : board_squares) {
        delete sq; //Calls the square destructor before deleting pointer to square
    }
}

void board::show_board()
{
    std::cout << std::endl << "\t"; //Display x co-ordinates
    for(int x{1}; x <= 15; x++) {
        if(x>10){std::cout << " " << x << "  ";
        } else {std::cout << "  " << x << "  ";}
    }
    std::cout << std::endl; //Top line of board
    print_line();

    for(int y{1}; y <= 15; y++) {
        std::cout << y <<"\t|"; //Y co-ords
        for(int x{1}; x <= 15; x++) {

            square* this_square(this->get_square(x, y));

            std::cout << " ";
            if(this_square->get_character() == ' ') {                            //Checks to see if a tile exists on this square
                switch(this_square->get_premium()) {                             //Each multiplier square displays in a different colour if no tile occupies it
                case multiplier::centre :
                    std::cout << PINK << "CC" << DEFAULT;
                    break;
                case multiplier::triple_word :
                    std::cout << RED << "TW" << DEFAULT;
                    break;
                case multiplier::double_word :
                    std::cout << PINK << "DW" << DEFAULT;
                    break;
                case multiplier::triple_letter :
                    std::cout << GREEN << "TL" << DEFAULT;
                    break;
                case multiplier::double_letter :
                    std::cout << BLUE << "DL" << DEFAULT;
                    break;
                case multiplier::none :
                    std::cout << "  ";
                    break;
                }
            } else {
                std::cout << this_square->get_character() << " ";
            }
            std::cout << " |";
        }
        std::cout << std::endl;
        print_line();
    }
}

std::string board::get_horizontal_words()
{
    std::string str;
    for(int y{1}; y <= 15; y++) {
        for(int x{1}; x <= 15; x++) {
            str.push_back(this->get_square(x, y)->get_character());
            if(x == 15) str.push_back(' '); //Separate words on consecutive lines that end and start at top/bottom of board
        }
    }
    return str;
}

std::string board::get_vertical_words()
{
    std::string str;
    for(int x{1}; x <= 15; x++) {
        for(int y{1}; y <= 15; y++) {
            str.push_back(this->get_square(x, y)->get_character());
            if(y == 15) str.push_back(' '); //Separate words on consecutive lines that end and start at top/bottom of board
        }
    }
    return str;
}

square* board::get_square(int x, int y)
{
    if(x <= 15 && x >= 1 && y <= 15 && y >= 1) {
        return board_squares[(x-1) + (y-1)*15];
    } else {
        std::cout << "Error: Square must be on board";
        return nullptr;
    }
}