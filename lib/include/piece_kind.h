#pragma once
#include <iostream>

namespace engine {

enum class piece_kind {
	pawn, knight, bishop, rook, queen, king
};

};

std::ostream& operator<<(std::ostream&, engine::piece_kind);
