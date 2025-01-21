#include <tree_t.h>


using namespace engine;

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
                last_node = new_node;
            }
            else
            {
                // the root already exists
                std::shared_ptr<node_t> new_node {new node_t};
                last_node->transition_map[line] = new_node;
                last_node = new_node;
            };
        };
    };
};

void tree_t::single_print(){

    int key_count = 0;

    std::shared_ptr<node_t> current_node = this->root;

    while(!(current_node->transition_map.empty())){
        key_count += 1;

        for(auto const& [key, value] : current_node->transition_map){
            std::cout << "the " << key_count << "th key is :" << key << std::endl;
            current_node = current_node->transition_map[key];
        };
    };
}
