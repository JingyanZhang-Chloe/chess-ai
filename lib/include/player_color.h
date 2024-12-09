#include <iostream>

enum class player_color {
	white, black
};

std::ostream& operator<<(std::ostream&, player_color);
