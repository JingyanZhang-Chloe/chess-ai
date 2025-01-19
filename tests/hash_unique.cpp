#include <cardboard_t.h>
#include <board_t.h>
#include <unordered_set>
#include <random>

using namespace engine;

std::optional<move_t> random_move(board_t& board, std::mt19937 rng) {
	auto pseudolegal_moves = board.pseudolegal_moves();
	std::vector<move_t> legal_moves;

	for (auto move : pseudolegal_moves) {
		if (board.is_legal(move)) legal_moves.push_back(move);
	}

	if (legal_moves.size() == 0) return std::nullopt;
	
	std::uniform_int_distribution<std::size_t> dist{0, legal_moves.size() - 1};
	return legal_moves[dist(rng)];
}

int main() {
	std::random_device dev;
	std::mt19937 rng{dev()};
	
	float total_collision_ratio = 0;
	int count = 0;
	
	for (int i = 0; i < 100; i++) {
		int total = 0;
		int collisions = 0;

		board_t board;

		std::unordered_set<board_t::hash_t> set;

		for (int j = 0; j < 100; j++) {
			total++;

			if (set.contains(board.to_bitset())) {
				collisions++;
			}
			else set.insert(board.to_bitset());

			auto move = random_move(board, rng);

			if (!move.has_value()) break;

			board.make_move(move.value());
		}

		count++;
		total_collision_ratio += static_cast<float>(collisions) / static_cast<float>(total);
	}
	
	std::cout << "Average collision ratio: " << (total_collision_ratio / count);
}
