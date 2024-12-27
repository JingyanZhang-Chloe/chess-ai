#include <gen_move_fn.h>
#include <move_t.h>
#include <piece_kind.h>
#include <iostream>

using namespace engine;

int main(){

    board_t board;

    std::string coor = "d1";

    std::vector<engine::move_t> v = gen_moves<engine::piece_kind::queen> (coor, player_color::white, board);

    for(move_t move : v){
        std::cout << move << std::endl;
    };

    
    return 0;
}