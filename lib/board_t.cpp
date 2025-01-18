#include <iostream>
#include <optional>
#include <cmath>
#include <board_t.h>
#include <piece_t.h>
#include <player_color.h>
#include <piece_kind.h>
#include <gen_move_fn.h>

using namespace engine;

// Reviewed, tested
board_t::board_t() 
	: _turn_color { player_color::white }, 
	white_king_or_left_rook_moved { false },
	white_king_or_right_rook_moved { false },
	black_king_or_left_rook_moved { false },
	black_king_or_right_rook_moved { false },
	_king_coordinates {{chess_coordinate_t{ 0, 4 }, chess_coordinate_t{ 7, 4 }}},
	__piece_count{{8, 2, 2, 2, 1, 1, 8, 2, 2, 2, 1, 1}},
	turns_since_capture_or_pawn_move{0}
{
	// Add pawns
	for (int column = 0; column < 8; column++) {
		this->piece({ 1, column }) = piece_t{ piece_kind::pawn, player_color::white };
		this->piece({ 6, column }) = piece_t{ piece_kind::pawn, player_color::black };
	}

	// Add rooks
	this->piece({ 0, 0 }) = piece_t{ piece_kind::rook, player_color::white };
	this->piece({ 0, 7 }) = piece_t{ piece_kind::rook, player_color::white };

	this->piece({ 7, 0 }) = piece_t{ piece_kind::rook, player_color::black };
	this->piece({ 7, 7 }) = piece_t{ piece_kind::rook, player_color::black };

	// Add knights
	this->piece({ 0, 1 }) = piece_t{ piece_kind::knight, player_color::white };
	this->piece({ 0, 6 }) = piece_t{ piece_kind::knight, player_color::white };

	this->piece({ 7, 1 }) = piece_t{ piece_kind::knight, player_color::black };
	this->piece({ 7, 6 }) = piece_t{ piece_kind::knight, player_color::black };

	// Add bishops
	this->piece({ 0, 2 }) = piece_t{ piece_kind::bishop, player_color::white };
	this->piece({ 0, 5 }) = piece_t{ piece_kind::bishop, player_color::white };

	this->piece({ 7, 2 }) = piece_t{ piece_kind::bishop, player_color::black };
	this->piece({ 7, 5 }) = piece_t{ piece_kind::bishop, player_color::black };

	// Add queens
	this->piece({ 0, 3 }) = piece_t{ piece_kind::queen, player_color::white };
	this->piece({ 7, 3 }) = piece_t{ piece_kind::queen, player_color::black };

	// Add kings
	this->piece({ 0, 4 }) = piece_t{ piece_kind::king, player_color::white };
	this->piece({ 7, 4 }) = piece_t{ piece_kind::king, player_color::black };

	this->position_count.insert({ this->to_bitset(), 1 });
}

// TODO: Input validation
board_t::board_t(std::string fen_string): 
	white_king_or_left_rook_moved{ true },
	white_king_or_right_rook_moved{ true },
	black_king_or_left_rook_moved{ true },
	black_king_or_right_rook_moved{ true },
	__piece_count{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}} {
	std::array<std::string, 6> fen_fields;
	
	for (int i = 0; i < 5; i++) {
		std::size_t delim_index = fen_string.find(" ");
		
		fen_fields[i] = fen_string.substr(0, delim_index);
		fen_string.erase(0, delim_index + 1);
	}

	fen_fields[5] = fen_string;

	// We use the first field to fill the pieces in the board
	int current_coordinate = 0;

	for (char c : fen_fields[0]) {
		if (std::isdigit(c)) current_coordinate += c - '1'; 
		else if (c == '/') continue;
		else {
			// Need to reflect the position because FEN is top-down
			int index = (7 - (current_coordinate / 8)) * 8 + (current_coordinate % 8);

			auto color = std::isupper(c) ? player_color::white : player_color::black;
			piece_kind kind;

			switch (std::tolower(c)) {
			case 'p': kind = piece_kind::pawn; break;
			case 'n': kind = piece_kind::knight; break;
            case 'r': kind = piece_kind::rook; break;
            case 'q': kind = piece_kind::queen; break;
            case 'b': kind = piece_kind::bishop; break;
            case 'k': 
				kind = piece_kind::king;
				this->_king_coordinates[static_cast<int>(color)] = 
					chess_coordinate_t{ index }; 
				break;
            }

			this->pieces[index] = piece_t{ kind, color };
			this->_piece_count(color, kind)++;
		}

		current_coordinate++;
	}

	// We use the second field to set whose turn it is
	this->_turn_color = fen_fields[1] == "w" ? player_color::white : player_color::black;

	// We use the third field to set the castling rights
	for (char c : fen_fields[2])
	switch (c) {
	case 'K': this->white_king_or_right_rook_moved = false; break;
	case 'Q': this->white_king_or_left_rook_moved = false; break;
	case 'k': this->black_king_or_right_rook_moved = false; break;
	case 'q': this->black_king_or_left_rook_moved = false; break;
	}

	// We use the fourth field to set the latest move
	if (fen_fields[3] != "-") {
		chess_coordinate_t en_passant_position{ fen_fields[3] };
		int y_direction = this->_turn_color == player_color::white ? 1 : -1;

		this->_latest_move = move_t{
			{ en_passant_position.row() + y_direction, en_passant_position.column() },
			{ en_passant_position.row() - y_direction, en_passant_position.column() }
		};
	}

	// We use the fifth field as the number of "half-turns"
	this->turns_since_capture_or_pawn_move = std::stoi(fen_fields[4]);

	// We don't use the sixth field
}

