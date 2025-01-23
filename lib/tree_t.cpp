#include <tree_t.h>
#include <fstream>

using namespace engine;

//tested
tree_t::tree_t(const std::string& inputPath)
: root{new node_t}
{
    std::ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        throw "[Error in tree_t constructor: ] Failed to open input file.";
    };

	int white_wins = 0;
	int draws = 0;
	int white_losses = 0;

    std::string line;
    std::getline(inputFile, line);

	if (line == "1-0")
		white_wins = 1;
	else if (line == "1/2-1/2")
		draws = 1;
	else if (line == "0-1")
		white_losses = 1;

	root->white_wins = white_wins;
	root->draws = draws;
	root->white_losses = white_losses;
		
    std::shared_ptr<node_t> last_node{ this->root };

    while(std::getline(inputFile, line)){
		std::shared_ptr<node_t> new_node{ new node_t };
		new_node->white_wins = white_wins;
		new_node->draws = draws;
		new_node->white_losses = white_losses;

		last_node->transition_map[line] = new_node;
		last_node = new_node;
	}
};

tree_t::tree_t(std::shared_ptr<node_t> root){
    this->root = root;
}

//tested
void tree_t::single_print(){

    int key_count = 0;

    std::shared_ptr<node_t> current_node = this->root;

    while(!(current_node->transition_map.empty())){
        key_count += 1;

        for(auto const& [key, value] : current_node->transition_map){
            std::cout << "the " << key_count << "th key is :" << key << " with white wins: " << current_node->white_wins << " draws: " << current_node->draws << " white_losses: " << current_node->white_losses << std::endl;
            current_node = current_node->transition_map[key];
        };
    };
}


void tree_t::merge(tree_t& tree, std::shared_ptr<node_t> head){
	head->white_wins = 0;
	head->draws = 0;
	head->white_losses = 0;

    for(auto const& [move, node] : tree.root->transition_map){
        // here [move, node] is a pair in the current_node for tree map
        // here, move is the set of all the moves for the root of the tree we want to merge
        // the node is the set of all the nodes the moves points to
        
        //check if this move is in our this
		if (head->transition_map.contains(move)) {
			tree_t other_subtree { node };
			this->merge(other_subtree, head->transition_map[move]);
		}
		else {
			head->transition_map[move] = node;
		}

		head->white_wins += head->transition_map[move]->white_wins;
		head->draws += head->transition_map[move]->draws;
		head->white_losses += head->transition_map[move]->white_losses;
    };
}

void tree_t::print_moves(std::shared_ptr<node_t> node){
    node->print();
}

void tree_t::print_children(std::shared_ptr<node_t> node){
    for(auto const& [key, value] : node->transition_map){
        std::cout << "the move: " << key << " has children ";
        print_moves(value);
    };
}

void tree_t::node_t::print(){
    // print the moves
    std::cout << "In this node we have moves: [ ";
    for(auto const& [key, value] : this->transition_map){
        std::cout << key << " ";
    }
    std::cout << " ]" << std::endl;
}

void tree_t::print(std::shared_ptr<node_t> begin, int depth){

    if(depth == 0){
        return;
    }

    std::cout << " ##### Moves in depth " << depth << " #####" << std::endl;
    std::cout << "[ ";
    for(auto const& [key, value] : begin->transition_map){
        std::cout << key << " ";
    };
    std::cout << " ]" << std::endl;

    for(auto const& [key, value] : begin->transition_map){
        
        std::cout << "the move " << key << " has children ";
        print(value, depth - 1);
        std::cout << " " << std::endl;
        
    };
}
