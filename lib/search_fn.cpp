#include <cmath>
#include <search_fn.h>
#include <move_t.h>
#include <move_info_t.h>
#include <player_color.h>

using namespace engine;
using namespace engine::minmax_types;

float engine::min_pass(board_t& board, int depth, cache_t& cache) {
	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;

	if (depth == 0 || board.is_game_over()) {
		float score = board.score();

		cache[board_hash] = { depth, score };
		return score;
	}
	
	float min_score = std::numeric_limits<float>::infinity();

	for (move_t move : board.pseudolegal_moves()) {
		move_info_t move_info = board.make_move(move);

		float score = max_pass(board, depth - 1, cache);

		board.unmake_move(move_info);

		if (score < min_score) min_score = score;
	}

	cache[board_hash] = { depth, min_score }; 
	return min_score;
}

float engine::max_pass(board_t& board, int depth, cache_t& cache) {
	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;

	if (depth == 0 || board.is_game_over()) {
		float score = board.score();

		cache[board_hash] = { depth, score };
		return score;
	}
	
	float max_score = -std::numeric_limits<float>::infinity();

	for (move_t move : board.pseudolegal_moves()) {
		move_info_t move_info = board.make_move(move);

		float score = min_pass(board, depth - 1, cache);

		board.unmake_move(move_info);

		if (score > max_score) max_score = score;
	}

	cache[board_hash] = { depth, max_score }; 
	return max_score;
}


float engine::minmax(board_t& board, int depth) {
	cache_t cache;

	return engine::minmax(board, depth, cache);
}

float engine::minmax(board_t& board, int depth, cache_t& cache) {
	return board.turn_color() == player_color::white
		? max_pass(board, depth, cache)
		: min_pass(board, depth, cache);
}
