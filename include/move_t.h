#include <iostream>

struct move_t {
	std::pair<int, int> source;
	std::pair<int, int> destination;

	char promotion_code;

	move_t(const std::string& move_spec);
};
