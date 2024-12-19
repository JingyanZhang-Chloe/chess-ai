#include<iostream>
#include <move_t.h>

int main(int argc, char const *argv[]) {
    try {
        engine::move_t move = engine::move_t{"g1a1k"};
        std::cout << move << std::endl;
		
		return 0;
    }
	catch(const char* str){
        std::cout << str << std::endl;
    }

    return 1;
};
