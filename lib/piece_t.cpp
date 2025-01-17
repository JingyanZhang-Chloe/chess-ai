#include "include/piece_t.h" 

using namespace engine;

piece_t::piece_t(piece_kind kind, player_color color) : kind{ kind }, color{ color } {};

bool piece_t::operator==(const piece_t&) const = default;
