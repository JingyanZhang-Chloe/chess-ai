#include <board_t.h>
#include <cassert>

using namespace engine;

void display_moves(std::vector<move_t> moves) {
	for (auto move : moves) std::cout << move << std::endl;
}

int main() {
	try {
	std::cout << "==========[Count 1]==========\n";
	board_t initial_board;
	std::cout << initial_board << std::endl;
	auto moves = initial_board.legal_moves();
	display_moves(moves);

	std::cout << "==========[Count 2]==========\n";
	board_t board1{"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 "};
	std::cout << board1 << std::endl;
	moves = board1.legal_moves();
	display_moves(moves);
	
	std::cout << "==========[Count 3]==========\n";
	board_t board2{"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"};
	std::cout << board2 << std::endl;
	moves = board2.legal_moves();
	display_moves(moves);

	std::cout << "==========[Count 4]==========\n";
	board_t board3{"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"};
	std::cout << board3 << std::endl;
	moves = board3.legal_moves();
	display_moves(moves);
	} catch (const char* e) {
		std::cout << e;
	}
	
	return 0;
}
