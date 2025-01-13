#include <iostream>
#include <optional>
#include <cmath>
#include <board_t.h>
#include <piece_t.h>
#include <player_color.h>
#include <piece_kind.h>
#include <gen_move_fn.h>


using namespace engine;

// Reviewed, tested
board_t::board_t() 
	: _turn_color { player_color::white }, 
	white_king_or_left_rook_moved { false },
	white_king_or_right_rook_moved { false },
	black_king_or_left_rook_moved { false },
	black_king_or_right_rook_moved { false },
	__piece_count{{8, 2, 2, 2, 1, 1, 8, 2, 2, 2, 1, 1}},
	turns_since_capture_or_pawn_move{0}
{
	// Add pawns
	for (int column = 0; column < 8; column++) {
		this->piece({ 1, column }) = piece_t{ piece_kind::pawn, player_color::white };
		this->piece({ 6, column }) = piece_t{ piece_kind::pawn, player_color::black };
	}

	// Add rooks
	this->piece({ 0, 0 }) = piece_t{ piece_kind::rook, player_color::white };
	this->piece({ 0, 7 }) = piece_t{ piece_kind::rook, player_color::white };

	this->piece({ 7, 0 }) = piece_t{ piece_kind::rook, player_color::black };
	this->piece({ 7, 7 }) = piece_t{ piece_kind::rook, player_color::black };

	// Add knights
	this->piece({ 0, 1 }) = piece_t{ piece_kind::knight, player_color::white };
	this->piece({ 0, 6 }) = piece_t{ piece_kind::knight, player_color::white };

	this->piece({ 7, 1 }) = piece_t{ piece_kind::knight, player_color::black };
	this->piece({ 7, 6 }) = piece_t{ piece_kind::knight, player_color::black };

	// Add bishops
	this->piece({ 0, 2 }) = piece_t{ piece_kind::bishop, player_color::white };
	this->piece({ 0, 5 }) = piece_t{ piece_kind::bishop, player_color::white };

	this->piece({ 7, 2 }) = piece_t{ piece_kind::bishop, player_color::black };
	this->piece({ 7, 5 }) = piece_t{ piece_kind::bishop, player_color::black };

	// Add queens
	this->piece({ 0, 3 }) = piece_t{ piece_kind::queen, player_color::white };
	this->piece({ 7, 3 }) = piece_t{ piece_kind::queen, player_color::black };

	// Add kings
	this->piece({ 0, 4 }) = piece_t{ piece_kind::king, player_color::white };
	this->piece({ 7, 4 }) = piece_t{ piece_kind::king, player_color::black };

	this->position_count.insert({ this->to_bitset(), 1 });
}

