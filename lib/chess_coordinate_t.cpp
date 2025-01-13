#include <string>
#include <chess_coordinate_t.h>

using namespace engine;

// Reviewed
chess_coordinate_t::chess_coordinate_t(int row, int column) {
	if (row < 0 || row > 7 || column < 0 || column > 7) {
		throw "[Chess Coordinate Error] Row or column out of range.";
	}

	this->coordinate = static_cast<int8_t>(row * 8 + column);
}

// Reviewed
chess_coordinate_t::chess_coordinate_t(const std::string& coordinate_spec) {
	if (coordinate_spec.size() < 2) {
		throw "[Chess Coordinate Error] The specification string is too small.";
	}

	// Convert the first character (letter) into the column integer
	char column_char = std::tolower(coordinate_spec[0]);
	int column = column_char - 'a';

	char row_char = coordinate_spec[1];
	int row = row_char - '1';

	if (column < 0 || column > 7 || row < 0 || row > 7) 
		throw "[Chess Coordinate Error] The column and row indicators in the"
		" specification string are invalid.";
	

	this->coordinate = static_cast<int8_t>(row * 8 + column);
}

// Reviewed
int8_t chess_coordinate_t::row() {
	return this->coordinate >> 3;
}

// Reviewed
int8_t chess_coordinate_t::column() {
	return this->coordinate & 0b111;
}

// Reviewed
char chess_coordinate_t::column_as_char() {
	int col = this->column();

	return static_cast<char>(static_cast<int>('a') + col);
}

// Reviewed
int8_t chess_coordinate_t::index() {
	return this->coordinate;
}

// Reviewed
std::ostream& operator<<(std::ostream& os, chess_coordinate_t coord) {
	int row = coord.row() + 1;
	char column = coord.column_as_char();

	return os << column << row;
}

// Reviewed
bool chess_coordinate_t::operator == (const chess_coordinate_t& coor) const = default;
