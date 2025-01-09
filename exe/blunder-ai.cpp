#include <iostream>
#include <chess_coordinate_t.h>
#include <board_t.h>
#include <fstream>
#include <move_t.h>

int main(int argc, char ** argv) {
	engine::board_t board;
	std::ifstream inputFile(argv[2]);
	for (std::string move_string; std::getline(inputFile, move_string);) {
		engine::move_t move(move_string);
		board.make_move(move);
		}

	std::vector<engine::move_t> legal_moves = board.get_legal_moves();
	//here we will use the function search in search_t.h instead of legal_moves[0]//
	std::ofstream asciiFile(argv[4]); // dont know what path to put yet
	if(legal_moves.empty()){
		std::cerr << "No possible legal moves" << std::endl;
	}else {
		asciiFile << legal_moves[0] << std::endl;
	}
	
	return 0;
}
