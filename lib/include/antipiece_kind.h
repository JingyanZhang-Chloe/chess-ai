#pragma once
#include <piece_kind.h>

namespace engine {

enum class antipiece_kind {
	pawn=0, knight=1, bishop=2, rook=3, queen=4, king=5
};

namespace piece_kind_fn {
	antipiece_kind from_piece_kind(piece_kind);
	piece_kind to_piece_kind(antipiece_kind);
}

};
