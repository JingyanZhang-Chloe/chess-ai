#include <move_t.h>
#include <iostream>
#include <helper.h>

move_t::move_t(const std::string& move_spec){
    std::cout << "here" << std::endl;
    int i = 0;
	for(char c : move_spec){
        std::cout << "here2" << std::endl;
        if(i == 0){
            source.first = helper::chess_coord_to_int(c).value();
        }
        else if(i == 1){
            source.second = helper::chess_coord_to_int(c).value();
        }
        else if(i == 2){
            destination.first = helper::chess_coord_to_int(c).value();
        }        
        else if(i == 3){
            destination.second = helper::chess_coord_to_int(c).value();
        }
        else if(i == 4){
            promotion_code = c;
        }
        else{
            std::cout << "There's some error occur in the constructor of the move_t" << std::endl;
        };

        i += 1;
    };
}