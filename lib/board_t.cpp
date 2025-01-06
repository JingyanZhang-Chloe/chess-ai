#include <iostream>
#include <optional>
#include <board_t.h>
#include <piece_t.h>
#include <player_color.h>
#include <piece_kind.h>
#include <gen_move_fn.h>

using namespace engine;

// Reviewed
board_t::castling_rights_t::castling_rights_t() : left_black{ false }, left_white{ false }, right_black{ false }, right_white{ false } {}

// Reviewed, tested
board_t::board_t() {
	// Add pawns
	for (int column = 0; column < 8; column++) {
		this->piece({ 1, column }) = piece_t{ piece_kind::pawn, player_color::white };
		this->piece({ 6, column }) = piece_t{ piece_kind::pawn, player_color::black };
	};

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
}

// Reviewed
std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) {
	return this->pieces[coord.index()];
}

// Reviewed
const std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) const {
	return this->pieces[coord.index()];
}

// Reviewed:
// Possibly update game state here
board_t& board_t::make_move(move_t move) {
	// Push current state to history
	board_t current = *this;
	this->history.push(current);
	
	if (move.promotion_code.has_value())
		this->piece(move.destination) = piece_t {
			move.promotion_code.value(),
			this->turn_color
		};
	else
		this->piece(move.destination) = this->piece(move.source);

	this->piece(move.source) = std::nullopt;

	this->turn_color = player_color_fn::opposite(this->turn_color);

	return *this;
}

// Reviewed
board_t& board_t::unmake_latest_move() {
	board_t previous = this->history.top();
	this->history.pop();

	*this = previous;

	return *this;
}

// Reviewed:
// Move out the massive switch into its own function
// Remove inserts by having the gen functions directly inserting by reference
std::vector<move_t> board_t::get_legal_moves() const { 
	std::vector<move_t> legal_moves;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			chess_coordinate_t coord { row, col };
			const auto& mb_piece = this->piece(coord);

			if (mb_piece.has_value() && mb_piece.value().color == this->turn_color) {
				piece_t p = mb_piece.value();
				std::vector<move_t> p_moves;

				switch (p.kind) {
				case piece_kind::rook:
					p_moves = gen_moves<engine::piece_kind::rook>(coord, this->turn_color, *this);
					legal_moves.insert(legal_moves.end(), p_moves.begin(), p_moves.end());
					break;
				
				case piece_kind::bishop:
					p_moves = gen_moves<engine::piece_kind::bishop>(coord, this->turn_color, *this);
					legal_moves.insert(legal_moves.end(), p_moves.begin(), p_moves.end());
					break;
				
				case piece_kind::queen: 
					p_moves = gen_moves<engine::piece_kind::queen>(coord, this->turn_color, *this);
					legal_moves.insert(legal_moves.end(), p_moves.begin(), p_moves.end());
					break;
				
				case piece_kind::knight: 
					p_moves = gen_moves<engine::piece_kind::knight>(coord, this->turn_color, *this);
					legal_moves.insert(legal_moves.end(), p_moves.begin(), p_moves.end());
					break;
				
				case piece_kind::king: 
					p_moves = gen_moves<engine::piece_kind::king>(coord, this->turn_color, *this);
					legal_moves.insert(legal_moves.end(), p_moves.begin(), p_moves.end());
					break;
				
				case piece_kind::pawn:
					p_moves = gen_moves<engine::piece_kind::pawn>(coord, this->turn_color, *this);
					legal_moves.insert(legal_moves.end(), p_moves.begin(), p_moves.end());
					break;
				}
			}
		}
	}

	return legal_moves;
}

// Reviewed
std::optional<chess_coordinate_t> board_t::king_coordinates(player_color color) const {
	for (int row = 0; row < 8; row++) {
		for (int column = 0; column < 8; column++) {
			std::optional<piece_t> piece = this->piece({ row, column });

			if (piece.has_value()) {
				if (piece.value().color == color
					&& piece.value().kind == piece_kind::king
				) {
					return chess_coordinate_t{ row, column };
				}
			}
		}
	}

	return std::nullopt;
}

// Reviewed
std::optional<player_color> board_t::wining_player() {
	if(!king_coordinates(player_color::white).has_value()) {
		return player_color::black;
	}

	if(!king_coordinates(player_color::black).has_value()) {
		return player_color::white;
	}

	return std::nullopt;
}

// Reviewed
bool board_t::is_check(){
	// Check if the player who played the previous turn has a move that targets the king
	auto old_turn_color = this->turn_color;
	this->turn_color = player_color_fn::opposite(this->turn_color);

	std::vector<move_t> all_legal_moves = get_legal_moves();
	
	this->turn_color = old_turn_color;
	std::optional<chess_coordinate_t> king_coord = this->king_coordinates(this->turn_color);
	
	if (!king_coord.has_value()) return false;

	for (move_t move : all_legal_moves) {
		if (move.destination == king_coord.value()) return true;
	}
	
	return false;
}

int board_t::eval(){
	int my_score = 0;
	int oppo_score = 0;

	for(int row = 0; row <= 7; ++row){
		for(int column = 0; column <= 7; ++ column){
			if(this->piece({row, column}).has_value()){
				if(this->piece({row, column}).value().color == turn_color){
					my_score += 1;
				}
				else
				{
					oppo_score += 1;
				}
			}
		}
	}

	return my_score - oppo_score;
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