bool board_t::operator==(const board_t& other) const {
	// Check that everything is equal except cached info
	return std::equal(
		std::begin(this->pieces), 
		std::end(this->pieces), 
		std::begin(other.pieces)
	) && this->turn_color() == other.turn_color()
	&& this->latest_move() == other.latest_move()
	&& this->white_king_or_left_rook_moved == other.white_king_or_left_rook_moved
	&& this->white_king_or_right_rook_moved == other.white_king_or_right_rook_moved
	&& this->black_king_or_left_rook_moved == other.black_king_or_left_rook_moved
	&& this->black_king_or_right_rook_moved == other.black_king_or_right_rook_moved
	&& this->_king_coordinates == other._king_coordinates
	&& this->__piece_count == other.__piece_count
	&& this->position_count == other.position_count
	&& this->turns_since_capture_or_pawn_move == other.turns_since_capture_or_pawn_move;
}

// Reviewed
move_info_t board_t::make_move(move_t move) {
	move_info_t info = get_move_info(move);

	if(!(this->piece(move.source).has_value())){
		std::cout << move.source << std::endl;
		throw "[Error in make_move] try to make a move from an empty position";
	};

	this->turns_since_capture_or_pawn_move++;

	// Update castling-related information if rook or king is moving
	if (this->piece(move.source).value().kind == piece_kind::king) {
		if (this->_turn_color == player_color::white) {
			this->white_king_or_left_rook_moved = true;
			this->white_king_or_right_rook_moved = true;
		}

		else {
			this->black_king_or_left_rook_moved = true;
			this->black_king_or_right_rook_moved = true;
		}

		this->_king_coordinates[static_cast<int>(this->turn_color())] = move.destination;
	}
	if (this->piece(move.source).value().kind == piece_kind::rook) {
		if (this->_turn_color == player_color::white) {
			if (move.source.column() == 0)
				this->white_king_or_left_rook_moved = true;
			else if (move.source.column() == 7)
				this->white_king_or_right_rook_moved = true;
		}
		else {
			if (move.source.column() == 0)
				this->black_king_or_left_rook_moved = true;
			else if (move.source.column() == 7)
				this->black_king_or_right_rook_moved = true;
		}
	}

	// Update 50-move-rule-related information
	if (this->piece(move.source).value().kind == piece_kind::pawn)
		this->turns_since_capture_or_pawn_move = 0;

	// Handle the case where the king is castling
	if (this->piece(move.source).value().kind == piece_kind::king
		&& std::abs(move.destination.column() - move.source.column()) > 1) {
		// We move the rook, the king movement gets handled normally after
		int castle_direction = move.destination.column() - move.source.column();
		int rook_column = castle_direction > 0 ? 7 : 0;
		int new_rook_column = castle_direction > 0 ? 5 : 3;
		
		this->piece({ move.source.row(), rook_column }) = std::nullopt;
		this->piece({ move.source.row(), new_rook_column }) 
			= piece_t{ piece_kind::rook, this->_turn_color };
	}

	// Handle capture
	if (this->piece(move.destination).has_value()) {
		this->_piece_count(
			player_color_fn::opposite(this->_turn_color), 
			this->piece(move.destination).value().kind
		)--;

		this->turns_since_capture_or_pawn_move = 0;

		piece_t captured_piece = this->piece(move.destination).value();

		if (captured_piece.kind == piece_kind::king)
			this->_king_coordinates[static_cast<int>(captured_piece.color)] = std::nullopt;
	}

	if(this->piece(move.source).value().kind == piece_kind::pawn){
		if(abs(move.source.row() - move.destination.row()) == abs(move.source.column() - move.destination.column())){
			if(!(this->piece(move.destination).has_value())){
				// then it is en passant
				//std::cout << *this << std::endl;
				//std::cout << "we are handling an en passant, the evil pawn's coord: " << move.source << std::endl;


				if(!(this->piece({move.source.row(), move.destination.column()}).has_value()) || this->piece({move.source.row(), move.destination.column()}).value().kind != piece_kind::pawn){
					std::cout << "the evil pawn is here: " << move.source << std::endl;
					throw "[Error in make move en passant: try to eat some no pawn piece by en passant]";
				}

				//if it is indeed a pawn, we peacefully eat it
				this->piece({move.source.row(), move.destination.column()}) = std::nullopt;
				this->_piece_count(player_color_fn::opposite(this->turn_color()), piece_kind::pawn)--;
			}
		}
	}

	this->piece(move.destination) = this->piece(move.source);
	this->piece(move.source) = std::nullopt;

	// Handle promotion
	if (move.promotion_code.has_value()) {
		this->_piece_count(this->_turn_color, move.promotion_code.value())++;
		this->_piece_count(this->_turn_color, piece_kind::pawn)--;

		this->piece(move.destination) = piece_t{
			move.promotion_code.value(),
			this->_turn_color
		};
	}

	// Clear cache
	this->_is_check = std::nullopt;
	this->_is_draw = std::nullopt;
	this->_pseudolegal_moves = std::nullopt;
	this->current_hash = std::nullopt;


	// Update game-state-related information
	this->_turn_color = player_color_fn::opposite(this->_turn_color);
	this->_latest_move = move;
	
	board_t::hash_t current_hash = this->to_bitset();

	if (this->position_count.contains(current_hash))
		this->position_count[current_hash]++;
	else 
		this->position_count.insert({ current_hash, 1 });
	
	
	return info;
}

