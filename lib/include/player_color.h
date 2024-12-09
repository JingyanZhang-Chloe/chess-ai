#pragma once
#include <iostream>

namespace engine {

enum class player_color {
	white, black
};

};

std::ostream& operator<<(std::ostream&, engine::player_color);
