#pragma once
#include <move_t.h>
#include <piece_t.h>

namespace engine
{

struct move_info_t {
    move_t move;
    std::optional<move_t> last_move;
    std::optional<piece_t> eaten_piece;
    bool white_king_or_left_rook_moved;
	bool white_king_or_right_rook_moved;
	bool black_king_or_left_rook_moved;
	bool black_king_or_right_rook_moved;
    int turns_since_capture_or_pawn_move;

    move_info_t(move_t move, bool white_king_or_left_rook_moved, bool white_king_or_right_rook_moved, bool black_king_or_left_rook_moved, bool black_king_or_right_rook_moved, int turns_since_capture_or_pawn_move);
};


} // namespace engine

