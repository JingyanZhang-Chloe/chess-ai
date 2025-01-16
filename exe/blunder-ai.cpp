#include <iostream>
#include <chess_coordinate_t.h>
#include <board_t.h>
#include <fstream>
#include <random>
#include <move_t.h>
#include <search_fn.h>
#include <chrono>

using namespace engine;

std::vector<move_t> legal_moves(board_t& board) {
	board_t initial_board = board;

	std::vector<move_t> moves = board.legal_moves();
	std::vector<move_t> re;

	for (auto move : moves) {
		move_info_t move_info = board.make_move(move);
		board_t board_after_move = board;
		auto moves_2 = board.legal_moves();
		
		bool other_can_capture = false;

		for (auto move_2 : moves_2) {
			auto move_info_2 = board.make_move(move_2);

			if (board.winning_player().has_value()) {
				other_can_capture = true;
				board.unmake_move(move_info_2);
				break;
			}
			board.unmake_move(move_info_2);

			if (board != board_after_move) {
				std::cout << "[Legal Moves] Board not same after making and unmaking sub-move" << std::endl;
				std::cout << "Should be: " << std::endl;
				std::cout << board_after_move << std::endl;
				std::cout << "But is: " << std::endl;
				std::cout << board << std::endl;
				std::cout << "The sub-move was: " << move_2 << std::endl;
				throw;
			}
		}
		
		if (!other_can_capture) re.push_back(move);

		board.unmake_move(move_info);

		if (board != initial_board) {
			std::cout << "[Legal Moves] Board not same after making and unmaking move" << std::endl;
			std::cout << "Should be: " << std::endl;
			std::cout << initial_board << std::endl;
			std::cout << "But is: " << std::endl;
			std::cout << board << std::endl;
			std::cout << "The move was: " << move << std::endl;
			throw;
		}
	}
	
	return re;
}

std::optional<move_t> random_strategy(board_t& board) {
	std::random_device rd;

    // seed value designed specifically to be different across app executions
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

    std::mt19937 gen(seed);
	
	std::vector<move_t> _legal_moves = legal_moves(board);
	std::uniform_int_distribution<int> dist{0, (int) _legal_moves.size() - 1};
	
	std::cout << std::endl << _legal_moves.size() << " legal moves found by engine" << std::endl;

	return _legal_moves.at(dist(gen));
}

std::optional<move_t> minmax_strategy(board_t& board, int depth = 4) {
	float best_score = std::numeric_limits<float>::infinity() 
		* (board.turn_color() == player_color::white ? -1 : 1);
	std::optional<move_t> best_move;

	minmax_types::cache_t cache;
	
	for (move_t move : board.legal_moves()) {
		move_info_t move_info = board.make_move(move);
		float score = minmax(board, depth - 1, cache);
		board.unmake_move(move_info);
		
		std::cout << score << " ";

		if ((board.turn_color() == player_color::white && score > best_score)
		|| (board.turn_color() == player_color::black && score < best_score)) {
			best_score = score;
			best_move = move;
		}
	}

	return best_move;
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

		std::optional<move_t> chosen_move = minmax_strategy(board);

		if (chosen_move.has_value())
			output_file << chosen_move.value() << std::endl;
	
		return 0;
	} catch (std::exception* e) {
		std::cout << e->what();
		std::cout << file_string;
		return 1;
	}
}
