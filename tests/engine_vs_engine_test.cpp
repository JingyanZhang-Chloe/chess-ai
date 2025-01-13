#include <iostream>
#include <chrono>
#include "move_t.h"
#include "board_t.h"
#include "search_t.h"
#include "piece_t.h"
#include <vector>
#include <optional>

using namespace engine;

int main() {
    engine::board_t board;
    int delai_for_move_allowed = 10000; // millieseconds
    int max_moves = 80; // 40 is the average number of full move during a chess game (20 each)
    bool found = false; // to check if the move is legal

    std::cout<< "Start test\n\n";

    for(int i = 1; i<=max_moves; i++){
        std::cout << board;
        std::vector<engine::move_t> legal_moves = board.legal_moves();
        auto starting_time = std::chrono::steady_clock::now();    // mesuring time for the engine to output a move
        engine::move_t move = legal_moves[0];   // needs to be changed by our search
        auto ending_time = std::chrono::steady_clock::now();
        auto total_time_for_move = std::chrono::duration_cast<std::chrono::milliseconds>(ending_time - starting_time).count();

        std::optional<engine::piece_t> mb_piece = board.piece(move.source);  //checking if there is a piece at this square
        if (mb_piece.has_value()==false){
            std::cerr<< "Engine tried to move a piece from a square with no pieces:" << move.source <<"\n";
            return 1;
        }

        engine::piece_t piece = mb_piece.value();

        for (const auto& m : legal_moves) {   //checking if the move is legal
            if (m == move) {
                found = true;
                break;
            }
        }

        if(found == false){
            std::cerr<<"Engine tried to do an illegal move\n";
            return 1;
        }
        found = false;
        if(total_time_for_move > delai_for_move_allowed){
            std::cerr<< "The move" << i << "took more than 5 seconds\n";
            return 1;
        }
        board.make_move(move);
        std::cout << "Move " << i << ": " << move << "\n, piece: " << piece.kind<< ",\n color: " << piece.color << ",\n took " << total_time_for_move << "ms" << std::endl;
    
    }
    return 0;
}