// Reviewed
move_info_t board_t::make_move(move_t move) {

	move_info_t info = get_move_info(move);

	this->turns_since_capture_or_pawn_move++;

	// Update castling-related information if rook or king is moving
	if (this->piece(move.source).value().kind == piece_kind::king) {
		if (this->_turn_color == player_color::white) {
			this->white_king_or_left_rook_moved = true;
			this->white_king_or_right_rook_moved = true;
		}

		else {
			this->black_king_or_left_rook_moved = true;
			this->black_king_or_right_rook_moved = true;
		}
	}

	if (this->piece(move.source).value().kind == piece_kind::rook) {
		if (this->_turn_color == player_color::white) {
			if (move.source.column() == 0)
				this->white_king_or_left_rook_moved = true;
			else if (move.source.column() == 7)
				this->white_king_or_right_rook_moved = true;
		}
		else {
			if (move.source.column() == 0)
				this->black_king_or_left_rook_moved = true;
			else if (move.source.column() == 7)
				this->black_king_or_right_rook_moved = true;
		}
	}

	// Update 50-move-rule-related information
	if (this->piece(move.source).value().kind == piece_kind::pawn)
		this->turns_since_capture_or_pawn_move = 0;

	// Update destination piece
	// First handle the case where the king is castling
	if (this->piece(move.source).value().kind == piece_kind::king
		&& std::abs(move.destination.column() - move.source.column()) > 1) {
		// We move the rook, the king movement gets handled normally after
		int castle_direction = move.destination.column() - move.source.column();
		int rook_column = castle_direction == 1 ? 7 : 0;
		int new_rook_column = castle_direction == 1 ? 5 : 3;
		
		this->piece({ move.source.row(), rook_column }) = std::nullopt;
		this->piece({ move.source.row(), new_rook_column }) 
			= piece_t{ piece_kind::rook, this->_turn_color };
	}
	// Then handle the case for promotion
	if (move.promotion_code.has_value()) {
		this->_piece_count(this->_turn_color, move.promotion_code.value())++;
		this->_piece_count(this->_turn_color, piece_kind::pawn)--;

		this->piece(move.destination) = piece_t{
			move.promotion_code.value(),
			this->_turn_color
		};
	}
	// Finally, the case for a normal move
	else {
		if (this->piece(move.destination).has_value()) {
			this->_piece_count(
				player_color_fn::opposite(this->_turn_color), 
				this->piece(move.destination).value().kind
			)--;

			this->turns_since_capture_or_pawn_move = 0;
		}

		this->piece(move.destination) = this->piece(move.source);
	}

	// Update source piece
	this->piece(move.source) = std::nullopt;
	
	// Update game-state-related information
	this->_turn_color = player_color_fn::opposite(this->_turn_color);
	this->_latest_move = move;
	
	hash_t current_hash = this->to_bitset();

	if (this->position_count.contains(current_hash))
		this->position_count[current_hash]++;
	else 
		this->position_count.insert({ current_hash, 0 });

	return info;
}

// Reviewed:
// Move out the massive switch into its own function
// Remove inserts by having the gen functions directly inserting by reference
std::vector<move_t> board_t::legal_moves() const { 
	std::vector<move_t> legal_moves;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			chess_coordinate_t coord { row, col };
			const auto& mb_piece = this->piece(coord);

			if (mb_piece.has_value() && mb_piece.value().color == this->_turn_color) {
				piece_t p = mb_piece.value();

				std::vector<move_t> p_moves = gen_moves(p.kind, coord, this->_turn_color, *this);
				legal_moves.insert(legal_moves.end(), p_moves.begin(), p_moves.end());
			}
		}
	}

	return legal_moves;
}

// Reviewed
std::optional<move_t> board_t::latest_move() const {
	return this->_latest_move;
}

// Reviewed
std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) {
	return this->pieces[coord.index()];
}

// Reviewed
const std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) const {
	return this->pieces[coord.index()];
}

bool board_t::is_attacked(chess_coordinate_t coord) const {
	player_color opp_color = player_color_fn::opposite(this->_turn_color);

	for (auto kind : { antipiece_kind::pawn, antipiece_kind::knight, 
		antipiece_kind::bishop, antipiece_kind::rook, antipiece_kind::queen, 
		antipiece_kind::king })
	for (move_t move : gen_moves(kind, coord, opp_color, *this))
	if (
		this->piece(move.destination).has_value()
		&& this->piece(move.destination).value().kind 
			== piece_kind_fn::to_piece_kind(kind)
	) return true;

	return false;
}

// Reviewed
int board_t::piece_count() const {
	return this->piece_count(player_color::white) + this->piece_count(player_color::black);
}

// Reviewed
int board_t::piece_count(player_color color) const {
	int re = 0;

	for (auto kind : { piece_kind::pawn, piece_kind::knight, piece_kind::bishop,
		piece_kind::rook, piece_kind::queen, piece_kind::king })
		re += this->piece_count(color, kind);

	return re;
}

// Reviewed
int board_t::piece_count(piece_kind kind) const {
	int re = 0;

	for (auto color: { player_color::white, player_color::black })
		re += this->piece_count(color, kind);

	return re;
}

// Reviewed
int board_t::piece_count(player_color color, piece_kind kind) const {
	return this->_piece_count(color, kind);
}

// Reviewed
std::optional<chess_coordinate_t> board_t::king_coordinates(player_color color) const {
	for (int row = 0; row < 8; row++) {
		for (int column = 0; column < 8; column++) {
			std::optional<piece_t> piece = this->piece({ row, column });

			if (piece.has_value() && piece.value().color == color
				&& piece.value().kind == piece_kind::king)
				return chess_coordinate_t{ row, column };
		}
	}

	return std::nullopt;
}

