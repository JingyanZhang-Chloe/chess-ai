#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <piece_t.h>
#include <move_t.h>
#include <chess_coordinate_t.h>
#include <bitset>

namespace engine {

class board_t {
public:
	board_t();

	//=======================
	// Move-related functions
	//=======================
	board_t& make_move(move_t);

	std::vector<move_t> legal_moves() const;
	std::optional<move_t> latest_move() const;	

	//========================
	// Piece-related functions
	//========================
	std::optional<piece_t>& piece(chess_coordinate_t);
	const std::optional<piece_t>& piece(chess_coordinate_t) const;

	bool is_attacked(chess_coordinate_t) const;

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
	bool can_castle(side) const;
	
	bool is_check() const;
	bool is_draw() const;

	float score() const;
	std::bitset<265> to_bitset() const;

private:
	std::optional<piece_t> pieces[64];

	player_color _turn_color;
	std::optional<move_t> _latest_move;

	bool white_king_or_left_rook_moved;
	bool white_king_or_right_rook_moved;
	bool black_king_or_left_rook_moved;
	bool black_king_or_right_rook_moved;

	std::array<int, 12> __piece_count;
	int& _piece_count(player_color, piece_kind);
	const int& _piece_count(player_color, piece_kind) const;

	using hash_t = std::bitset<265>;

	std::unordered_map<hash_t, int> position_count;

	int turns_since_capture_or_pawn_move;

};

};

std::ostream& operator << (std::ostream&, const engine::board_t&);
