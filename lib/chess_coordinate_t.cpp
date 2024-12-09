#include <string>
#include "include/chess_coordinate_t.h"

using namespace engine;

chess_coordinate_t::chess_coordinate_t(const std::string& coordinate_spec) {
	if (coordinate_spec.size() < 2) {
		throw "[Chess Coordinate Error] The specification string is too small.";
	}

	// Convert the first character (letter) into the column integer
	char column_char = std::tolower(coordinate_spec[0]);
	uint8_t column = column_char - 'a';

	std::cout << "col: " << "\"" << column << "\"";

	char row_char = coordinate_spec[1];
	uint8_t row = column_char - '1';

	if (column < 0 || column > 7 || row < 0 || row > 7) {
		std::cout << "Row: " << row << "Column: " << column << std::endl;
		throw "[Chess Coordinate Error] The column and row indicators in the"
			" specification string are invalid.";
	}

	this->coordinate = row * 8 + column;
};

uint8_t chess_coordinate_t::row() {
	return this->coordinate / 8;
};

uint8_t chess_coordinate_t::column() {
	return this->coordinate % 8;
};

char chess_coordinate_t::column_as_char() {
	uint8_t col = this->column();

	return static_cast<char>(static_cast<int>('A') + col);
};

std::ostream& operator<<(std::ostream& os, chess_coordinate_t coord) {
	uint8_t row = coord.row();
	char column = coord.column_as_char();

	return os << column << row;
};
