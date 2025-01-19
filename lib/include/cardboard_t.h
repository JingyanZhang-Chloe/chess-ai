#pragma once
#include <piece_t.h>
#include <player_color.h>

namespace engine {

class cardboard_t {
public:
	cardboard_t(
		std::optional<piece_t>[],
		player_color,
		bool can_en_passant,
		// castlings rights
		bool,
		bool,
		bool,
		bool
	);

	std::strong_ordering operator<=>(const cardboard_t&) const;

private:
	std::array<std::uint64_t, 4> piece_blocks;
	std::uint8_t rights_block;
};

}
