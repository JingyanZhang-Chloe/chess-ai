#include <tree_t.h>
#include<filesystem>

int main(){

    try{
    engine::tree_t tree{std::filesystem::current_path()/"output_games"/"twic1553.0"};

    engine::tree_t test_tree(std::filesystem::current_path()/"output_games"/"twic1553.1");

    engine::tree_t test_tree_2(std::filesystem::current_path()/"output_games"/"twic1553.2");

    engine::tree_t test_tree_3(std::filesystem::current_path()/"output_games"/"twic1553.3");

    ///*
    std::cout << "======First merge=========" << std::endl;
    tree.merge(test_tree, tree.root);
    tree.print(tree.root, 3);
    //*/

    std::cout << "======Second merge=========" << std::endl;
    tree.merge(test_tree_2, tree.root);
    tree.print(tree.root, 3);

    std::cout << "======Third merge=========" << std::endl;
    tree.merge(test_tree_3, tree.root);
    tree.print(tree.root, 6);


    } catch (const char* str){
        std::cout << str << std::endl;
    };

    return 0;

}