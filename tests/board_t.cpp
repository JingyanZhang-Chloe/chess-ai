#include <board_t.h>

int main() {
	engine::board_t test;
	test.make_move(std::string{"e2e4"});

	std::cout << test;
};
