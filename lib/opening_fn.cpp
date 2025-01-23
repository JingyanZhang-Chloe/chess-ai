#include <opening.h>

using namespace engine;

std::unordered_map<std::string, std::optional<engine::move_t>> fen_map;

std::optional<move_t> engine::opening(board_t& board) {
	std::string fen = board.fen();

	if (!fen_map.contains(fen)) return std::nullopt;

	return fen_map[fen];
}

