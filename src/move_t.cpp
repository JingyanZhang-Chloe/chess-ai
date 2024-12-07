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
            promotion_code = move_spec[4];
        }
    }else{
        throw "There's some error occur in the constructor of the move_t";
    };
};