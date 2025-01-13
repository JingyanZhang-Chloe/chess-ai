#include <iostream>
#include <chess_coordinate_t.h>
#include <board_t.h>
#include <fstream>
#include <move_t.h>

using namespace engine;

int main(int argc, char** argv) {
	board_t board;

	std::ifstream input_file{ argv[2] };
	std::ofstream output_file{ argv[4] };

	for (std::string move_string; std::getline(input_file, move_string);) {
		board.make_move(move_string);
	}

	std::vector<move_t> legal_moves = board.legal_moves();

	if (legal_moves.empty()) {
		std::cerr << "No possible legal moves" << std::endl;
	} else {
		output_file << legal_moves[0] << std::endl;
	}
	
	return 0;
}