// Reviewed
std::vector<move_t> board_t::pseudolegal_moves() { 
	if (!_pseudolegal_moves.has_value()) {
		this->_pseudolegal_moves = std::vector<move_t>{};
		this->_pseudolegal_moves.value().reserve(16);

		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				chess_coordinate_t coord { row, col };
				const auto& mb_piece = this->piece(coord);

				if (mb_piece.has_value() && mb_piece.value().color == this->_turn_color) {
					piece_t p = mb_piece.value();

					gen_moves(
						p.kind, 
						coord, 
						this->_turn_color, 
						*this, 
						this->_pseudolegal_moves.value()
					);
				}
			}
		}
	}

	return this->_pseudolegal_moves.value();
}

// Reviewed
std::optional<move_t> board_t::latest_move() const {
	return this->_latest_move;
}

bool board_t::is_legal(move_t move) {
	auto move_info = this->make_move(move);
	this->_turn_color = player_color_fn::opposite(this->_turn_color);
	
	bool legal = !this->is_check();

	this->_turn_color = player_color_fn::opposite(this->_turn_color);
	this->unmake_move(move_info);
	
	return legal;
}

// Reviewed
std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) {
	return this->pieces[coord.index()];
}

// Reviewed
const std::optional<piece_t>& board_t::piece(chess_coordinate_t coord) const {
	return this->pieces[coord.index()];
}

bool board_t::is_attacked(chess_coordinate_t coord) {
	player_color opp_color = player_color_fn::opposite(this->_turn_color);

	std::vector<move_t> move_container;
	move_container.reserve(28);

	for (auto kind : { antipiece_kind::pawn, antipiece_kind::knight, 
		antipiece_kind::bishop, antipiece_kind::rook, antipiece_kind::queen, 
		antipiece_kind::king }) {

		gen_moves(kind, coord, opp_color, *this, move_container);
		
		for (auto move : move_container)
		if (this->piece(move.destination).has_value()
		&& this->piece(move.destination).value().kind == 
		piece_kind_fn::to_piece_kind(kind)
		) return true;

		move_container.clear();
	}
	
	return false;
}

