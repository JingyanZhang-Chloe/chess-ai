#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <piece_t.h>
#include <move_t.h>
#include <chess_coordinate_t.h>
#include <move_info_t.h>
#include <cardboard_t.h>

namespace engine {

class board_t {
public:
	board_t();
	board_t(std::string fen_string);

	bool operator==(const board_t&) const;

	//=======================
	// Move-related functions
	//=======================
	move_info_t make_move(move_t);
	void unmake_move(move_info_t);

	std::vector<move_t> pseudolegal_moves();
	std::optional<move_t> latest_move() const;

	bool is_legal(move_t);

	//========================
	// Piece-related functions
	//========================
	std::optional<piece_t>& piece(chess_coordinate_t);
	const std::optional<piece_t>& piece(chess_coordinate_t) const;

	bool is_attacked(chess_coordinate_t);

	int piece_count() const;
	int piece_count(player_color) const;
	int piece_count(piece_kind) const;
	int piece_count(player_color, piece_kind) const;

	std::optional<chess_coordinate_t> king_coordinates(player_color) const;

	//=============================
	// Game-state-related functions
	//=============================
	player_color turn_color() const;
	std::optional<player_color> winning_player() const;

	enum class side { left, right };
	bool can_castle(side);
	
	bool is_check();
	bool is_checkmate();
	bool is_draw();
	bool is_game_over();

	int score();

	using hash_t = cardboard_t;
	hash_t to_bitset();

	std::string fen();

private:
	std::optional<piece_t> pieces[64];

	player_color _turn_color;
	std::optional<move_t> _latest_move;

	bool white_king_or_left_rook_moved;
	bool white_king_or_right_rook_moved;
	bool black_king_or_left_rook_moved;
	bool black_king_or_right_rook_moved;

	std::array<std::optional<chess_coordinate_t>, 2> _king_coordinates;

	std::array<int, 12> __piece_count;
	int& _piece_count(player_color, piece_kind);
	const int& _piece_count(player_color, piece_kind) const;

	int positional_score;

	std::unordered_map<hash_t, int> position_count;

	int turns_since_capture_or_pawn_move;

	move_info_t get_move_info(move_t);

	// Cached Info
	std::optional<bool> _is_check;
	std::optional<bool> _is_draw;
	std::optional<std::vector<move_t>> _pseudolegal_moves;
	std::optional<hash_t> current_hash;
};

};



std::ostream& operator << (std::ostream&, const engine::board_t&);
