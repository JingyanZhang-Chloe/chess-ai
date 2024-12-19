#include <iostream>
#include "include/player_color.h"

std::ostream& operator<<(std::ostream& os, engine::player_color color) {
	using namespace engine;

	switch (color) {
		case player_color::white:
			return os << "white";
			break;
		case player_color::black:
			return os << "black";
			break;
		default:
			return os;
			break;
	};
};
