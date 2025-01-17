#pragma once
#include <board_t.h>
#include <move_t.h>

namespace engine::strategies {

move_t random(board_t&);

move_t minmax(board_t&);

}
