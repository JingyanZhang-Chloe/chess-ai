#include <iostream>
#include <piece_kind.h> 

using namespace engine;

// Reviewed
std::ostream& operator<<(std::ostream& os, piece_kind piece_name){
	switch (piece_name) {
	case piece_kind::pawn: return os << "pawn";
	case piece_kind::knight: return os << "knight";
	case piece_kind::bishop: return os << "bishop";
	case piece_kind::rook: return os << "rook";
	case piece_kind::queen: return os << "queen";
	case piece_kind::king: return os << "king";
	default:
		throw "Piece kind is not in the list [pawn, knight, bishop, rook, queen, king]";
	}
}
