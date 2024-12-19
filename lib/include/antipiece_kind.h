#include <piece_kind.h>

namespace engine {

enum class antipiece_kind {
	pawn, knight, bishop, rook, queen, king
};

antipiece_kind from_piece_kind(piece_kind);

piece_kind to_piece_kind(antipiece_kind);

};
