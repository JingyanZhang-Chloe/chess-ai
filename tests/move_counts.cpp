#include <board_t.h>
#include <cassert>

using namespace engine;

void display_moves(std::vector<move_t> moves) {
	for (auto move : moves) std::cout << move << std::endl;
}

std::vector<move_t> legal_moves(board_t& board) {
	std::vector<move_t> moves = board.legal_moves();
	std::vector<move_t> re;

	for (auto move : moves) {
		move_info_t move_info = board.make_move(move);
		auto moves_2 = board.legal_moves();
		
		bool other_can_capture = false;

		for (auto move_2 : moves_2) {
			auto move_info_2 = board.make_move(move_2);
			if (board.winning_player().has_value()) {
				other_can_capture = true;
				board.unmake_move(move_info_2);
				break;
			}
			board.unmake_move(move_info_2);
		}
		
		if (!other_can_capture) re.push_back(move);

		board.unmake_move(move_info);
	}
	
	return re;
}

int perft(board_t& board, int depth) {
	if (depth == 0) return 1;
	
	int result = 0;

	std::vector<move_t> possible_moves = legal_moves(board);
	for (auto move : possible_moves) {
		auto move_info = board.make_move(move);
		result += perft(board, depth - 1);
		board.unmake_move(move_info);
	};

	return result;
}
		
int main() {
	try {
	std::cout << "==========[Count 1]==========\n";
	board_t initial_board;
	std::cout << initial_board << std::endl;
	assert(perft(initial_board, 1) == 20);
	//assert(perft(initial_board, 2) == 400);
	std::cout << "passed1" << std::endl;
//	assert(perft(initial_board, 3) == 8902);

	std::cout << "==========[Count 2]==========\n";
	board_t board1{"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 "};
	std::cout << board1 << std::endl;
	display_moves(legal_moves(board1));
	assert(perft(board1, 1) == 6);
	std::cout << "passed2" << std::endl;
	assert(perft(board1, 2) == 264);
	
	std::cout << "==========[Count 3]==========\n";
	board_t board2{"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"};
	std::cout << board2 << std::endl;

	std::cout << "==========[Count 4]==========\n";
	board_t board3{"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"};
	std::cout << board3 << std::endl;
	
	} catch (const char* e) {
		std::cout << e;
	}
	
	return 0;
}
