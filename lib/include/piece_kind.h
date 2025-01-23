#pragma once
#include <iostream>
#include <chess_coordinate_t.h>
#include <player_color.h>

namespace engine {

enum class piece_kind {
	pawn=0, knight=1, bishop=2, rook=3, queen=4, king=5
};

template<piece_kind kind>
class piece_kind_info;

template<>
struct piece_kind_info<piece_kind::pawn> {
	static constexpr int value = 90;
	
	// Square table for white, reflect for black
	static constexpr int square_table[] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		10, 25, 25, 25, 25, 25, 10,
		10, 25, 25, 25, 25, 25, 10,
		10, 25, 25, 25, 25, 25, 10,
		10, 25, 25, 25, 25, 25, 10,
		10, 25, 25, 25, 25, 25, 10,
		10, 25, 25, 25, 25, 25, 10,
	};
};

template<>
struct piece_kind_info<piece_kind::knight> {
	static constexpr int value = 320;

	static constexpr int square_table[] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 5, 10, 10, 10, 10, 5, 0,
		0, 10, 25, 25, 25, 25, 10, 0,
		0, 10, 25, 25, 25, 25, 10, 0,
		0, 10, 25, 25, 25, 25, 10, 0,
		0, 10, 25, 25, 25, 25, 10, 0,
		0, 5, 10, 10, 10, 10, 5, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};
};

template<>
struct piece_kind_info<piece_kind::bishop> {
	static constexpr int value = 330;

	static constexpr int square_table[] = {
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
	};
};

template<>
struct piece_kind_info<piece_kind::rook> {
	static constexpr int value = 490;

	static constexpr int square_table[] = {
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
	};
};

template<>
struct piece_kind_info<piece_kind::queen> {
	static constexpr int value = 910;

	static constexpr int square_table[] = {
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
	};
};

template<>
struct piece_kind_info<piece_kind::king> {
	static constexpr int square_table[] = {
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10,
	};
};

namespace piece_kind_fn {
	constexpr auto all = { piece_kind::pawn, piece_kind::knight, piece_kind::bishop,
		piece_kind::rook, piece_kind::queen, piece_kind::king };

	int get_score(piece_kind);
	int get_position_score(piece_kind, player_color, chess_coordinate_t);
}

}

std::ostream& operator<<(std::ostream&, engine::piece_kind);
