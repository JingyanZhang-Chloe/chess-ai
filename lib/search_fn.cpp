#include <cmath>
#include <search_fn.h>
#include <move_t.h>
#include <move_info_t.h>
#include <player_color.h>
#include <chrono>

using namespace engine;
using namespace engine::minmax_types;

using namespace std::chrono;
using time_point_t = std::chrono::time_point<high_resolution_clock>;

constexpr int timeout_check_every = 500;
constexpr int timeout = 4500;

class timeout_t {};

int max_pass(board_t&, int, int, int, cache_t&);
int max_pass(board_t&, int, int, int, cache_t&, time_point_t, int&);

int min_pass(
	board_t& board, int depth, int alpha, int beta, cache_t& cache
) {
	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;

	if (depth == 0 || board.is_game_over()) {
		int score = board.score();

		cache[board_hash] = { depth, score };
		return score;
	}
	
	int min_score = std::numeric_limits<int>::max();

	for (move_t move : board.pseudolegal_moves()) {
		move_info_t move_info = board.make_move(move);

		int score = max_pass(board, depth - 1, alpha, beta, cache);

		board.unmake_move(move_info);

		if (score < min_score) min_score = score;
		if (min_score <= alpha) return -std::numeric_limits<int>::max();
		if (min_score < beta) beta = min_score;
	}

	cache[board_hash] = { depth, min_score }; 
	return min_score;
}

int min_pass(
	board_t& board, int depth, int alpha, int beta, cache_t& cache, time_point_t start_time, int& call_counter
) {
	call_counter++;

	// Check for timeout
	if (
		call_counter % timeout_check_every == 0
		&& duration_cast<milliseconds>(high_resolution_clock::now() - start_time)
			.count() >= timeout
	) throw timeout_t {};

	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;

	if (depth == 0 || board.is_game_over()) {
		int score = board.score();

		cache[board_hash] = { depth, score };
		return score;
	}
	
	int min_score = std::numeric_limits<int>::max();

	for (move_t move : board.pseudolegal_moves()) {
		move_info_t move_info = board.make_move(move);

		int score = max_pass(board, depth - 1, alpha, beta, cache, start_time, call_counter);

		board.unmake_move(move_info);

		if (score < min_score) min_score = score;
		if (min_score <= alpha) return -std::numeric_limits<int>::max();
		if (min_score < beta) beta = min_score;
	}

	cache[board_hash] = { depth, min_score }; 
	return min_score;
}

int max_pass(
	board_t& board, int depth, int alpha, int beta, cache_t& cache) {
	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;

	if (depth == 0 || board.is_game_over()) {
		int score = board.score();

		cache[board_hash] = { depth, score };
		return score;
	}
	
	int max_score = -std::numeric_limits<int>::max();

	for (move_t move : board.pseudolegal_moves()) {
		move_info_t move_info = board.make_move(move);

		int score = min_pass(board, depth - 1, alpha, beta, cache);

		board.unmake_move(move_info);

		if (score > max_score) max_score = score;
		if (max_score >= beta) return std::numeric_limits<int>::max();
		if (max_score > alpha) alpha = max_score;
	}

	cache[board_hash] = { depth, max_score }; 
	return max_score;
}

int max_pass(
	board_t& board, int depth, int alpha, int beta, cache_t& cache, time_point_t start_time, int& call_counter
) {
	call_counter++;

	// Check for timeout
	if (
		call_counter % timeout_check_every == 0
		&& duration_cast<milliseconds>(high_resolution_clock::now() - start_time)
			.count() >= timeout
	) throw timeout_t {};

	board_t::hash_t board_hash = board.to_bitset();

	if (cache.contains(board_hash) && depth <= cache[board_hash].first)
		return cache[board_hash].second;

	if (depth == 0 || board.is_game_over()) {
		int score = board.score();

		cache[board_hash] = { depth, score };
		return score;
	}
	
	int max_score = -std::numeric_limits<int>::max();

	for (move_t move : board.pseudolegal_moves()) {
		move_info_t move_info = board.make_move(move);

		int score = min_pass(board, depth - 1, alpha, beta, cache, start_time, call_counter);

		board.unmake_move(move_info);

		if (score > max_score) max_score = score;
		if (max_score >= beta) return std::numeric_limits<int>::max();
		if (max_score > alpha) alpha = max_score;
	}

	cache[board_hash] = { depth, max_score }; 
	return max_score;
}

int engine::minmax(board_t& board, int depth, cache_t& cache) {
	int alpha = -std::numeric_limits<int>::infinity();
    int beta  =  std::numeric_limits<int>::infinity();

	if (board.turn_color() == player_color::white) {
		return max_pass(board, depth, alpha, beta, cache);
    } else {
        return min_pass(board, depth, alpha, beta, cache);
    }
}

int engine::minmax(board_t& board, int depth) {
	cache_t cache;

	return minmax(board, depth, cache);
}

