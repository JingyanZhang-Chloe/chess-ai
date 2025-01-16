#include <cmath>
#include <search_fn.h>
#include <move_t.h>
#include <move_info_t.h>
#include <player_color.h>

using namespace engine;

// Cache that maps board hashes to pairs of the depth of computation and the
// computed score
using cache_t = std::unordered_map<board_t::hash_t, std::pair<int, float>>;

float max_pass(board_t&, int, float, float, cache_t&);

float min_pass(board_t& board, int depth, float alpha, float beta, cache_t& cache) {
	if (depth == 0) return board.score();

	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;

	float min_score = std::numeric_limits<float>::infinity();

	for (move_t move : board.legal_moves()) {
		move_info_t move_info = board.make_move(move);

		float score = max_pass(board, depth - 1, alpha, beta, cache);


		board.unmake_move(move_info);

		if (score < min_score) min_score = score;
		if (min_score <= alpha) break;
		if (min_score < beta) beta = min_score;
	}

	cache[board_hash] = { depth, min_score }; 
	return min_score;
}

float max_pass(board_t& board, int depth, float alpha, float beta, cache_t& cache) {
	if (depth == 0) return board.score();

	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;
	
	float max_score = -std::numeric_limits<float>::infinity();

	for (move_t move : board.legal_moves()) {
		move_info_t move_info = board.make_move(move);

		float score = min_pass(board, depth - 1, alpha, beta, cache);

		board.unmake_move(move_info);

		if (score > max_score) max_score = score;
		if (max_score >= beta) break;
		if (max_score > alpha) alpha = max_score;
	}

	cache[board_hash] = { depth, max_score }; 
	return max_score;
}

float engine::minmax(board_t& board, int depth) {
	cache_t cache;

	float alpha = -std::numeric_limits<float>::infinity();
	float beta =   std::numeric_limits<float>::infinity();

	return board.turn_color() == player_color::white
		? max_pass(board, depth, alpha, beta, cache)
		: min_pass(board, depth, alpha, beta, cache);
}
