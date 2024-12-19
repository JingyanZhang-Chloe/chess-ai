#pragma once
#include <iostream>
#include "piece_t.h"
#include "chess_coordinate_t.h"
#include "board_t.h"
#include "piece_kind.h"

namespace engine{

template<piece_kind K>
std::vector<move_t> gen_moves(chess_coordinate_t, player_color, const board_t&);
// this function do not consider if it is actually "legal" since we don't consider check pos

















}

