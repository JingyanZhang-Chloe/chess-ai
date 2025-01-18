#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <stdexcept>
#include <board_t.h>
#include <convert_pgn_to_uci.h>
#include <filesystem>
#include <move_convert_fn.h>

using namespace engine;


std::string engine::san_to_uci(std::string& san, board_t& board){

    //std::cout << "now we are in the function of san_to_uci" << std::endl;

    //std::cout << "the board is like this:" << std::endl;

    //std::cout << board << std::endl;

    std::string move_str;

    for(auto& move : board.pseudolegal_moves()){
        if(!(board.is_legal(move))){
            continue;
        }

        //std::cout << "***For move: " << move << " we get the san value by to_san as: " << to_san(move, board) << std::endl;
        
        if(to_san(move, board) == san){
            if(move.promotion_code.has_value()){
                switch (move.promotion_code.value())
                {
                case piece_kind::queen : 
                    return move.source.into_string() + move.destination.into_string() + "q";
                    break;

                case piece_kind::bishop :
                    return move.source.into_string() + move.destination.into_string() + "b";
                    break;

                case piece_kind::knight :
                    return move.source.into_string() + move.destination.into_string() + "k";
                    break;

                case piece_kind::rook :
                    return move.source.into_string() + move.destination.into_string() + "r";
                    break;
                
                default:
                    break;
                };
            };

            return move.source.into_string() + move.destination.into_string();
        }
    }

    std::cout << "the san we don't find value is " << san << std::endl;

    std::cout << "the board is \n" << board << std::endl;

    for(auto& move : board.pseudolegal_moves()){
        if(!(board.is_legal(move))){
            continue;
        }

        //std::cout << "***For move: " << move << " we get the san value by to_san as: " << to_san(move, board) << std::endl;
    }

    throw "[Error in san_to_uci: ] invalid san value";
};

void engine::convert(const std::string& inputPath, const std::string& outputDir, int filenumber){
    //since we will output each game as a file, so we take the second arg as Dir(instead of Path)
    if(!std::filesystem::exists(outputDir)){
        std::filesystem::create_directory(outputDir);
    };

    std::ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        throw "[Error in convert: ] Failed to open input file.";
    };

    std::string line;
    std::string game; // to store each game as a string
    int gamenumber = 0;

    while(std::getline(inputFile, line)){

        // this is the end of one game block
        if(line.empty() && game.empty() != true){
            gamenumber ++;
            //std::cout << "[GAME " << gamenumber << "]" << std::endl;
            board_t board;

            std::stringstream gameStream(game); // to make the game string into token separated by space

            //create a file for this game
            std::ofstream outputFile(outputDir + "/game" + "[" + std::to_string(filenumber) + "]" + std::to_string(gamenumber) + ".uci");
            if (!outputFile.is_open()) {
                throw std::runtime_error("Failed to create output file for game " + std::to_string(gamenumber));
            };

            // Process the current game

            //remove the useless info
            std::string pgnContent;

            while (std::getline(gameStream, line)) {
                if (line.empty()) continue;
                if (line[0] == '[') {
                    //skip!!
                    continue;
                } 
                else 
                {
                    // Collect moves
                    pgnContent += line + " ";
                }
            };

            //clean the data
            std::string moveContent;

            std::regex clockRegex(R"(\{[^}]*\})"); // Matches { ... }
            std::regex resultRegex(R"((1-0|0-1|1/2-1/2))"); // Matches game results
            moveContent = std::regex_replace(pgnContent, clockRegex, ""); // Remove clock annotations
            moveContent = std::regex_replace(moveContent, resultRegex, ""); // Remove results

            //turn Pgn into UCI move
            std::string move;

            std::istringstream moveStream(moveContent);
            std::vector<std::string> sanMoves;
            while (moveStream >> move) {
                if(move.find('.') == std::string::npos){
                    sanMoves.push_back(move);
                };
            };

            //print out the sanMoves
            
            /*
            for(std::string san : sanMoves){
                std::cout << "[the san is]" << san << std::endl;
            };
            */
            
            
            //now in the vector sanMoves, we have all the moves but in the form of SAN
            //now turn the SAN form into UCI form

            bool if_continue = true;
            for(std::string san : sanMoves){
                try{
                    std::string uci = san_to_uci(san, board);
                } catch (const char* str) {
                    std::cout << str << std::endl;
                    std::cout << "skip the game " << gamenumber << std::endl;
                    if_continue = false;
                }
            };


            if(if_continue){
                for(std::string san : sanMoves){
                    std::string uci = san_to_uci(san, board);
                    outputFile << uci << std::endl;
                    board.make_move(move_t{uci});
                    //std::cout << "MOVEEEEE the board : " << uci << " from the san value : " << san << std::endl;
                };
            };

            outputFile.close();
            game.clear();
        }
        else 
        {
            // Accumulate game content
            game += line + "\n";
        };
    }

    // Handle the last game block if any
    if (!game.empty()) {
        gamenumber++;

        board_t board;

        std::stringstream gameStream(game);

        std::ofstream outputFile(outputDir + "/game" + "[" + std::to_string(filenumber) + "]" + std::to_string(gamenumber) + ".uci");
        if (!outputFile.is_open()) {
            throw std::runtime_error("Failed to create output file for game " + std::to_string(gamenumber));
        }

        // Process the current game

        //remove the useless info
        std::string pgnContent;

        while (std::getline(gameStream, line)) {
            if (line.empty()) continue;
            if (line[0] == '[') {
                //skip!!
                continue;
            } 
            else 
            {
                // Collect moves
                pgnContent += line + " ";
            }            
        };

        //clean the data
        std::string moveContent;

        std::regex clockRegex(R"(\{[^}]*\})"); // Matches { ... }
        std::regex resultRegex(R"((1-0|0-1|1/2-1/2))"); // Matches game results
        moveContent = std::regex_replace(pgnContent, clockRegex, ""); // Remove clock annotations
        moveContent = std::regex_replace(moveContent, resultRegex, ""); // Remove results

        //turn Pgn into UCI move
        std::string move;

        std::istringstream moveStream(moveContent);
        std::vector<std::string> sanMoves;
        
        while (moveStream >> move) {
            
            if(move.find('.') == std::string::npos){
                sanMoves.push_back(move);
            };
            
        }

        //now in the vector sanMoves, we have all the moves but in the form of SAN
        //now turn the SAN form into UCI form
        for(std::string san : sanMoves){
            outputFile << san_to_uci(san, board) << std::endl;
            board.make_move(move_t{san_to_uci(san, board)});
        };

        outputFile.close();
    };

    std::cout << "Processed " << gamenumber << " games." << std::endl;
}

void engine::convert_dir(const std::string& inputDir, const std::string& outputDir){

    int filenumber = 0;
    try {
        // Iterate through each entry in the directory
        for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
            if (std::filesystem::is_regular_file(entry) && entry.path().string().contains(".pgn")) { // Check if the entry is a regular file
                filenumber += 1;
                std::cout << "handling the file " << filenumber << entry.path().string() << std::endl;
                convert(entry.path().string(), outputDir, filenumber);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
}
