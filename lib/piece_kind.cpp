#include <iostream>
#include "include/piece_kind.h" 

using namespace engine;

std::ostream& operator<<(std::ostream& os, piece_kind piece_name){
	switch (piece_name)
	{
	case piece_kind::pawn:
		return os << "pawn";
		break;
	case piece_kind::knight:
		return os << "knight";
		break;
	case piece_kind::bishop:
		return os << "bishop";
		break;
	case piece_kind::rook:
		return os << "rook";
		break;
	case piece_kind::queen:
		return os << "queen";
		break;
	case piece_kind::king:
		return os << "king";
		break;
	default:
		throw "Error in printing the piece_kind";
		break;
	}
}
