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
			bool left_white;
			bool left_black;
			bool right_white;
			bool right_black;

			castling_rights_t();
		};

		board_t();

		std::optional<piece_t>& piece(chess_coordinate_t);
		const std::optional<piece_t>& piece(chess_coordinate_t) const;
		
		board_t& make_move(move_t);
		board_t& unmake_latest_move();

		std::vector<move_t> get_legal_moves() const;

		std::optional<chess_coordinate_t> king_coordinates(player_color) const;

		std::optional<move_t> last_move;

		castling_rights_t castling_rights;

		std::optional<player_color> wining_player();

		player_color turn_color;

		bool is_check();
		//this function check if the current player is in check

	private:
		std::optional<piece_t> pieces[64];
		std::stack<board_t, std::vector<board_t>> history;
};

};

std::ostream& operator << (std::ostream&, const engine::board_t&);
