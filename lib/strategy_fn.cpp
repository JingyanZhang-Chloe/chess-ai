#include <strategy_fn.h>
#include <search_fn.h>

using namespace engine;

move_t engine::strategies::minmax(board_t& board, int depth) {
	minmax_types::cache_t cache;

	float best_score = engine::minmax(board, depth, cache);

	auto pseudolegal_moves = board.pseudolegal_moves();

	for (auto move : pseudolegal_moves) {
		auto move_info = board.make_move(move);
		auto hash = board.to_bitset();

		if (cache.contains(hash)) {
			float score = cache[hash].second;

			if (best_score == score) {
				board.unmake_move(move_info);
				return move;
			}
		}

		board.unmake_move(move_info);
	}

	for (auto move : pseudolegal_moves) {
		auto move_info = board.make_move(move);
		auto hash = board.to_bitset();

		if (cache.contains(hash)) {
			float score = cache[hash].second;
			std::cout << score << " ";

			if (best_score == score) {
				board.unmake_move(move_info);
				return move;
			}
		}

		board.unmake_move(move_info);
	}
	throw "[Minmax Strategy] Move recurrence relation is invalid";
}
