#include <board_t.h>

using namespace engine;

int main() {
	board_t board{ "7k/5Q2/8/8/8/8/8/8 b - - 1 1" };
	std::cout << board.can_castle(board_t::side::left);

	std::cout << board;

	return board.is_draw() ? 0 : 1;
}
