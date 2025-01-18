#include <convert_pgn_to_uci.h>
#include <board_t.h>

int main(){

    try{
        engine::convert_dir("input_games", "output_games");
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    
    return 0;
}