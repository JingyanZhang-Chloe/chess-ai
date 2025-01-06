#include <iostream>
#include <optional>
#include <board_t.h>
#include <piece_t.h>
#include <player_color.h>
#include <piece_kind.h>
#include "gen_move_fn.h"

using namespace engine;

board_t::castling_rights_t::castling_rights_t() : left_black{ false }, left_white{ false }, right_black{false}, right_white{false} {};

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
};

std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) {
	return this->pieces[coord.index()];
};

const std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) const {
	return this->pieces[coord.index()];
};

board_t& board_t::make_move(move_t move) {
	// Push current state to history
	board_t current = *this;
	this->history.push(current);
	
	this->piece(move.destination) = this->piece(move.source);
	this->piece(move.source) = std::nullopt;

	return *this;
};

board_t& board_t::unmake_latest_move() {
	board_t previous = this->history.top();
	this->history.pop();

	*this = previous;

	return *this;
};

std::vector<move_t> board_t::get_legal_moves() const { 
	std::vector<move_t> legal_moves;
	player_color turn = player_color::white; //i just put this for now and i will change it when the board gives the color of the player turn
	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			chess_coordinate_t coord(row, col);
			const auto& mb_piece = this->piece(coord);
			if(mb_piece.has_value()==false){
				continue;
			}else{
				piece_t p = mb_piece.value();
				if(p.color == turn){
					switch(p.kind){
						case piece_kind::rook: {
							auto rook_moves = gen_moves<engine::piece_kind::rook>(coord, turn, *this); 
							legal_moves.insert(legal_moves.end(), rook_moves.begin(), rook_moves.end());
                    		break;
						}
						case piece_kind::bishop: {
							auto bishop_moves = gen_moves<engine::piece_kind::bishop>(coord, turn, *this);
							legal_moves.insert(legal_moves.end(), bishop_moves.begin(), bishop_moves.end());
                    		break;
						}
						case piece_kind::queen: {
							auto queen_moves = gen_moves<engine::piece_kind::queen>(coord, turn, *this);
							legal_moves.insert(legal_moves.end(), queen_moves.begin(), queen_moves.end());
                    		break;
						}
						case piece_kind::knight: {
							auto knight_moves = gen_moves<engine::piece_kind::knight>(coord, turn, *this);
							legal_moves.insert(legal_moves.end(), knight_moves.begin(), knight_moves.end());
                    		break;
						}
						case piece_kind::king: {
							auto king_moves = gen_moves<engine::piece_kind::king>(coord, turn, *this);
							legal_moves.insert(legal_moves.end(), king_moves.begin(), king_moves.end());
                    		break;
						}
						case piece_kind::pawn: {
							auto pawn_moves = gen_moves<engine::piece_kind::pawn>(coord, turn, *this);
							legal_moves.insert(legal_moves.end(), pawn_moves.begin(), pawn_moves.end());
                    		break;
						}
					}
				}
			}
		}
	}
	return legal_moves;
};

chess_coordinate_t board_t::king_coordinates(player_color color) const {
	for (int row = 0; row < 8; row++) {
		for (int column = 0; column < 8; column++) {
			std::optional<piece_t> piece = this->piece({ row, column });

			if (piece.has_value()) {
				if (piece.value().color == color
						&& piece.value().kind == piece_kind::king) {
					return { row, column };
				}
			}
		}
	}

	throw "[Board Error] Less than 2 kings on the board.";
}

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
