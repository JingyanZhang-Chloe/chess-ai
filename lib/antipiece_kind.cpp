#include <antipiece_kind.h>

using namespace engine;

antipiece_kind from_piece_kind(piece_kind kind) {
	switch (kind) {
		case piece_kind::pawn:
			return antipiece_kind::pawn;
		
		case piece_kind::knight:
			return antipiece_kind::knight;

		case piece_kind::bishop:
			return antipiece_kind::bishop;	

		case piece_kind::rook:
			return antipiece_kind::rook;

		case piece_kind::queen:
			return antipiece_kind::queen;

		case piece_kind::king:
			return antipiece_kind::king;
	}
};

piece_kind to_piece_kind(antipiece_kind kind) {
	switch (kind) {
		case antipiece_kind::pawn:
			return piece_kind::pawn;
		
		case antipiece_kind::knight:
			return piece_kind::knight;

		case antipiece_kind::bishop:
			return piece_kind::bishop;	

		case antipiece_kind::rook:
			return piece_kind::rook;

		case antipiece_kind::queen:
			return piece_kind::queen;

		case antipiece_kind::king:
			return piece_kind::king;
	}
};
