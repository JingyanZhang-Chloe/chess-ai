#include <iostream>
#include <optional>
#include <piece_t.h>

struct move_t {
	std::pair<int, int> source;
	std::pair<int, int> destination;

	std::optional<piece_kind> promotion_code;

	move_t(const std::string& move_spec);
};

std::ostream& operator << (std::ostream& os, move_t& move);


