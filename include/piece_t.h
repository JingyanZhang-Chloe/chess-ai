#include<iostream>

enum class piece_kind {
	pawn, knight, bishop, rook, queen, king
};

std::ostream& operator << (std::ostream& os, piece_kind piece_name);

enum class player_color {
	white, black
};

struct piece_t {
	piece_kind kind;
	player_color color;

	piece_t(piece_kind, player_color);
};


