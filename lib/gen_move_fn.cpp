#include <gen_move_fn.h>

using namespace engine;

// Reviewed
// Returns whether we should stop (blocked by a piece)
bool add_if_empty_or_opponent(
	chess_coordinate_t source,
	chess_coordinate_t destination, 
	player_color current_player, 
	const board_t& board,
	std::vector<move_t>& moves
) {
	auto piece = board.piece(destination);

	if (piece.has_value()) {
		if (piece.value().color != current_player) 
			moves.push_back({ source, destination }); 

		return false;
	}
	else {
		moves.push_back({ source, destination });

		return true;
	}
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::rook>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
    std::vector<move_t> re;
	
	// Loop right
    for (int column = source.column() + 1; column < 8; column++) {
        chess_coordinate_t destination { source.row(), column };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

    // Loop left
    for (int column = source.column() - 1; column >= 0; column--) {
        chess_coordinate_t destination { source.row(), column };
		
		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

    // Loop down
    for (int row = source.row() - 1; row >= 0; row--) {
        chess_coordinate_t destination { row, source.column() };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

	// Loop up
    for (int row = source.row() + 1; row < 8; row++) {
        chess_coordinate_t destination { row, source.column() };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
    }

    return re;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::bishop>(
	chess_coordinate_t source, player_color color, const board_t& board
) {
    std::vector<move_t> re;
	
	// Loop south-west
	for (int k = 1; source.row() - k >= 0 && source.column() - k >= 0; k++) {
		chess_coordinate_t destination { source.row() - k, source.column() - k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

	// Loop north-west
	for (int k = 1; source.row() + k < 8 && source.column() - k >= 0; k++) {
		chess_coordinate_t destination { source.row() + k, source.column() - k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

	// Loop south-east
	for (int k = 1; source.row() - k >= 0 && source.column() + k < 8; k++) {
		chess_coordinate_t destination { source.row() - k, source.column() + k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

	// Loop north-east
	for (int k = 1; source.row() + k < 8 && source.column() + k < 8; k++) {
		chess_coordinate_t destination { source.row() + k, source.column() + k };

		if (add_if_empty_or_opponent(source, destination, color, board, re)) break;
	}

    return re;
}

// Reviewed
template<>
std::vector<move_t> engine::gen_moves<piece_kind::queen>(chess_coordinate_t coord, player_color color, const board_t& board){
    std::vector v1 = gen_moves<piece_kind::bishop>(coord, color, board);
    std::vector v2 = gen_moves<piece_kind::rook>(coord, color, board);

    v1.insert(v1.end(), v2.begin(), v2.end());

    return v1;
}

std::vector<move_t> pawn_simple_captures(
	chess_coordinate_t source, player_color color, const board_t& board
) {
	int y_direction = 1;
	int promoting_row = 7;

	if (color == player_color::black) {
		y_direction = -1;
		promoting_row = 0;
	}
	
	std::vector<move_t> capturing_advances;
	capturing_advances.reserve(2);

	for (int dx : { -1, 1 }) {
		chess_coordinate_t advance { source.row() + y_direction, source.column() + dx };
		add_if_empty_or_opponent(source, advance, color, board, capturing_advances);
	}

	if (source.row() + y_direction == promoting_row) {
		std::vector<move_t> re;
		re.reserve(capturing_advances.size() * 4);

		for (const move_t& move : capturing_advances) 
		for (piece_kind promotion_code : { 
			piece_kind::knight, piece_kind::bishop, 
			piece_kind::rook, piece_kind::queen 
		})
			re.push_back({ move.source, move.destination, promotion_code });

		return re;
	}
	else return capturing_advances;
}

//helper functions for the pawn
std::vector<move_t> get_eat_move(chess_coordinate_t coord, player_color color, const board_t& board){
    // this function return an std::vector for every option move that a pawn can take to eat some other piece
    //consider en passant

    std::vector<move_t> re;

    //this is the begining of the game, so there will be no last_move
    if (!board.last_move.has_value()) return re;

	// simple captures
	

    // for player of color white 
    if(color == player_color::white){
        
        if(coord.row() < 7){
            if(coord.row() == 5){

                move_t last_move = board.last_move.value();
                //en passant, since the white pawn will move to the position where the black pawn leave, then this aim must be empty, then this move will not be gen below
                //so we will not add twice for the same move
                //impossible to make promo by en passant
            
                //from left
                if(last_move.source.row() == coord.row() + 2 && last_move.source.column() == coord.column() - 1 && last_move.destination.row() == coord.row() && last_move.destination.column() == coord.column() - 1 && board.piece(last_move.source).value().kind == piece_kind::pawn){
                    chess_coordinate_t aim{coord.row() + 1, coord.column() - 1};
                    re.push_back(move_t{coord, aim});
                    //eat the pawn!!!!!
                };

                //from right
                if(last_move.source.row() == coord.row() + 2 && last_move.source.column() == coord.column() + 1 && last_move.destination.row() == coord.row() && last_move.destination.column() == coord.column() + 1 && board.piece(last_move.source).value().kind == piece_kind::pawn){
                    chess_coordinate_t aim{coord.row() + 1, coord.column() + 1};
                    re.push_back(move_t{coord, aim});
                    //eat the pawn!!!!!
                };
            };

            //left top
            if(coord.column() > 0){
                chess_coordinate_t aim = chess_coordinate_t{coord.column() - 1, coord.row() + 1};
                if(board.piece(aim).has_value()){
                    if(board.piece(aim).value().color == player_color::black){
                        //we eat this
                        if(aim.row() == 7){
                            //we have promo
                            for(piece_kind promo : std::vector<piece_kind>{piece_kind::bishop, piece_kind::knight, piece_kind::queen, piece_kind::rook}){
                                move_t move{coord, aim, promo};
                                re.push_back(move);
                            };
                        }
                        else{
                            re.push_back(move_t{coord, aim});
                        };
                    };
                };
            };

            //right top
            if(coord.column() < 7){
                chess_coordinate_t aim = chess_coordinate_t{coord.column() + 1, coord.row() + 1};
                if(board.piece(aim).has_value()){
                    if(board.piece(aim).value().color == player_color::black){
                        //we eat this
                        if(aim.row() == 7){
                            //we have promo
                            for(piece_kind promo : std::vector<piece_kind>{piece_kind::bishop, piece_kind::knight, piece_kind::queen, piece_kind::rook}){
                                move_t move{coord, aim, promo};
                                re.push_back(move);
                            };
                        }
                        else{
                            re.push_back(move_t{coord, aim});
                        };
                    };
                };
            };
        };
    }
    else
    {
        // for player color of black
        if(coord.row() > 0){

            if(coord.row() == 4){

                move_t last_move = board.last_move.value();
                //en passant, since the black pawn will move to the position where the white pawn leave, then this aim must be empty, then this move will not be gen below
                //so we will not add twice for the same move
                //impossible to make promo by en passant
            
                //from left
                if(last_move.source.row() == coord.row() - 2 && last_move.source.column() == coord.column() - 1 && last_move.destination.row() == coord.row() && last_move.destination.column() == coord.column() - 1 && board.piece(last_move.source).value().kind == piece_kind::pawn){
                    chess_coordinate_t aim{coord.row() - 1, coord.column() - 1};
                    re.push_back(move_t{coord, aim});
                    //eat the pawn!!!!!
                };

                //from right
                if(last_move.source.row() == coord.row() - 2 && last_move.source.column() == coord.column() + 1 && last_move.destination.row() == coord.row() && last_move.destination.column() == coord.column() + 1 && board.piece(last_move.source).value().kind == piece_kind::pawn){
                    chess_coordinate_t aim{coord.row() - 1, coord.column() + 1};
                    re.push_back(move_t{coord, aim});
                    //eat the pawn!!!!!
                };

            };

            //down left
            if(coord.column() > 0){
                chess_coordinate_t aim = chess_coordinate_t{coord.column() - 1, coord.row() - 1};
                if(board.piece(aim).has_value()){
                    if(board.piece(aim).value().color == player_color::white){
                        //we eat this
                        if(aim.row() == 0){
                            //we have promo
                            for(piece_kind promo : std::vector<piece_kind>{piece_kind::bishop, piece_kind::knight, piece_kind::queen, piece_kind::rook}){
                                move_t move{coord, aim, promo};
                                re.push_back(move);
                            };
                        }
                        else{
                            re.push_back(move_t{coord, aim});
                        };
                    };
                
                };
            };

            // down right
            if(coord.column() < 7){
                chess_coordinate_t aim = chess_coordinate_t{coord.column() + 1, coord.row() - 1};
                if(board.piece(aim).has_value()){
                    if(board.piece(aim).value().color == player_color::white){
                        //we eat this
                        if(aim.row() == 0){
                            //we have promo
                            for(piece_kind promo : std::vector<piece_kind>{piece_kind::bishop, piece_kind::knight, piece_kind::queen, piece_kind::rook}){
                                move_t move{coord, aim, promo};
                                re.push_back(move);
                            };
                        }
                        else{
                            re.push_back(move_t{coord, aim});
                        };
                    };
                };
            };

        };
    };


    return re;

}

std::vector<move_t> add_promo(move_t move, player_color color){

    std::vector<move_t> re;

    if(color == player_color::white){
        if(move.destination.row() == 7){
            //we have promo
            for(piece_kind promo : std::vector<piece_kind>{piece_kind::bishop, piece_kind::knight, piece_kind::queen, piece_kind::rook}){
                move_t the_move{move.source, move.destination, promo};
                re.push_back(the_move);
            };
        };
    }
    else
    {
        if(move.destination.row() == 0){
            //we have promo
            for(piece_kind promo : std::vector<piece_kind>{piece_kind::bishop, piece_kind::knight, piece_kind::queen, piece_kind::rook}){
                move_t the_move{move.source, move.destination, promo};
                re.push_back(the_move);
            };
        };
    };
}


template<>
std::vector<move_t> engine::gen_moves<piece_kind::pawn>(chess_coordinate_t coord, player_color color, const board_t& board){

    std::vector<move_t> re;

    if(color == player_color::white){
        if(coord.row() == 1){
            //this is their first move
            for(int front = 0; front <= 2; ++front){
                chess_coordinate_t aim{coord.row() + front, coord.column()};
                if(board.piece(aim).has_value()){
                    break;
                };

                re.push_back(move_t{coord, aim});
            };
        }
        else{
            //this is not their first move
            if(coord.row() < 7){
                chess_coordinate_t aim{coord.row() + 1, coord.column()};
                if(board.piece(aim).has_value() != true){
                    move_t move{coord, aim};
                    if(aim.row() == 7){
                        re.insert(re.begin(), add_promo(move, player_color::white).begin(), add_promo(move, player_color::white).end());
                    }
                    else{
                        re.push_back(move);
                    };
                };
            };
        };

        re.insert(re.begin(), get_eat_move(coord, player_color::white, board).begin(), get_eat_move(coord, player_color::white, board).end());

    }
    else
    {
        if(coord.row() == 6){
            //this is their first move
            for(int front = 0; front <= 2; ++front){
                chess_coordinate_t aim{coord.row() - front, coord.column()};
                if(board.piece(aim).has_value()){
                    break;
                };

                re.push_back(move_t{coord, aim});
            };
        }
        else{
            //this is not their first move
            if(coord.row() > 0){
                chess_coordinate_t aim{coord.row() - 1, coord.column()};
                if(board.piece(aim).has_value() != true){
                    move_t move{coord, aim};
                    if(aim.row() == 0){
                        re.insert(re.begin(), add_promo(move, player_color::black).begin(), add_promo(move, player_color::black).end());
                    }
                    else{
                        re.push_back(move);
                    };
                };
            };
        };

        re.insert(re.begin(), get_eat_move(coord, player_color::black, board).begin(), get_eat_move(coord, player_color::black, board).end());

    };

    return re;

}


//helper function for the king 
bool is_valid(chess_coordinate_t aim, board_t board){
    // return true if the aim for one move is in the board
    if(aim.column() >= 0 && aim.column() <= 7 && aim.row() >= 0 && aim.row() <= 7){
        return true;
    }
    else{
        return false;
    };
}

template<>
std::vector<move_t> engine::gen_moves<piece_kind::king>(chess_coordinate_t coord, player_color color, const board_t& board){
    // the legal moves for king will not include any move that can cause in check

    std::vector<move_t> re;


    std::vector<int> movement{-1 , 0 , 1};
    for(int column_move : movement){
        for(int row_move : movement){
            chess_coordinate_t aim{coord.row() + row_move, coord.column() + column_move};
            if(coord != aim && is_valid(aim, board)){
                if(board.piece(aim).has_value()){
                    if(board.piece(aim).value().color != color){

                        if(!(board.is_in_check(color, aim))){
                            re.push_back(move_t{coord, aim});
                        }
                        
                    };
                }
                else{
                    if(!(board.is_in_check(color, aim))){
                        re.push_back(move_t{coord, aim});
                    }
                };
            };
        };
    };

    // castling
    //here we assume castling rights already check: the rook and the king never move,
    //The king is not in check, and is not moving into check or through check. (It is okay if the rook is being attacked, though),
    //There are no pieces between the king and the rook.
    if(color == player_color::white){
        if(board.castling_rights.right_white){
            chess_coordinate_t aim{coord.row(), coord.column() + 2};
            re.push_back(move_t{coord, aim});
        };

        if(board.castling_rights.left_white){
            chess_coordinate_t aim{coord.row(), coord.column() - 2};
            re.push_back(move_t{coord, aim});
        };
    }
    else
    {
        if(board.castling_rights.right_black){
            chess_coordinate_t aim{coord.row(), coord.column() + 2};
            re.push_back(move_t{coord, aim});
        };

        if(board.castling_rights.left_black){
            chess_coordinate_t aim{coord.row(), coord.column() - 2};
            re.push_back(move_t{coord, aim});
        };
    };

	return re;
};

