#pragma once
#include <board_t.h>
#include <move_t.h>
#include <chrono>

namespace engine {

namespace minmax_types {

using cache_t = std::unordered_map<board_t::hash_t, std::pair<int, float>>;

}

int minmax(board_t&, int depth, minmax_types::cache_t& cache);
int minmax(board_t&, int depth);

void get_pv(board_t&, minmax_types::cache_t&, int goal, int depth, std::vector<move_t>&);

int pv_search_max_pass(
	board_t& board, 
	int depth,
	int max_depth,
	int alpha,
	int beta,
	minmax_types::cache_t& cache,
	std::vector<move_t>& pv,
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
	int& call_counter
);

move_t pv_search(board_t&, minmax_types::cache_t&, int start_depth);
move_t pv_search(board_t&, int start_depth = 1);

}
