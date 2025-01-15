#include <move_info_t.h>

using namespace engine;

move_info_t::move_info_t(
	move_t move, 
	std::optional<piece_t> captured_piece,
    std::optional<move_t> last_move,
	bool white_king_or_left_rook_moved, 
	bool white_king_or_right_rook_moved, 
	bool black_king_or_left_rook_moved, 
	bool black_king_or_right_rook_moved, 
	int turns_since_capture_or_pawn_move
) : move{move}, 
	captured_piece{captured_piece},
	last_move{last_move},
	white_king_or_left_rook_moved{white_king_or_left_rook_moved},
	white_king_or_right_rook_moved{white_king_or_right_rook_moved}, 
	black_king_or_left_rook_moved{black_king_or_left_rook_moved},
	black_king_or_right_rook_moved{black_king_or_right_rook_moved},
	turns_since_capture_or_pawn_move{turns_since_capture_or_pawn_move} {};
