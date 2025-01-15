#include <iostream>
#include <chess_coordinate_t.h>
#include <board_t.h>
#include <fstream>
#include <move_t.h>
#include <search_fn.h>

using namespace engine;

std::optional<move_t> strategy(board_t& board, int depth = 4) {
	float best_score = minmax(board, depth);
	std::cout << best_score << ";";

	for (move_t move : board.legal_moves()) {
		move_info_t move_info = board.make_move(move);
		float score = minmax(board, depth - 1);
		std::cout << score << " ";
		board.unmake_move(move_info);

		if (score == best_score) return move;
	}

	return std::nullopt;
}

int main(int argc, char** argv) {
	board_t board;

	std::ifstream input_file{ argv[2] };
	std::ofstream output_file{ argv[4] };

	std::string file_string;

	try {
		for (std::string move_string; std::getline(input_file, move_string);) {
			file_string += move_string;

			board.make_move(move_string);
		}

		std::optional<move_t> chosen_move = strategy(board);

		if (chosen_move.has_value())
			output_file << chosen_move.value() << std::endl;
	
		return 0;
	} catch (std::exception* e) {
		std::cout << e->what();
		std::cout << file_string;
		return 1;
	}
}
