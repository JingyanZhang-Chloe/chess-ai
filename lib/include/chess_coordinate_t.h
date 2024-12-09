#include <iostream>
#include <string>
#include <cstdint>

namespace engine {

class chess_coordinate_t {
	public:
		chess_coordinate_t(const std::string&);
		
		uint8_t row();
		uint8_t column();
		char column_as_char();
	
	private:
		uint8_t coordinate;
};

std::ostream& operator<<(std::ostream&, chess_coordinate_t);

};
