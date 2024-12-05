enum class piece_kind {
	pawn, knight, bishop, rook, queen, king
};

enum class player_color {
	white, black
};

struct piece_t {
	piece_kind kind;
	player_color color;

	piece_t(piece_kind, player_color);
};


