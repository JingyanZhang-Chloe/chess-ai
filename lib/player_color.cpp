#include <iostream>
#include <player_color.h>

using namespace engine;
using namespace player_color_fn;

// Reviewed
player_color player_color_fn::opposite(player_color col) {
	switch (col) {
	case player_color::white: return player_color::black;
	case player_color::black: return player_color::white;
	}
}

// Reviewed
std::ostream& operator<<(std::ostream& os, player_color color) {
	switch (color) {
	case player_color::white: return os << "white";
	case player_color::black: return os << "black";
	}
}