// Reviewed
player_color board_t::turn_color() const {
	return this->_turn_color;
}

// Reviewed
std::optional<player_color> board_t::winning_player() const {
	if(!king_coordinates(player_color::white).has_value()) {
		return player_color::black;
	}

	if(!king_coordinates(player_color::black).has_value()) {
		return player_color::white;
	}

	return std::nullopt;
}

bool board_t::can_castle(board_t::side side) const {
	int castle_row;
	bool king_or_rook_moved;

	if (this->_turn_color == player_color::white) {
		castle_row = 0;

		if (side == board_t::side::left)
			king_or_rook_moved = this->white_king_or_left_rook_moved;
		else
			king_or_rook_moved = this->white_king_or_right_rook_moved;
	}
	else {
		castle_row = 7;

		if (side == board_t::side::left)
			king_or_rook_moved = this->black_king_or_left_rook_moved;
		else
			king_or_rook_moved = this->black_king_or_right_rook_moved;
	}

	if (king_or_rook_moved) return false;
	
	if (this->is_check()) return false;

	// We check if all pieces between the king and the rook are empty
	// Then we check if the the king will castle through check
	if (side == board_t::side::left) {
		for (int column = 3; column >= 1; column--)
		if (this->piece({ castle_row, column }).has_value()) return false;

		for (int column = 3; column >= 2; column--)
		if (this->is_attacked({ castle_row, column })) return false;
	}
	else {
		for (int column = 5; column <= 6; column++)
		if (this->piece({ castle_row, column }).has_value()) return false;

		for (int column = 5; column <= 6; column++)
		if (this->is_attacked({ castle_row, column })) return false;
	}

	return true;
}

// Reviewed
bool board_t::is_check() const {
	std::optional<chess_coordinate_t> king_coords = 
		this->king_coordinates(this->turn_color());
	
	return king_coords.has_value() && this->is_attacked(king_coords.value());
}

// TODO:  Serious optimization needed here
bool board_t::is_draw() const {
	// Case 1 : Stalemate
	if (this->legal_moves().size() == 0) return true;

	int __piece_count = this->piece_count();
	// Case 2 : Dead Position
	// King vs. King: Only two kings are left
	if (__piece_count == this->piece_count(piece_kind::king)) return true;
	
	//King and Bishop vs. King: A king with one bishop cannot checkmate
	if (__piece_count == 3
		&& this->piece_count(piece_kind::king) == 2
		&& this->piece_count(piece_kind::bishop) == 1
	) return true;

	//King and Knight vs. King: A king with one knight cannot checkmate
	if (__piece_count == 3
		&& this->piece_count(piece_kind::king) == 2
		&& this->piece_count(piece_kind::bishop) == 1
	) return true;

	// Case 3 : Threefold Repetition
	hash_t current_hash = this->to_bitset();

	if (this->position_count.contains(current_hash)
		&& this->position_count.at(current_hash) >= 3) return true;

	// Case 4 : 50-Move Rule
	if (this->turns_since_capture_or_pawn_move >= 50) return true;

	return false;
}

float board_t::score() const {
	std::optional<player_color> _winning_player = this->winning_player();
	if (_winning_player.has_value()) {
		if (_winning_player.value() == this->turn_color())
			return std::numeric_limits<float>::infinity();
		else
			return -std::numeric_limits<float>::infinity();
	}

	if (this->is_draw()) return 0;

	float score = 0;
	for (auto kind : { piece_kind::pawn, piece_kind::knight, piece_kind::bishop,
		piece_kind::rook, piece_kind::queen })
		score += piece_kind_fn::get_score(kind) * (
			this->piece_count(player_color::white, kind)
			- this->piece_count(player_color::black, kind)
		);

	return score;
}

int& board_t::_piece_count(player_color color, piece_kind kind) {
	return this->__piece_count[static_cast<int>(color) * 6 + static_cast<int>(kind)];
}

const int& board_t::_piece_count(player_color color, piece_kind kind) const {
	return this->__piece_count[static_cast<int>(color) * 6 + static_cast<int>(kind)];
}

