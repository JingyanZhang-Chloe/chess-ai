#include <iostream>
#include <cmath>
#include <piece_kind.h> 

using namespace engine;

int piece_kind_fn::get_score(piece_kind kind) {
	switch (kind) {
	case piece_kind::pawn: return piece_kind_info<piece_kind::pawn>::value;
	case piece_kind::knight: return piece_kind_info<piece_kind::knight>::value;
	case piece_kind::bishop: return piece_kind_info<piece_kind::bishop>::value;
	case piece_kind::rook: return piece_kind_info<piece_kind::rook>::value;
	case piece_kind::queen: return piece_kind_info<piece_kind::queen>::value;
	case piece_kind::king: return std::numeric_limits<int>::max();
	}
}

int piece_kind_fn::get_position_score(
	 piece_kind kind, player_color color, chess_coordinate_t pos
) {
	int index = pos.index();
	int re = 0;

	switch (kind) {
	case piece_kind::pawn: re = piece_kind_info<piece_kind::pawn>::square_table[
		8 * (color == player_color::white ? pos.row() : (7 - pos.row())) + pos.column()
	]; break;
	case piece_kind::knight: re = piece_kind_info<piece_kind::knight>::square_table[index]; break;
	case piece_kind::bishop: re = piece_kind_info<piece_kind::bishop>::square_table[index]; break;
	case piece_kind::rook: re = piece_kind_info<piece_kind::rook>::square_table[index]; break;
	case piece_kind::queen: re = piece_kind_info<piece_kind::queen>::square_table[index]; break;
	case piece_kind::king: re = piece_kind_info<piece_kind::king>::square_table[index]; break;
	}

	return color == player_color::white ? re : -re;
}
	
// Reviewed
std::ostream& operator<<(std::ostream& os, piece_kind piece_name){
	switch (piece_name) {
	case piece_kind::pawn: return os << "p";
	case piece_kind::knight: return os << "k";
	case piece_kind::bishop: return os << "b";
	case piece_kind::rook: return os << "r";
	case piece_kind::queen: return os << "q";
	case piece_kind::king: return os << "K";
	default:
		throw "Piece kind is not in the list [pawn, knight, bishop, rook, queen, king]";
	}
}
