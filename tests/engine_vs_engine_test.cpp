#include <iostream>
#include <chrono>
#include "move_t.h"
#include "board_t.h"
#include <search_fn.h>
#include <piece_t.h>
#include <vector>
#include <random>
#include <optional>

using namespace engine;

// Random move strategy
move_t strategy(const board_t& board, std::mt19937 rng) {
	std::vector<move_t> legal_moves = board.legal_moves();

	std::uniform_int_distribution<int> rand_dist{0, (int) legal_moves.size() - 1};
	
	return legal_moves.at(rand_dist(rng));
}

int main() {
	int seed;
	std::cout << "Enter the random seed (type a random integer): ";
	std::cin >> seed;
	std::cout << "\n\n";

	std::mt19937 rng;
	rng.seed(seed);

    board_t board;
    int max_move_time = 10000; // milliseconds

    std::cout << "\033[94m============[Self-play Test]============\033[0m\n\n";

	for (int move_counter = 0;; move_counter++) {
		std::cout << board << std::endl;

		if (board.winning_player().has_value()) {
			std::cout << "\033[32m[Game Won]: \033[1m"
			<< board.winning_player().value() << "\033[0m" << std::endl;
			break;
		}
		else if (board.is_draw()) {
			std::cout << "\033[32m[Game Draw]\033[1m" << std::endl;
			break;
		}

        auto starting_time = std::chrono::steady_clock::now();

		move_t move = strategy(board, rng);

        auto ending_time = std::chrono::steady_clock::now();
        auto move_time = std::chrono::duration_cast<std::chrono::milliseconds>(ending_time - starting_time).count();

        if (move_time > max_move_time) {
            std::cerr<< "[Error] Engine took more than time than allowed.\n";
            return 1;
        }

        board.make_move(move);

        std::cout << "Move Number: " << move_counter
		<< "\nMove: " << move 
		<< ",\nPiece: " << board.piece(move.destination).value().kind
		<< ",\nColor: " << board.piece(move.destination).value().color 
		<< ",\nTime taken: " << move_time << "ms\n" << std::endl;
    }

    return 0;
}
