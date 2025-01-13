#include "move_info_t.h"

using namespace engine;


move_info_t::move_info_t(move_t move, bool white_king_or_left_rook_moved, bool white_king_or_right_rook_moved, bool black_king_or_left_rook_moved, bool black_king_or_right_rook_moved, int turns_since_capture_or_pawn_move) : move{move} {
    this->white_king_or_left_rook_moved = white_king_or_left_rook_moved;
    this->white_king_or_right_rook_moved = white_king_or_right_rook_moved;
    this->black_king_or_left_rook_moved = black_king_or_left_rook_moved;
    this->black_king_or_right_rook_moved = black_king_or_right_rook_moved;
    this->turns_since_capture_or_pawn_move = turns_since_capture_or_pawn_move;
}