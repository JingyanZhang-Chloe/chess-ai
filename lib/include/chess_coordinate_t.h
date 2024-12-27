#pragma once
#include <iostream>
#include <string>
#include <cstdint>

namespace engine {

class chess_coordinate_t {
	public:
		chess_coordinate_t(int row, int column);
		chess_coordinate_t(const std::string&);
		
		int8_t row();
		int8_t column();
		char column_as_char();
		int8_t index();

		bool operator == (const chess_coordinate_t& coor) const;
	
	private:
		int8_t coordinate;
};

};

std::ostream& operator << (std::ostream&, engine::chess_coordinate_t);


