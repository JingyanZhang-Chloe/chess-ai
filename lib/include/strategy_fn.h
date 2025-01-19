#pragma once
#include <board_t.h>
#include <move_t.h>

namespace engine::strategies {

move_t minmax(board_t&, int depth = 4);

}
