#include <iostream>
#include "include/player_color.h"

using namespace engine;

std::ostream& operator<<(std::ostream& os, player_color color) {
	switch (color) {
		case player_color::white:
			return os << "white";
			break;
		case player_color::black:
			return os << "black";
			break;
	};
};

