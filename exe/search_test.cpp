#include <board_t.h>
#include <search_fn.h>
#include <vector>
#include <chrono>

using namespace engine;
using namespace engine::minmax_types;
using namespace std::chrono;

void cache_log(board_t& board, std::vector<move_t> moves) {
	cache_t cache;

	board_t board_copy = board;
	std::cout << board_copy << std::endl;

	std::vector<move_t> _pv;
	std::vector<move_t> pv {{ "b5c5" }};
	int cc = 0;

	std::cout << board_copy.score() << std::endl;

	auto move = pv_search(board_copy, cache, 4);
	std::cout << board_copy.score() << std::endl;
	std::cout << "Move: " << move << std::endl;
	std::cout << "[!PV!]: ";

	for (auto m : _pv) std::cout << m << " ";
	std::cout << std::endl;

	for (auto move : board.pseudolegal_moves()) {
		auto move_info = board.make_move(move);
		auto hash = board.to_bitset();
		board.unmake_move(move_info);

		if (cache.contains(hash)) {
			std::cout << "[Cache Log] (Initial) -> (" << move <<
				"): (" << cache[hash].first
			<< ", " << cache[hash].second << ")" << std::endl;
		}
	}

	for (auto path_move : moves) {
		board.make_move(path_move);
		std::cout << board << std::endl;

		for (auto move : board.pseudolegal_moves()) {
			auto move_info = board.make_move(move);
			auto hash = board.to_bitset();

			if (cache.contains(hash)) {
				std::cout << "[Cache Log] (" << path_move
				<< ") -> (" << move << "): (" << cache[hash].first
				<< ", " << cache[hash].second << ")" << std::endl;
			}

			board.unmake_move(move_info);
		}
	}
}

int main() {
	try {
	board_t board1{"2bqkbnr/3n3p/Q3p3/p3N1p1/P1p2PP1/NpPp3B/1P1P1P1P/R1BK3R w k - 0 ?"};
	cache_log(board1, {});
	}
	catch (const char* e) {
		std::cout << e;
	}

	return 0;
}
