#pragma once
#include <chess_coordinate_t.h>
#include <board_t.h>
#include <piece_kind.h>
#include <antipiece_kind.h>

namespace engine {

template<piece_kind K>
void gen_moves(
	chess_coordinate_t, player_color, board_t&, std::vector<move_t>&
);

void gen_moves(
	piece_kind, 
	chess_coordinate_t, 
	player_color, 
	board_t&,
	std::vector<move_t>& 
);

// For reference on what is an antipiece, see antipiece_kind.h
template<antipiece_kind K>
void gen_moves(
	chess_coordinate_t, player_color, board_t&, std::vector<move_t>&
);

void gen_moves(
	antipiece_kind, 
	chess_coordinate_t, 
	player_color, 
	board_t&,
	std::vector<move_t>&
);

}

