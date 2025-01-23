#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <board_t.h>
#include <convert_pgn_to_uci.h>
#include <filesystem>
#include <move_convert_fn.h>

using namespace engine;

std::string engine::san_to_uci(std::string& san, board_t& board){
    std::string move_str;

	// We go through every move and see if its SAN representation matches
    for(auto& move : board.pseudolegal_moves()){
        if(!(board.is_legal(move))){
            continue;
        }

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

    throw "[Error in san_to_uci: ] invalid san value";
};

void engine::convert(const std::string& inputPath, const std::string& outputDir){
    //since we will output each game as a file, so we take the second arg as Dir(instead of Path)
    if(!std::filesystem::exists(outputDir)){
        std::filesystem::create_directory(outputDir);
    };

    std::ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        throw "[Error in convert: ] Failed to open input file.";
    };

    std::string line;
	std::string score;
    int gamenumber = 0;

    while(std::getline(inputFile, line)){
		if (line.substr(0, 8) == "[Result ")
			score = line.substr(9, 3);
		
		// Handle the case where the line represents a game
		else if (line.substr(0, 3) == "1. ") {
			try {
				// We split the game by spaces
				std::string word;
				int word_counter = 0;

				board_t board;
				std::vector<std::string> uci_moves;

				bool has_clock = line.contains("{");

				for (char c : line) {
					if (c != ' ')
						word += c;

					// We reach the end of a word
					else {
						// The case where the word is a move
						if (
							(has_clock && 
							(word_counter % 8 == 1 || word_counter % 8 == 5))
							|| (!has_clock &&
							(word_counter % 3 == 1 || word_counter % 3 == 2))
						) {
							std::string uci_move = san_to_uci(word, board);
							uci_moves.push_back(uci_move);

							board.make_move(uci_move);
						}

						// Reset word
						word = "";
						word_counter++;
					}
				}
				
				std::string filename = std::filesystem::path(inputPath).stem().string()
					+ "." + std::to_string(gamenumber);

				std::filesystem::path output_path{ outputDir };
				output_path /= filename;

				std::ofstream output_file{output_path};

				output_file << score << std::endl;
				for (auto uci : uci_moves) output_file << uci << std::endl;
				output_file.close();
			}
			catch (...) {
				std::cout << "[Convert]: Skipping game " << gamenumber
				<< " in input file " << inputPath << std::endl;
			}
			
			// Reset game
			score = "";
			gamenumber++;
		}
	}
}

void engine::convert_dir(const std::string& inputDir, const std::string& outputDir){
	// Iterate through each entry in the directory
	for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
		// Check if the entry is a regular file
		if (
			std::filesystem::is_regular_file(entry) && entry.path().string().contains(".pgn")
		) {
			convert(entry.path().string(), outputDir);
		}
	}
}
