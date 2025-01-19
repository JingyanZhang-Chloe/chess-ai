#include <chrono>
#include <board_t.h>

using namespace engine;

// Counts number of reachable positions
void perft(board_t& board, int depth, int& counter) {
	if (depth == 0 || board.is_game_over()) {
		counter++; 
		return;
	}

	auto pseudolegal_moves = board.pseudolegal_moves();

	for (const move_t& move : pseudolegal_moves) {
		if (!board.is_legal(move)) continue;

		auto move_info = board.make_move(move);
		perft(board, depth - 1, counter);
		board.unmake_move(move_info);
	}
}

int perft(board_t& board, int depth) {
	int counter = 0;
	perft(board, depth, counter);
	
	return counter;
}

int main() {
	using namespace std::chrono;

	high_resolution_clock::time_point start_time, end_time;

	board_t start_board;
	
	for (int i = 0; i < 6; i++) {
		start_time = high_resolution_clock::now();
		int result = perft(start_board, i);
		end_time = high_resolution_clock::now();

		std::cout << "Perft " << i << " is: " << result << ". It tooks "
		<< duration_cast<milliseconds>(end_time - start_time).count() << "ms." << std::endl;
	}
}
