#include <iostream>
#include <chess_coordinate_t.h>

int main() {
	engine::chess_coordinate_t coord{"b1"};

	std::cout << coord;

	return 0;
}
