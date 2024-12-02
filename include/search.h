#include <board_t>
#include <moves>

namespace algorithms {
	// This struct shows which methods an algorithm should implement
	// Do not inherit from it. (Inherited classes are slower)
	struct base {
		// Get which next move the search believes is best
		virtual move_t next_move(board_t) = 0;
	};

	struct minmax {
		// Add fields here

		move_t next_move(board_t);
	};
};
