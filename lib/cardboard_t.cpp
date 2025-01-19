#include <cardboard_t.h>

using namespace engine;

cardboard_t::cardboard_t(
	std::optional<piece_t> pieces[],
	player_color turn_color,
	bool can_en_passant,
	bool white_king_or_left_rook_moved,
	bool white_king_or_right_rook_moved,
	bool black_king_or_left_rook_moved,
	bool black_king_or_right_rook_moved
): piece_blocks{{ 0, 0, 0, 0 }}, rights_block{ 
	static_cast<std::uint8_t>(
		can_en_passant
		| white_king_or_left_rook_moved << 1
		| white_king_or_right_rook_moved << 2
		| black_king_or_left_rook_moved << 3
		| black_king_or_right_rook_moved << 4
		// TODO: Remove when referee bug is fixed
		// | turn_color == player_color::white ? 0 : 32
	)
} 
{

	for (int row = 0; row < 8; row++) for (int col = 0; col < 8; col++) {
		auto index = 8 * row + col;

		if (pieces[index].has_value()) {
			auto piece = pieces[index].value();
			
			auto bit_offset = 4 * index;
			auto block_index = bit_offset >> 6;
			auto block_bit_offset = bit_offset % 64;

			std::uint64_t piece_code = static_cast<std::uint64_t>(piece.kind)
				| (piece.color == player_color::white ? 0 : 16);

			this->piece_blocks[block_index] |= (piece_code << bit_offset);
		}
	}
}

std::size_t std::hash<cardboard_t>::operator()(const cardboard_t& cardboard) const {
	std::hash<std::uint64_t> hash64;
	std::hash<std::uint8_t> hash8;

	std::size_t re = 0;

	for (int i = 0; i < 1; i++) {
		re ^= hash64(cardboard.piece_blocks[i]);
	}
	
	re ^= hash8(cardboard.rights_block);

	return re;
}
