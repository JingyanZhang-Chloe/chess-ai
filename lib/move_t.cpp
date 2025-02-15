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

move_t::move_t(chess_coordinate_t start, chess_coordinate_t end, piece_kind k) : source{0,0}, destination{0,0}{
    // we may assume the start coordinate is valid
    if(end.column() < 0 || end.column() > 7){
        throw "[Move Constractor Error] invalid end column";
    };

    if(end.row() < 0 || end.row() > 7){
        throw "[Move Constractor Error] invalid end row";
    };

    this->source = start;
    this->destination = end;
    this->promotion_code = k;
}

std::ostream& operator << (std::ostream& os, const move_t& move){
    if(move.promotion_code.has_value()){
        return os <<  move.source << move.destination  << move.promotion_code.value();
    }
    else{
        return os << move.source << move.destination;
    }
}

bool move_t::operator == (const move_t& move) const = default;

std::size_t std::hash<move_t>::operator()(const move_t& move) const {
    std::hash<chess_coordinate_t> coordinate_hash;
    std::hash<piece_kind> piece_kind_hash;

    if(move.promotion_code.has_value()){
        std::hash<piece_kind> piece_kind_hash;
        return (coordinate_hash(move.source) ^ coordinate_hash(move.destination)) ^ piece_kind_hash(move.promotion_code.value());
    }

    return (coordinate_hash(move.source) ^ coordinate_hash(move.destination));
}