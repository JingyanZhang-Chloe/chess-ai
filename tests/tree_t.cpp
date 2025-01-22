#include <tree_t.h>
#include<filesystem>

int main(){
    engine::tree_t tree{std::filesystem::current_path()/"output_games"/"game[1]1.uci"};

    engine::tree_t test_tree(std::filesystem::current_path()/"output_games"/"game[1]2.uci");

    engine::tree_t test_tree_2(std::filesystem::current_path()/"output_games"/"game[1]4.uci");

    tree.merge(test_tree, tree.root);

    tree.merge(test_tree_2, tree.root);

    tree.print_moves(tree.root);
    tree.print_children(tree.root);

    return 0;

}