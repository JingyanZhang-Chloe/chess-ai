#include <move_info_t.h>

using namespace engine;

// This comes from the code of Jingyan Zhang (in this team)
// but due to a wrong file manipulation, git does not show that this is a revision of
// her code.
move_info_t::move_info_t(
	move_t move, 
	std::optional<piece_t> captured_piece,
    std::optional<move_t> last_move,
	bool white_king_or_left_rook_moved, 
	bool white_king_or_right_rook_moved, 
	bool black_king_or_left_rook_moved, 
	bool black_king_or_right_rook_moved, 
	int turns_since_capture_or_pawn_move,
	std::optional<bool> is_check,
	std::optional<bool> is_draw,
	std::optional<std::vector<move_t>> pseudolegal_moves,
	std::optional<cardboard_t> current_hash
) : move{move}, 
	captured_piece{captured_piece},
	last_move{last_move},
	white_king_or_left_rook_moved{white_king_or_left_rook_moved},
	white_king_or_right_rook_moved{white_king_or_right_rook_moved}, 
	black_king_or_left_rook_moved{black_king_or_left_rook_moved},
	black_king_or_right_rook_moved{black_king_or_right_rook_moved},
	turns_since_capture_or_pawn_move{turns_since_capture_or_pawn_move},
	is_check{is_check},
	is_draw{is_draw},
	pseudolegal_moves{pseudolegal_moves},
	current_hash{current_hash}
{};