// Reviewed
int board_t::piece_count() const {
	return this->piece_count(player_color::white) + this->piece_count(player_color::black);
}

// Reviewed
int board_t::piece_count(player_color color) const {
	int re = 0;

	for (auto kind : { piece_kind::pawn, piece_kind::knight, piece_kind::bishop,
		piece_kind::rook, piece_kind::queen, piece_kind::king })
		re += this->piece_count(color, kind);

	return re;
}

// Reviewed
int board_t::piece_count(piece_kind kind) const {
	int re = 0;

	for (auto color: { player_color::white, player_color::black })
		re += this->piece_count(color, kind);

	return re;
}

// Reviewed
int board_t::piece_count(player_color color, piece_kind kind) const {
	return this->_piece_count(color, kind);
}

// Reviewed
std::optional<chess_coordinate_t> board_t::king_coordinates(player_color color) const {
	return this->_king_coordinates[static_cast<int>(color)];
}

// Reviewed
player_color board_t::turn_color() const {
	return this->_turn_color;
}

// Reviewed
std::optional<player_color> board_t::winning_player() const {
	if(!king_coordinates(player_color::white).has_value()) {
		return player_color::black;
	}

	if(!king_coordinates(player_color::black).has_value()) {
		return player_color::white;
	}

	return std::nullopt;
}

bool board_t::can_castle(board_t::side side) {
	int castle_row;
	bool king_or_rook_moved;

	if (this->_turn_color == player_color::white) {
		castle_row = 0;

		if (side == board_t::side::left)
			king_or_rook_moved = this->white_king_or_left_rook_moved;
		else
			king_or_rook_moved = this->white_king_or_right_rook_moved;
	}
	else {
		castle_row = 7;

		if (side == board_t::side::left)
			king_or_rook_moved = this->black_king_or_left_rook_moved;
		else
			king_or_rook_moved = this->black_king_or_right_rook_moved;
	}

	if (king_or_rook_moved) return false;
	
	if (this->is_check()) return false;

	// We check if all pieces between the king and the rook are empty
	// Then we check if the the king will castle through check
	if (side == board_t::side::left) {
		for (int column = 3; column >= 1; column--)
		if (this->piece({ castle_row, column }).has_value()) return false;

		for (int column = 3; column >= 2; column--)
		if (this->is_attacked({ castle_row, column })) return false;
	}
	else {
		for (int column = 5; column <= 6; column++)
		if (this->piece({ castle_row, column }).has_value()) return false;

		for (int column = 5; column <= 6; column++)
		if (this->is_attacked({ castle_row, column })) return false;
	}

	return true;
}

// Reviewed
bool board_t::is_check() {
	if (!this->_is_check.has_value()) {
		std::optional<chess_coordinate_t> king_coords = 
		this->king_coordinates(this->turn_color());
		
		this->_is_check = king_coords.has_value() 
			&& this->is_attacked(king_coords.value());
	}

	return this->_is_check.value();
}


bool board_t::is_checkmate() {

    if(!is_check()){
        return false;
    }

    // Generate all possible moves for the current player
    auto moves = pseudolegal_moves();

    // Check if there is legal move
    for(const auto& move : moves){
        if(is_legal(move)){
            return false; // Found a legal move, so not checkmate
        }
    }

    // No legal moves and the player is in check
    return true;
}

// TODO:  Optimization needed here, redundancy of legal_moves call.
bool board_t::is_draw() {
	if (this->_is_draw.has_value()) return this->_is_draw.value();

	// Case 1 : Stalemate
	if (!this->is_check()) {
		std::vector<move_t> moves = this->pseudolegal_moves();
		bool exists_legal_move = false;
		
		for (auto move : moves) 
		if (this->is_legal(move)) {
			exists_legal_move = true;
			break;
		}

		if (!exists_legal_move) {
			this->_is_draw = true;
			return true;
		}
	}

	// Case 2 : Dead Position
	int __piece_count = this->piece_count();
	// King vs. King: Only two kings are left
	if (__piece_count == this->piece_count(piece_kind::king)) {
		this->_is_draw = true;
		return true; 
	}
	
	//King and Bishop vs. King: A king with one bishop cannot checkmate
	if (__piece_count == 3
		&& this->piece_count(piece_kind::king) == 2
		&& this->piece_count(piece_kind::bishop) == 1
	) {
		this->_is_draw = true;
		return true;
	}

	//King and Knight vs. King: A king with one knight cannot checkmate
	if (__piece_count == 3
		&& this->piece_count(piece_kind::king) == 2
		&& this->piece_count(piece_kind::bishop) == 1
	) {
		this->_is_draw = true;
		return true;
	}

	// Case 3 : Threefold Repetition
	hash_t current_hash = this->to_bitset();

	if (this->position_count.contains(current_hash)
		&& this->position_count.at(current_hash) >= 3) {
		this->_is_draw = true;
		return true;
	}

	// Case 4 : 50-Move Rule
	if (this->turns_since_capture_or_pawn_move >= 50) {
		this->_is_draw = true;
		return true;
	}

	this->_is_draw = false;
	return false;
}

