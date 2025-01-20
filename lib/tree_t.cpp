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
            
        };
    };


}
