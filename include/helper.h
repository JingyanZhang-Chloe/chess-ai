#include <optional>

namespace helper {
	std::optional<int> chess_coord_to_int(char c) {
		int code_num = c - '0';
		int code_alph = c - 'a';
		int code;

		if (1 <= code_num && code_num < 9) {
			return code_num;
		}
		else if (0 <= code_alph && code_alph < 8) {
			return code_alph + 1;
		}
		else {
			return std::nullopt;
		}
	};
};
