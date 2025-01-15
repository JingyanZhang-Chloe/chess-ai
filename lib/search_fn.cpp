#include <cmath>
#include <search_fn.h>
#include <move_t.h>
#include <move_info_t.h>
#include <player_color.h>

using namespace engine;

float max_pass(board_t&, int);

float min_pass(board_t& board, int depth) {
	if (depth == 0) return board.score();
	
	float min_score = std::numeric_limits<float>::infinity();

	for (move_t move : board.legal_moves()) {
		move_info_t move_info = board.make_move(move);

		float score = max_pass(board, depth - 1);

		board.unmake_move(move_info);

		if (score < min_score) score = min_score;
	}

	return min_score;
}

float max_pass(board_t& board, int depth) {
	if (depth == 0) return board.score();
	
	float max_score = -std::numeric_limits<float>::infinity();

	for (move_t move : board.legal_moves()) {
		move_info_t move_info = board.make_move(move);

		float score = min_pass(board, depth - 1);

		board.unmake_move(move_info);

		if (score > max_score) score = max_score;
	}

	return max_score;
}

float minmax(board_t& board, int depth) {
	return board.turn_color() == player_color::white
		? max_pass(board, depth)
		: min_pass(board, depth);
}
