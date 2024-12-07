#include <move_t.h>
#include <iostream>
#include <helper.h>


move_t::move_t(const std::string& move_spec){
    if(move_spec.size() == 4 || move_spec.size() == 5){
        source.first = helper::chess_coord_to_int(move_spec[0]).value();
        source.second = helper::chess_coord_to_int(move_spec[1]).value();
        destination.first = helper::chess_coord_to_int(move_spec[2]).value();
        destination.second = helper::chess_coord_to_int(move_spec[3]).value();
        if(move_spec.size() == 5){
            switch (move_spec[4])
            {
            case 'k':
                promotion_code = piece_kind::knight;
                break;
            case 'r':
                promotion_code = piece_kind::rook;
                break;
            case 'q':
                promotion_code = piece_kind::queen;
                break;
            case 'b':
                promotion_code = piece_kind::bishop;
                break;
            default:
                throw "Error in promotion code";
                break;
            };
        }
    }else{
        throw "Error in the constructor of the move_t";
    };
};

std::ostream& operator << (std::ostream& os, move_t& move){
    if(move.promotion_code.has_value()){
        return os << "From: " << move.source.first << move.source.second << " To: " << move.destination.first << move.destination.second << " With promotion code: " << move.promotion_code.value();
    }
    else{
        return os << "From: " << move.source.first << move.source.second << " To: " << move.destination.first << move.destination.second;
    }
}
