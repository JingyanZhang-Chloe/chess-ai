#pragma once
#include <iostream>
#include <optional>
#include "piece_kind.h"
#include "chess_coordinate_t.h"

namespace engine {

struct move_t {
	engine::chess_coordinate_t source;
	engine::chess_coordinate_t destination;

	std::optional<engine::piece_kind> promotion_code;

	move_t(const std::string& move_spec);
	move_t(chess_coordinate_t start, chess_coordinate_t end);

	bool operator == (const move_t& move) const;
};

};

std::ostream& operator << (std::ostream& os, const engine::move_t& move);


