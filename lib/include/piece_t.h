#pragma once
#include "piece_kind.h"
#include "player_color.h"

namespace engine {

struct piece_t {
	piece_kind kind;
	player_color color;

	piece_t(piece_kind, player_color);
	
};

};


