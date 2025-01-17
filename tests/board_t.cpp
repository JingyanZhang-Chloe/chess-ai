#include <board_t.h>

int main() {
	engine::board_t test;
	
	test.make_move(std::string{"e2e4"});
	test.make_move(std::string{"e7e6"});
	test.make_move(std::string{"b2b3"});
	test.make_move(std::string{"d7d5"});
	test.make_move(std::string{"c1b2"});
	test.make_move(std::string{"d5e4"});
	test.make_move(std::string{"b1c3"});
	test.make_move(std::string{"g8f6"});
	test.make_move(std::string{"g2g4"});
	test.make_move(std::string{"h7h6"});
	test.make_move(std::string{"d1e2"});
	test.make_move(std::string{"f8b4"});
	test.make_move(std::string{"e1c1"});
	test.make_move(std::string{"d8e7"});
	test.make_move(std::string{"c1b1"});
	test.make_move(std::string{"b4a3"});
	test.make_move(std::string{"b2a1"});
	test.make_move(std::string{"a7a5"});
	test.make_move(std::string{"c3e4"});
	test.make_move(std::string{"f6e4"});
	test.make_move(std::string{"e2e4"});
	test.make_move(std::string{"a5a4"});
	test.make_move(std::string{"a1g7"});
	test.make_move(std::string{"a4b3"});
	test.make_move(std::string{"g7h8"});
	test.make_move(std::string{"b3a2"});
	test.make_move(std::string{"b1a1"});
	test.make_move(std::string{"b8c6"});
	test.make_move(std::string{"f1b5"});
	test.make_move(std::string{"c8d7"});
	test.make_move(std::string{"g1f3"});
	test.make_move(std::string{"e7c5"});
	test.make_move(std::string{"e4c4"});
	test.make_move(std::string{"c5f2"});
	test.make_move(std::string{"h1f1"});
	test.make_move(std::string{"f2g2"});
	test.make_move(std::string{"f3e5"});
	test.make_move(std::string{"c6e5"});



	std::cout << test;
};
