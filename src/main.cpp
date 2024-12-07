#include <move_t.h>

int main() {

	move_t move = move_t("g1h1k");
	std::cout << move.source.first << move.source.second << " " << move.destination.first << move.destination.second << " " << move.promotion_code.value_or(' ') << std::endl;

	return 0;
}
