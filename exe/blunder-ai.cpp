#include <iostream>
#include <chess_coordinate_t.h>
#include <board_t.h>
#include <fstream>
#include <move_t.h>
#include <search_fn.h>
#include <strategy_fn.h>

using namespace engine;

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

		move_t chosen_move = strategies::minmax(board);
		board.make_move(chosen_move);

		output_file << chosen_move << std::endl;
	
		return 0;
	} catch (std::exception* e) {
		std::cout << e->what();
		std::cout << file_string;
		return 1;
	}
	catch (const char* str) {
		std::cout << str;
		return 1;
	}
}
