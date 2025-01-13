#include <iostream>
#include <cmath>
#include <piece_kind.h> 

using namespace engine;

float piece_kind_fn::get_score(piece_kind kind) {
	switch (kind) {
	case piece_kind::pawn: return piece_kind_info<piece_kind::pawn>::value;
	case piece_kind::knight: return piece_kind_info<piece_kind::knight>::value;
	case piece_kind::bishop: return piece_kind_info<piece_kind::bishop>::value;
	case piece_kind::rook: return piece_kind_info<piece_kind::rook>::value;
	case piece_kind::queen: return piece_kind_info<piece_kind::queen>::value;
	case piece_kind::king: return std::numeric_limits<float>::infinity();
	}
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
