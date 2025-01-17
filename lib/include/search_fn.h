#pragma once
#include <board_t.h>
#include <move_t.h>

namespace engine {

namespace minmax_types {

using cache_t = std::unordered_map<board_t::hash_t, std::pair<int, float>>;

}

float max_pass(board_t&, int depth, float alpha, float beta, minmax_types::cache_t&);
float min_pass(board_t&, int depth, float alpha, float beta, minmax_types::cache_t&);

float minmax(board_t&, int depth, minmax_types::cache_t& cache);
float minmax(board_t&, int depth);

}
