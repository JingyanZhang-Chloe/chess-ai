#pragma once
#include <piece_kind.h>

namespace engine {

enum class antipiece_kind {
	pawn=1, knight=2, bishop=3, rook=4, queen=5, king=6
};

namespace piece_kind_fn {
	antipiece_kind from_piece_kind(piece_kind);
	piece_kind to_piece_kind(antipiece_kind);
}

};
