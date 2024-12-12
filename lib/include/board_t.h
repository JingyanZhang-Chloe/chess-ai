#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include "piece_t.h"
#include "move_t.h"
#include "chess_coordinate_t.h"

namespace engine {

class board_t {
	public:
		struct castling_rights_t {
			bool white;
			bool black;

			castling_rights_t();
		};

		board_t();

		const std::optional<piece_t>& piece(chess_coordinate_t) const;
		
		board_t& make_move(move_t);
		board_t& unmake_latest_move();

		std::vector<move_t> get_legal_moves() const;

	private:
		std::optional<piece_t> pieces[64];
		castling_rights_t castling_rights;
		std::stack<board_t> history;

		std::optional<piece_t>& piece(chess_coordinate_t);
};

};

std::ostream& operator << (std::ostream&, const engine::board_t&);
