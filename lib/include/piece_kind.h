#pragma once
#include <iostream>

namespace engine {

enum class piece_kind {
	pawn=0, knight=1, bishop=2, rook=3, queen=4, king=5
};

template<piece_kind kind>
class piece_kind_info;

template<>
struct piece_kind_info<piece_kind::pawn> {
	static constexpr float value = 0.9;
};

template<>
struct piece_kind_info<piece_kind::knight> {
	static constexpr float value = 3.2;
};

template<>
struct piece_kind_info<piece_kind::bishop> {
	static constexpr float value = 3.3;
};

template<>
struct piece_kind_info<piece_kind::rook> {
	static constexpr float value = 4.9;
};

template<>
struct piece_kind_info<piece_kind::queen> {
	static constexpr float value = 9.1;
};

namespace piece_kind_fn {
	constexpr auto all = { piece_kind::pawn, piece_kind::knight, piece_kind::bishop,
		piece_kind::rook, piece_kind::queen, piece_kind::king };

	float get_score(piece_kind);
}

}

std::ostream& operator<<(std::ostream&, engine::piece_kind);
