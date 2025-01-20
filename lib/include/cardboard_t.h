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

	bool operator==(const cardboard_t&) const = default;
	std::strong_ordering operator<=>(const cardboard_t&) const = default;

	friend std::hash<cardboard_t>;

private:
	std::array<std::uint64_t, 4> piece_blocks;
	std::uint8_t rights_block;
};

}

template<>
class std::hash<engine::cardboard_t> {
public:
	std::size_t operator()(const engine::cardboard_t&) const;
};
