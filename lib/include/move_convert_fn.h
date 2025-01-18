#pragma once
#include <move_t.h>
#include <board_t.h>

namespace engine{

std::string to_san(move_t move, board_t& board);

}
