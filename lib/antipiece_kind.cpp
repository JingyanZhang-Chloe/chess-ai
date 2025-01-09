#include <antipiece_kind.h>
#include <piece_kind.h>

using namespace engine;

antipiece_kind from_piece_kind(piece_kind k) {
	return static_cast<antipiece_kind>(k);
};

piece_kind to_piece_kind(antipiece_kind k) {
	return static_cast<piece_kind>(k);
};
