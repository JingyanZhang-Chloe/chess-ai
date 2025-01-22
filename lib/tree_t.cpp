#include <tree_t.h>


using namespace engine;

//tested
tree_t::tree_t(const std::string& inputPath){
    std::ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        throw "[Error in tree_t constructor: ] Failed to open input file.";
    };

    std::string line;
    std::string white_win = "1-0";
    std::string draw = "1/2-1/2";
    std::string white_loss = "0-1";

    int white_wins = 0;
    int draws = 0;
    int white_losses = 0;
    std::shared_ptr<node_t> last_node {new node_t};

    while(std::getline(inputFile, line)){
        if(line.contains("-")){
            //this is a score
            if(line == white_win){
                white_wins += 1;
            }
            else if(line == draw){
                draws += 1;
            }
            else if(line == white_loss){
                white_losses += 1;
            }
            else{
                throw "[Error in tree_t constructor: ] invalid score line";
            };

        }
        else
        {
            if(this->root == nullptr){
                this->root = std::shared_ptr<node_t>{new node_t};
                std::shared_ptr<node_t> new_node {new node_t};
                this->root->transition_map[move_t{line}] = new_node;
                this->root->white_wins = white_wins;
                this->root->draws = draws;
                this->root->white_losses = white_losses;
                
                last_node = new_node;
            }
            else
            {
                // the root already exists
                std::shared_ptr<node_t> new_node {new node_t};
                last_node->transition_map[move_t{line}] = new_node;
                last_node->white_wins = white_wins;
                last_node->draws = draws;
                last_node->white_losses = white_losses;
                last_node = new_node;
            };
        };
    };
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


void tree_t::merge(tree_t& tree, std::shared_ptr<node_t> current_node_for_this){
    if(tree.root->transition_map.empty()){
        return;
    }

    if(this->root == nullptr){
        *this = tree;
    }

    if(current_node_for_this == nullptr){
        // we are working on the 0th depth
        current_node_for_this = this->root;
    }

    current_node_for_this->white_wins += tree.root->white_wins;
    current_node_for_this->draws += tree.root->draws;
    current_node_for_this->white_losses = tree.root->white_losses;

    
    for(auto const& [key, value] : tree.root->transition_map){
        // here [key, value] is a pair in the current_node for tree map
        // here, key is the set of all the moves for the root of the tree we want to merge
        // the value is the set of all the nodes the moves points to
        
        //check if this key is in our this
        for(auto const& [aim_key, aim_value] : current_node_for_this->transition_map){
            if(key == aim_key){
                std::cout << "COMBINE"<< std::endl;
                // if we find one that matches

                current_node_for_this = aim_value;

                tree_t new_tree{value};

                this->merge(new_tree, current_node_for_this);

                return;
            };

        };

        std::cout <<"ADD" << std::endl;

        // if we do not find the match, we just add this key in our current node for this
        current_node_for_this->transition_map.insert(std::make_pair(key, value));

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