// Reviewed, tested
std::ostream& operator << (std::ostream& os, const board_t& board) {
	os << "\n";

	for (int row = 7; row >= 0; row--) {
		// Display separator
		for (int column = 0; column < 8; column++) {
			os << "----";
		}
		
		os << "-\n";

		// Display top padding
		for (int column = 0; column < 8; column++) {
			os << "|   ";
		}
		
		os << "|\n";
		
		// Display row
		for (int column = 7; column >= 0; column--) {
			os << "| ";
			auto piece = board.piece({ row, column });

			if (piece.has_value()) {
				piece_kind kind = piece.value().kind;
				player_color color = piece.value().color;

				if (color == player_color::black) {
					os << "\033[31m";
				}

				switch (kind) {
					case piece_kind::pawn:
						os << "p";
						break;
					case piece_kind::knight:
						os << "n";
						break;
					case piece_kind::bishop:
						os << "b";
						break;
					case piece_kind::rook:
						os << "r";
						break;
					case piece_kind::queen:
						os << "q";
						break;
					case piece_kind::king:
						os << "k";
						break;
					default:
						os << "?";
						break;
				}

				os << "\033[0m";
			}
			else {
				os << " ";
			}

			os << " ";
		}

		os << "|\n";

		// Display bottom padding
		for (int column = 0; column < 8; column++) {
			os << "|   ";
		}
		
		os << "|\n";
	}
	
	return os;
};

std::bitset<265> board_t::to_bitset() const {   //do not consider en passant state
	std::bitset<265> key;
	std::size_t bit = 0;
	for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
			chess_coordinate_t coord { row, col };
			const auto& mb_piece = this->piece(coord);
			if (mb_piece.has_value()) {
				piece_t current_piece = mb_piece.value();
				int color_hash;
				int piece_kind_hash = 0;
				if (current_piece.color == player_color::white) {
    				color_hash = 0;
				} else {
    				color_hash = 6;
				}
				switch(current_piece.kind) {
					case piece_kind::pawn:
						piece_kind_hash = 1; 
						break;
                    case piece_kind::knight:
						piece_kind_hash = 2; 
						break;
                    case piece_kind::bishop: 
						piece_kind_hash = 3; 
						break;
                    case piece_kind::rook:   
						piece_kind_hash = 4; 
						break;
                    case piece_kind::queen:  
						piece_kind_hash = 5; 
						break;
                    case piece_kind::king:   
						piece_kind_hash = 6; 
						break;
				}
				int piece_hash = piece_kind_hash + color_hash;
				key.set(bit + piece_hash);
				for (int b = 0; b < 4; ++b) {
                	key.set(bit + b, (piece_hash >> b) & 1);
            	}
			}
			else{
				key.set(bit + 0);
			}
			bit += 4;
		}
	}
	key.set(bit++, this->white_king_or_left_rook_moved);
    key.set(bit++, this->white_king_or_right_rook_moved);
    key.set(bit++, this->black_king_or_left_rook_moved);
    key.set(bit++, this->black_king_or_right_rook_moved);
	key.set(bit++, this->_turn_color == player_color::white);
	return key;
}

move_info_t board_t::get_move_info(move_t move){
	// the board now is in the state of before the move
	move_info_t info { move, this->white_king_or_left_rook_moved, this->white_king_or_right_rook_moved, this->black_king_or_left_rook_moved, this->black_king_or_right_rook_moved, this->turns_since_capture_or_pawn_move };
	info.last_move = this->latest_move();

	info.eaten_piece = this->piece(move.destination);

}

void board_t::unmake_move(move_info_t info){
	this->latest_move() = info.last_move;
	this->white_king_or_left_rook_moved = info.white_king_or_left_rook_moved;
	this->white_king_or_right_rook_moved = info.white_king_or_right_rook_moved;
	this->black_king_or_left_rook_moved = info.black_king_or_left_rook_moved;
	this->black_king_or_right_rook_moved = info.black_king_or_right_rook_moved;
	this->turns_since_capture_or_pawn_move = info.turns_since_capture_or_pawn_move;
}
