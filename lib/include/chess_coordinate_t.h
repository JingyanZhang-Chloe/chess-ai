#pragma once
#include <iostream>
#include <string>
#include <cstdint>

namespace engine {

class chess_coordinate_t {
	public:
		chess_coordinate_t(int row, int column);
		chess_coordinate_t(int index);
		chess_coordinate_t(const std::string&);
		
		int8_t row();
		int8_t column();
		char column_as_char();
		int8_t index();

		bool operator == (const chess_coordinate_t& coor) const;

		std::string into_string();

		friend std::hash<chess_coordinate_t>;
	
	private:
		int8_t coordinate;
};

};

std::ostream& operator << (std::ostream&, engine::chess_coordinate_t);

template<>
class std::hash<engine::chess_coordinate_t> {
public:
	hash() = default;

	std::size_t operator()(const engine::chess_coordinate_t&) const;
};