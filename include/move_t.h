#include <iostream>

struct move_t {
	int row;
	int column;
	char promotion_code;

	move_t(const std::string& move_spec);
};
