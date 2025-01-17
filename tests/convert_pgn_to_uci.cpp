#include <convert_pgn_to_uci.h>
#include <board_t.h>

int main(){

    try{
        engine::convert("master_games.pgn", "output_games");
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    
    return 0;
}