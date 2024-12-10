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

std::ostream& operator << (std::ostream& os, const move_t& move){
    if(move.promotion_code.has_value()){
        return os << "From: " << move.source << " To: " << move.destination << " With promotion code: " << move.promotion_code.value();
    }
    else{
        return os << "From: " << move.source << " To: " << move.destination;
    }
}
