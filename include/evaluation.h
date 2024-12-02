#include <board_t>

namespace evaluation {
	// This evaluation function returns the weighted sum of material
	// If the position is winning for white, return a large positive value
	// If the position is winning for black, return a large negative value
	double by_material(board_t);
};
