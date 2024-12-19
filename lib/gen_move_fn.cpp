#include <gen_move_fn.h>

using namespace engine;

template<>
std::vector<move_t> engine::gen_moves<piece_kind::rook>(chess_coordinate_t coord, player_color color, const board_t& board) {
    std::vector<move_t> re;

    //loop through down column
    for(int column = column + 1; column <= 7; ++column){
        chess_coordinate_t pos {coord.row(), column};
        if(board.piece(pos).has_value()){
            if(board.piece(pos).value().color == color){
                break;
            }
            else{
                // it is a different color, EAT it!!
                re.push_back(move_t{coord, pos});
                //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                break;
            };

        }
        else{
            re.push_back(move_t{coord, pos});
        };
    };


    //loop through up column
    for(int column = coord.column() - 1; column >= 0; --column){
        chess_coordinate_t pos {coord.row(), column};
        if(board.piece(pos).has_value()){
            if(board.piece(pos).value().color == color){
                break;
            }
            else{
                // it is a different color, EAT it!!
                re.push_back(move_t{coord, pos});
                //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                break;
            };

        }
        else{
            re.push_back(move_t{coord, pos});
        };
    };

    //loop through left row
    for(int row = coord.row() - 1; row >= 0; --row){
        chess_coordinate_t pos {row, coord.column()};
        if(board.piece(pos).has_value()){
            if(board.piece(pos).value().color == color){
                break;
            }
            else{
                // it is a different color, EAT it!!
                re.push_back(move_t{coord, pos});
                //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                break;
            };

        }
        else{
            re.push_back(move_t{coord, pos});
        };
    };

    //loop through right row
    for(int row = coord.row() + 1; row <= 7; ++row){
        chess_coordinate_t pos {row, coord.column()};
        if(board.piece(pos).has_value()){
            if(board.piece(pos).value().color == color){
                break;
            }
            else{
                // it is a different color, EAT it!!
                re.push_back(move_t{coord, pos});
                //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                break;
            };

        }
        else{
            re.push_back(move_t{coord, pos});
        };
    };
};


template<>
std::vector<move_t> engine::gen_moves<piece_kind::bishop>(chess_coordinate_t coord, player_color color, const board_t& board){

    std::vector<move_t> re;

    // the up left corner
    for(int row = coord.row() - 1; row >= 0; --row){
        for(int column = coord.column() - 1; column >= 0; --column){
            if(abs(row - coord.row()) == abs(column - coord.column())){
                chess_coordinate_t pos = chess_coordinate_t{row, column};
                if(board.piece(pos).has_value()){
                    if(board.piece(pos).value().color == color){
                        break;
                    }
                    else{
                    // it is a different color, EAT it!!
                    re.push_back(move_t{coord, pos});
                    //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                    break;
                    };

                }
                else{
                    re.push_back(move_t{coord, pos});
                };
            };
        };
    };

    // the up right corner
    for(int row = coord.row() + 1; row <= 7; ++row){
        for(int column = coord.column() - 1; column >= 0; --column){
            if(abs(row - coord.row()) == abs(column - coord.column())){
                chess_coordinate_t pos = chess_coordinate_t{row, column};
                if(board.piece(pos).has_value()){
                    if(board.piece(pos).value().color == color){
                        break;
                    }
                    else{
                    // it is a different color, EAT it!!
                    re.push_back(move_t{coord, pos});
                    //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                    break;
                    };

                }
                else{
                    re.push_back(move_t{coord, pos});
                };
            };
        };
    };

    // the down left corner
    for(int row = coord.row() - 1; row >= 0; --row){
        for(int column = coord.column() + 1; column <= 7; ++column){
            if(abs(row - coord.row()) == abs(column - coord.column())){
                chess_coordinate_t pos = chess_coordinate_t{row, column};
                if(board.piece(pos).has_value()){
                    if(board.piece(pos).value().color == color){
                        break;
                    }
                    else{
                    // it is a different color, EAT it!!
                    re.push_back(move_t{coord, pos});
                    //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                    break;
                    };

                }
                else{
                    re.push_back(move_t{coord, pos});
                };
            };
        };
    };

    // the down right corner
    for(int row = coord.row() + 1; row <= 7; ++row){
        for(int column = coord.column() + 1; column <= 7; ++column){
            if(abs(row - coord.row()) == abs(column - coord.column())){
                chess_coordinate_t pos = chess_coordinate_t{row, column};
                if(board.piece(pos).has_value()){
                    if(board.piece(pos).value().color == color){
                        break;
                    }
                    else{
                    // it is a different color, EAT it!!
                    re.push_back(move_t{coord, pos});
                    //std::cout << "WE EAT YOUR " << board.piece(pos).value().kind << std::endl;
                    break;
                    };

                }
                else{
                    re.push_back(move_t{coord, pos});
                };
            };
        };
    };
};


template<>
std::vector<move_t> engine::gen_moves<piece_kind::queen>(chess_coordinate_t coord, player_color color, const board_t& board){

    std::vector v1 = gen_moves<piece_kind::bishop>(coord, color, board);

    std::vector v2 = gen_moves<piece_kind::rook>(coord, color, board);

    v1.insert(v1.end(), v2.begin(), v2.end());

    return v1;
}