int pv_search_min_pass(
	board_t& board, 
	int depth,
	int max_depth,
	int alpha,
	int beta,
	cache_t& cache,
	std::vector<move_t>& pv,
	time_point_t start_time,
	int& call_counter
) {
	call_counter++;

	// Check for timeout
	if (
		call_counter % timeout_check_every == 0
		&& duration_cast<milliseconds>(high_resolution_clock::now() - start_time)
			.count() >= timeout
	) throw timeout_t {};

	auto hash = board.to_bitset();

	// Handle base case
	if (depth == 0 || board.is_game_over()) {
		auto score = board.score();

		cache[hash] = { depth, score };
		return score;
	}

	// Check if cached
	if (cache.contains(hash)) {
		auto result = cache[hash];

		if (depth <= result.first) return result.second;
	}

	int min_score = std::numeric_limits<int>::max();

	// Handle principal variation
	if (max_depth - depth < pv.size()) {
		auto pv_move = pv[max_depth - depth];
		board_t board_copy = board;

		auto move_info = board.make_move(pv_move);

		int score = pv_search_max_pass(
			board, depth - 1, max_depth, alpha, beta, cache, pv, start_time, call_counter
		);

		board.unmake_move(move_info);

		min_score = score;
		beta = min_score;
	}

	// Handle other moves
	auto moves = board.pseudolegal_moves();

	for (auto move : moves) {
		// Skip the PV move as we already considered it
		if (max_depth - depth < pv.size() && move == pv[max_depth - depth]) continue;

		board_t board_copy = board;

		auto move_info = board.make_move(move);
		int score = max_pass(board, depth - 1, alpha, beta, cache, start_time, call_counter);
		board.unmake_move(move_info);

		if (score < min_score) min_score = score;
		if (min_score <= alpha) break;
		if (min_score < beta) beta = min_score;
	}

	cache[hash] = { depth, min_score };
	return min_score;
}

int engine::pv_search_max_pass(
	board_t& board, 
	int depth,
	int max_depth,
	int alpha,
	int beta,
	cache_t& cache,
	std::vector<move_t>& pv,
	time_point_t start_time,
	int& call_counter
) {
	call_counter++;

	// Check for timeout
	if (
		call_counter % timeout_check_every == 0
		&& duration_cast<milliseconds>(high_resolution_clock::now() - start_time)
			.count() >= timeout
	) throw timeout_t {};

	auto hash = board.to_bitset();

	// Handle base case
	if (depth == 0 || board.is_game_over()) {
		auto score = board.score();

		cache[hash] = { depth, score };
		return score;
	}

	// Check if cached
	if (cache.contains(hash)) {
		auto result = cache[hash];

		if (depth <= result.first) return result.second;
	}

	int max_score = -std::numeric_limits<int>::max();

	// Handle principal variation
	if (max_depth - depth < pv.size()) {
		auto pv_move = pv[max_depth - depth];

		auto move_info = board.make_move(pv_move);

		int score = pv_search_min_pass(
			board, depth - 1, max_depth, alpha, beta, cache, pv, start_time, call_counter
		);

		board.unmake_move(move_info);

		max_score = score;
		alpha = max_score;
	}

	// Handle other moves
	auto moves = board.pseudolegal_moves();

	for (auto move : moves) {
		// Skip the PV move as we already considered it
		if (max_depth - depth < pv.size() && move == pv[max_depth - depth]) continue;

		auto move_info = board.make_move(move);
		int score = min_pass(board, depth - 1, alpha, beta, cache, start_time, call_counter);
		board.unmake_move(move_info);

		if (score > max_score) max_score = score;
		if (max_score >= beta) break;
		if (max_score > alpha) alpha = max_score;
	}

	cache[hash] = { depth, max_score };
	return max_score;
}

void engine::get_pv(
	board_t& board, cache_t& cache, int goal, int depth, std::vector<move_t>& pv
) {
	if (depth == 0 || board.is_game_over()) return;

	auto pseudolegal_moves = board.pseudolegal_moves();

	for (auto move : pseudolegal_moves) {
		auto move_info = board.make_move(move);
		auto hash = board.to_bitset();

		if (
			cache.contains(hash) 
			&& cache[hash].first >= depth - 1 
			&& cache[hash].second == goal
		) {
			pv.push_back(move);
			get_pv(board, cache, cache[hash].second, depth - 1, pv);
			board.unmake_move(move_info);
			break;
		}

		board.unmake_move(move_info);
	}
}

move_t engine::pv_search(board_t& board, cache_t& cache, int start_depth) {
	using namespace std::chrono;

	board_t board_copy = board;

	auto start_time = high_resolution_clock::now();
	
	std::vector<move_t> pv;
	
	int i = start_depth;
	std::optional<move_t> best_move;

	try {
	for (;; i++) { 
		std::cout << "Searching at depth " << i << std::endl;
		int alpha = -std::numeric_limits<int>::max();
		int beta  =  std::numeric_limits<int>::max();

		int call_counter = 0;

		int best_score = board.turn_color() == player_color::white
		? pv_search_max_pass(board, i, i, alpha, beta, cache, pv, start_time, call_counter)
		: pv_search_min_pass(board, i, i, alpha, beta, cache, pv, start_time, call_counter);
		pv.clear();
		get_pv(board, cache, best_score, i, pv);
		best_move = pv[0];
	}
	}
	catch (timeout_t) {
		std::cout << board_copy.fen() << std::endl;

		return best_move.value();
	}
}

move_t engine::pv_search(board_t& board, int start_depth) {
	cache_t cache;

	return pv_search(board, cache, start_depth);
}