bool board_t::is_game_over() {
	return this->winning_player().has_value() || this->is_draw();
}

float board_t::score() {
	std::optional<player_color> _winning_player = this->winning_player();
	if (_winning_player.has_value()) {
		if (_winning_player.value() == player_color::white)
			return std::numeric_limits<float>::infinity();
		else
			return -std::numeric_limits<float>::infinity();
	}

	if (this->is_draw()) {
		return -1;
	}
	
	float score = 0;

	for (auto kind : { piece_kind::pawn, piece_kind::knight, piece_kind::bishop,
		piece_kind::rook, piece_kind::queen })
		score += piece_kind_fn::get_score(kind) * (
			this->piece_count(player_color::white, kind)
			- this->piece_count(player_color::black, kind)
		);

	return score;
}

int& board_t::_piece_count(player_color color, piece_kind kind) {
	return this->__piece_count[static_cast<int>(color) * 6 + static_cast<int>(kind)];
}

const int& board_t::_piece_count(player_color color, piece_kind kind) const {
	return this->__piece_count[static_cast<int>(color) * 6 + static_cast<int>(kind)];
}

// Reviewed, tested
std::ostream& operator << (std::ostream& os, const board_t& board) {
	os << "\n";

	for (int row = 7; row >= 0; row--) {
		// Display separator
		for (int column = 0; column < 8; column++) {
			os << "----";
		}
		
		os << "-\n";

		// Display top padding
		for (int column = 0; column < 8; column++) {
			os << "|   ";
		}
		
		os << "|\n";
		
		// Display row
		for (int column = 0; column < 8; column++) {
			os << "| ";
			auto piece = board.piece({ row, column });

			if (piece.has_value()) {
				piece_kind kind = piece.value().kind;
				player_color color = piece.value().color;

				if (color == player_color::black) {
					os << "\033[31m";
				}

				switch (kind) {
					case piece_kind::pawn:
						os << "p";
						break;
					case piece_kind::knight:
						os << "n";
						break;
					case piece_kind::bishop:
						os << "b";
						break;
					case piece_kind::rook:
						os << "r";
						break;
					case piece_kind::queen:
						os << "q";
						break;
					case piece_kind::king:
						os << "k";
						break;
					default:
						os << "?";
						break;
				}

				os << "\033[0m";
			}
			else {
				os << " ";
			}

			os << " ";
		}

		os << "|\n";

		// Display bottom padding
		for (int column = 0; column < 8; column++) {
			os << "|   ";
		}
		
		os << "|\n";
	}
	
	return os;
};

// EMERGENCY: Consider en passant
std::bitset<265> board_t::to_bitset() {
	if (!this->current_hash.has_value()) {
		this->current_hash = hash_t{};
		std::size_t bit = 0;

		for (int row = 0; row < 8; row++)
		for (int col = 0; col < 8; col++) {
			chess_coordinate_t coord { row, col };
			const auto& mb_piece = this->piece(coord);

			if (mb_piece.has_value()) {
				piece_t current_piece = mb_piece.value();

				int kind_hash = static_cast<int>(current_piece.kind);
				
				for (int c = 0; c < 3; c++) 
					this->current_hash.value().set(bit, (kind_hash >> c) & 1);

				bit += 3;
				this->current_hash.value().set(
					bit++, current_piece.color == player_color::white
				);
			}
			else {
				bit += 4;
			}
		}

		this->current_hash.value().set(bit++, this->white_king_or_left_rook_moved);
		this->current_hash.value().set(bit++, this->white_king_or_right_rook_moved);
		this->current_hash.value().set(bit++, this->black_king_or_left_rook_moved);
		this->current_hash.value().set(bit++, this->black_king_or_right_rook_moved);
		// EMERGENCY: Remove this when bug is fixed
		//this->current_hash.value().set(bit++, this->_turn_color == player_color::white);
	}

	return this->current_hash.value();
}


