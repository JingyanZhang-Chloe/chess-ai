#include "include/move_t.h"
#include <iostream>
#include "include/chess_coordinate_t.h"

using namespace engine;

move_t::move_t(const std::string& move_spec) : source{"a1"}, destination{"a1"} {
    if(move_spec.size() == 4 || move_spec.size() == 5){
        this->source = engine::chess_coordinate_t{move_spec};
        std::string sub = move_spec.substr(2,4);
        this->destination = engine::chess_coordinate_t{sub};
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
        throw "[Move Error] Error in the constructor of the move_t";
    };
};

move_t::move_t(chess_coordinate_t start, chess_coordinate_t end) : source{0,0}, destination{0,0}{
    // we may assume the start coordinate is valid
    if(end.column() < 0 || end.column() > 7){
        throw "[Move Constractor Error] invalid end column";
    };

    if(end.row() < 0 || end.row() > 7){
        throw "[Move Constractor Error] invalid end row";
    };

    this->source = start;
    this->destination = end;
};

std::ostream& operator << (std::ostream& os, const move_t& move){
    if(move.promotion_code.has_value()){
        return os << "From: " << move.source << " To: " << move.destination << " With promotion code: " << move.promotion_code.value();
    }
    else{
        return os << "From: " << move.source << " To: " << move.destination;
    }
}

bool move_t::operator == (const move_t& move) const = default;