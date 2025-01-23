#include <chrono>
#include <board_t.h>
#include <bitset>
#include <search_fn.h>

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
		
		board_t copy = board;
		auto move_info = board.make_move(move);
		perft(board, depth - 1, counter);
		board.unmake_move(move_info);

		if (copy != board) {
			std::cout << "Board is: " << std::endl;
			std::cout << board << std::endl;
			std::cout << "With score: " << board.score() << std::endl;
			std::cout << "But should be: " << std::endl;
			std::cout << copy << std::endl;
			std::cout << "With score: " << copy.score() << std::endl;
			std::cout << "After making the move: " << move << std::endl;
			throw "[Perft]: make_move and unmake_move do not invert: ";
		}
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

	board_t start_board { "2bqkbnr/3n3p/Q3p3/p3N1p1/P1p2PP1/NpPp3B/1P1P1P1P/R1BK3R w k - 0 ?" };
	
	std::cout << start_board.score();

	for (int i = 0; i < 5; i++) {
		start_time = high_resolution_clock::now();
		int result = perft(start_board, i);
		end_time = high_resolution_clock::now();

		std::cout << "Perft " << i << " is: " << result << ". It tooks "
		<< duration_cast<milliseconds>(end_time - start_time).count() << "ms." << std::endl;
	}

	std::cout << start_board.score();

	std::cout << "Principal Variation Search: " << std::endl;
	std::cout << engine::pv_search(start_board);

	std::bitset<265> a;
	
	start_time = high_resolution_clock::now();
	for (int i = 1; i < 100000000; i++) {
		std::hash<std::bitset<265>>{}(a);
	}
	end_time = high_resolution_clock::now();

	std::cout << "Hashing bitset<265> " << ". It tooks "
	<< duration_cast<milliseconds>(end_time - start_time).count() << "ms." << std::endl;

	cardboard_t b { new std::optional<piece_t>[64], player_color::white, false, false, false, false, false };
	
	start_time = high_resolution_clock::now();
	for (int i = 1; i < 100000000; i++) {
		std::hash<cardboard_t>{}(b);
	}
	end_time = high_resolution_clock::now();

	std::cout << "Hashing cardboard_t " << ". It tooks "
	<< duration_cast<milliseconds>(end_time - start_time).count() << "ms." << std::endl;

}
