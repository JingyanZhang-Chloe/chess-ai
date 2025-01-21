#include <tree_t.h>
#include<filesystem>

int main(){
    engine::tree_t tree{std::filesystem::current_path()/"output_games"/"game[1]1.uci"};

    tree.single_print();

}