#pragma once
#include <iostream>

namespace engine {

enum class player_color {
	white=0, black=1
};

namespace player_color_fn {

player_color opposite(player_color);

}

}

std::ostream& operator<<(std::ostream&, engine::player_color);
