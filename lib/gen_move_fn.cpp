#include <gen_move_fn.h>
#include <chess_coordinate_t.h>
#include <player_color.h>
#include <board_t.h>
#include <vector>
#include <array>

using namespace engine;

// Reviewed
bool is_coordinate_valid(int row, int col) {
	return 0 <= row && row < 8 && 0 <= col && col < 8;
}

// Reviewed
// Returns whether we should stop (blocked by a piece)
bool add_if_empty_or_opponent(
	chess_coordinate_t source,
	chess_coordinate_t destination, 
	player_color current_player, 
	const board_t& board,
	std::vector<move_t>& moves
) {
	auto piece = board.piece(destination);

	if (piece.has_value()) {
		if (piece.value().color != current_player) 
			moves.push_back({ source, destination }); 

		return true;
	}
	else {
		moves.push_back({ source, destination });

		return false;
	}
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::rook>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
    std::vector<move_t> re;
	
	// Loop right
    for (int column = source.column() + 1; column < 8; column++) {
        chess_coordinate_t destination { source.row(), column };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

    // Loop left
    for (int column = source.column() - 1; column >= 0; column--) {
        chess_coordinate_t destination { source.row(), column };
		
		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

    // Loop down
    for (int row = source.row() - 1; row >= 0; row--) {
        chess_coordinate_t destination { row, source.column() };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

	// Loop up
    for (int row = source.row() + 1; row < 8; row++) {
        chess_coordinate_t destination { row, source.column() };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

    return re;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::bishop>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
    std::vector<move_t> re;
	
	// Loop south-west
	for (int k = 1; source.row() - k >= 0 && source.column() - k >= 0; k++) {
		chess_coordinate_t destination { source.row() - k, source.column() - k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

	// Loop north-west
	for (int k = 1; source.row() + k < 8 && source.column() - k >= 0; k++) {
		chess_coordinate_t destination { source.row() + k, source.column() - k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

	// Loop south-east
	for (int k = 1; source.row() - k >= 0 && source.column() + k < 8; k++) {
		chess_coordinate_t destination { source.row() - k, source.column() + k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

	// Loop north-east
	for (int k = 1; source.row() + k < 8 && source.column() + k < 8; k++) {
		chess_coordinate_t destination { source.row() + k, source.column() + k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

    return re;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::knight>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	using listlist = std::array<std::pair<int, int>, 2>;

	std::vector<move_t> re;

	for (int dx : { -1, 1 }) for (int dy : { -1, 1 }) 
	for (auto [x_toggle, y_toggle] : listlist{{{ 0, 1 }, { 1, 0 }}}) {
		int row = source.row() + dy + dy * y_toggle;
		int column = source.column() + dx + dx * x_toggle;

		if (0 <= row && row < 8 && 0 <= column && column < 8)
			add_if_empty_or_opponent(source, { row, column }, color, board, re);
	}

	return re;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::queen>(
	chess_coordinate_t coord, player_color color, const board_t& board
) {
    std::vector v1 = gen_moves<piece_kind::bishop>(coord, color, board);
    std::vector v2 = gen_moves<piece_kind::rook>(coord, color, board);

    v1.insert(v1.end(), v2.begin(), v2.end());

    return v1;
}

std::vector<move_t> pawn_noncapturing_advances(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	std::vector<move_t> re;

	int initial_row = 1;
	int y_direction = 1;
	int promoting_row = 7;

	if (color == player_color::black) {
		initial_row = 6;
		y_direction = -1;
		promoting_row = 0;
	}
	
	int max_walk_distance = source.row() == initial_row ? 2 : 1;

	for (int walk_distance = 1; walk_distance <= max_walk_distance; walk_distance++) {
		chess_coordinate_t destination { 
			source.row() + y_direction * walk_distance,
			source.column()
		};

		if (board.piece(destination).has_value()) break;
		
		if (destination.row() == promoting_row)
		for (piece_kind promotion_code : { 
			piece_kind::knight, piece_kind::bishop, 
			piece_kind::rook, piece_kind::queen 
		}) 
		re.push_back({ source, destination, promotion_code });
		
		else re.push_back({ source, destination });
	}

	return re;
}

// Reviewed
std::vector<move_t> pawn_simple_captures(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	int y_direction = 1;
	int promoting_row = 7;

	if (color == player_color::black) {
		y_direction = -1;
		promoting_row = 0;
	}
	
	std::vector<move_t> capturing_advances;
	capturing_advances.reserve(2);

	for (int dx : { -1, 1 }) {
		int new_column = source.column() + dx;

		if (0 <= new_column && new_column < 8) {
			chess_coordinate_t advance { source.row() + y_direction, new_column };
			
			std::optional<piece_t> dest_piece = board.piece(advance);

			if (dest_piece.has_value() && dest_piece.value().color 
				== player_color_fn::opposite(color))
				capturing_advances.push_back({ source, advance });
		}
	}

	if (source.row() + y_direction == promoting_row) {
		std::vector<move_t> re;
		re.reserve(capturing_advances.size() * 4);

		for (const move_t& move : capturing_advances) 
		for (piece_kind promotion_code : { 
			piece_kind::knight, piece_kind::bishop, 
			piece_kind::rook, piece_kind::queen 
		})
			re.push_back({ move.source, move.destination, promotion_code });

		return re;
	}
	else return capturing_advances;
}

std::vector<move_t> pawn_en_passant(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	int y_direction = 1;
	int en_passant_row = 4;
	
	if (color == player_color::black) {
		y_direction = -1;
		en_passant_row = 3;
	}

	if (source.row() != en_passant_row) return {};
	if (!board.latest_move().has_value()) return {};

	move_t last_move = board.latest_move().value();

	if (board.piece(last_move.destination).value().kind != piece_kind::pawn)
		return {};

	int walk_distance = (last_move.destination.row() - last_move.source.row()) 
		* y_direction;
	
	if (walk_distance != 2) return {};

	int column_displacement = source.column() - last_move.destination.column();
	
	if (column_displacement != 1 && column_displacement != -1) return {};

	return {{{ 
		source, 
		{ last_move.destination.row() + y_direction, last_move.destination.column() }
	}}};
}

template<>
std::vector<move_t> engine::gen_moves<piece_kind::pawn>(
	chess_coordinate_t coord, player_color color, const board_t& board
) {
	std::vector<move_t> v1 = pawn_noncapturing_advances(coord, color, board);
	
	std::vector<move_t> v2 = pawn_simple_captures(coord, color, board);
	v1.insert(v1.end(), v2.begin(), v2.end());

	std::vector<move_t> v3 = pawn_en_passant(coord, color, board);
	v1.insert(v1.end(), v2.begin(), v2.end());

	return v1;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::king>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	std::vector<move_t> re;
	
	// Simple movements
	for (int dx : { -1, 0, 1 }) for (int dy : { -1, 0, 1 })
	if (dx != 0 || dy != 0) {
		int dest_row = source.row() + dy;
		int dest_col = source.column() + dx;

		if (is_coordinate_valid(dest_row, dest_col))
			add_if_empty_or_opponent(
				source, { dest_row, dest_col }, color, board, re
			);
	}

	// Castling
	if (board.can_castle(board_t::side::left))
		re.push_back({ source, { color == player_color::white ? 0 : 7, 2 }});

	if (board.can_castle(board_t::side::right))
		re.push_back({ source, { color == player_color::white ? 0 : 7, 6 }});

	return re;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<antipiece_kind::rook>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	return gen_moves<piece_kind::rook>(source, player_color_fn::opposite(color), board);
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<antipiece_kind::bishop>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	return gen_moves<piece_kind::bishop>(source, player_color_fn::opposite(color), board);
}

template<>
std::vector<move_t> engine::gen_moves<antipiece_kind::queen>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	return gen_moves<piece_kind::queen>(source, player_color_fn::opposite(color), board);
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<antipiece_kind::knight>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	return gen_moves<piece_kind::knight>(source, player_color_fn::opposite(color), board);
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<antipiece_kind::king>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	color = player_color_fn::opposite(color);

	std::vector<move_t> re;

	for (int dx : { -1, 0, 1 }) for (int dy : { -1, 0, 1 })
	if (dx != 0 || dy != 0) {
		int dest_row = source.row() + dy;
		int dest_col = source.column() + dx;

		if (is_coordinate_valid(dest_row, dest_col))
			add_if_empty_or_opponent(
				source, { dest_row, dest_col }, color, board, re
			);
	}

	return re;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<antipiece_kind::pawn>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	color = player_color_fn::opposite(color);

	int y_direction = color == player_color::white ? 1 : -1;
	int new_row = source.row() + y_direction;

	if (new_row < 0 || new_row >= 8) return {};	

	std::vector<move_t> re;

	for (int dx : { -1, 1 }) {
		int new_column = source.column() + dx;

		if (0 <= new_column && new_column < 8) {
			chess_coordinate_t anticapturing_backstep { new_row, new_column };
			add_if_empty_or_opponent(source, anticapturing_backstep, color, board, re);
		}
	}

	return re;
}

// Reviewed
std::vector<move_t> engine::gen_moves(
	piece_kind kind, chess_coordinate_t source, player_color color, const board_t& board
) {
	switch (kind) {
	case piece_kind::pawn:
		return engine::gen_moves<piece_kind::pawn>(source, color, board);
	case piece_kind::knight:
		return engine::gen_moves<piece_kind::knight>(source, color, board);
	case piece_kind::bishop:
		return engine::gen_moves<piece_kind::bishop>(source, color, board);
	case piece_kind::rook:
		return engine::gen_moves<piece_kind::rook>(source, color, board);
	case piece_kind::queen:
		return engine::gen_moves<piece_kind::queen>(source, color, board);
	case piece_kind::king:
		return engine::gen_moves<piece_kind::king>(source, color, board);
	}
}

// Reviewed
std::vector<move_t> engine::gen_moves(
	antipiece_kind kind, chess_coordinate_t source, player_color color, const board_t& board
) {
	switch (kind) {
	case antipiece_kind::pawn:
		return engine::gen_moves<antipiece_kind::pawn>(source, color, board);
	case antipiece_kind::knight:
		return engine::gen_moves<antipiece_kind::knight>(source, color, board);
	case antipiece_kind::bishop:
		return engine::gen_moves<antipiece_kind::bishop>(source, color, board);
	case antipiece_kind::rook:
		return engine::gen_moves<antipiece_kind::rook>(source, color, board);
	case antipiece_kind::queen:
		return engine::gen_moves<antipiece_kind::queen>(source, color, board);
	case antipiece_kind::king:
		return engine::gen_moves<antipiece_kind::king>(source, color, board);
	}
}