move_info_t board_t::get_move_info(move_t move){
	return {
		move, 
		this->piece(move.destination),
		this->latest_move(), 
		this->white_king_or_left_rook_moved, 
		this->white_king_or_right_rook_moved, 
		this->black_king_or_left_rook_moved, 
		this->black_king_or_right_rook_moved, 
		this->turns_since_capture_or_pawn_move,
		this->_is_check,
		this->_is_draw,
		this->_pseudolegal_moves,
		this->current_hash
	};
}

void board_t::unmake_move(move_info_t info){
	board_t::hash_t current_hash = this->to_bitset();

	this->position_count[current_hash]--;
	if (this->position_count[current_hash] == 0)
		this->position_count.erase(current_hash);

	this->_turn_color = player_color_fn::opposite(this->_turn_color);

	this->_latest_move = info.last_move;
	this->white_king_or_left_rook_moved = info.white_king_or_left_rook_moved;
	this->white_king_or_right_rook_moved = info.white_king_or_right_rook_moved;
	this->black_king_or_left_rook_moved = info.black_king_or_left_rook_moved;
	this->black_king_or_right_rook_moved = info.black_king_or_right_rook_moved;
	this->turns_since_capture_or_pawn_move = info.turns_since_capture_or_pawn_move;

	if (info.move.promotion_code.has_value()) {
		//we have promotion here
		this->_piece_count(this->_turn_color, info.move.promotion_code.value())--;
		this->_piece_count(this->_turn_color, piece_kind::pawn)++;
		this->piece(info.move.destination)->kind = piece_kind::pawn;
	}

	this->piece(info.move.source) = this->piece(info.move.destination);
	this->piece(info.move.destination) = info.captured_piece;

	piece_t source_piece = this->piece(info.move.source).value();

	if (source_piece.kind == piece_kind::king) {
		// Handle the case where the king castled
		if (std::abs(info.move.destination.column() - info.move.source.column()) > 1) {
			// We move the rook, the king movement gets handled normally after
			int castle_direction = info.move.destination.column() 
				- info.move.source.column();

			int rook_column = castle_direction > 0 ? 7 : 0;
			int new_rook_column = castle_direction > 0 ? 5 : 3;
	
			this->piece({ info.move.source.row(), new_rook_column }) = std::nullopt;
			this->piece({ info.move.source.row(), rook_column }) 
				= piece_t{ piece_kind::rook, this->_turn_color };
		}

		this->_king_coordinates[static_cast<int>(source_piece.color)] =
			info.move.source;
	}
	
	// Handle en passant
	if(source_piece.kind == piece_kind::pawn){
		if(abs(info.move.source.row() - info.move.destination.row()) == abs(info.move.source.column() - info.move.destination.column())){
			if(!(info.captured_piece.has_value())){
				// then it is en passant
				if((this->piece({info.move.source.row(), info.move.destination.column()}).has_value())){
					std::cout << "the piece should not be here: " << this->piece({info.move.source.row(), info.move.destination.column()}).value().kind << "at" << chess_coordinate_t{info.move.source.row(), info.move.destination.column()} << std::endl;
					throw "[Error in unmake move en passant: extra piece at a place where the pawn should eat]";
				}

				//if theres nothing, bring a pawn
				this->piece({info.move.source.row(), info.move.destination.column()}) = piece_t{piece_kind::pawn, player_color_fn::opposite(this->turn_color())};
				this->_piece_count(player_color_fn::opposite(this->turn_color()), piece_kind::pawn)++;
			}
		}
	}

	if (info.captured_piece.has_value()) {
		piece_t captured_piece = info.captured_piece.value();

		this->_piece_count(captured_piece.color, captured_piece.kind)++;

		if (captured_piece.kind == piece_kind::king)
			this->_king_coordinates[static_cast<int>(captured_piece.color)]
				= info.move.destination;
	}

	// Restore cached info
	this->_is_check = info.is_check;
	this->_is_draw = info.is_draw;
	this->current_hash = info.current_hash;
	this->_pseudolegal_moves = info.pseudolegal_moves;
}
