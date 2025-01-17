#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <stdexcept>
#include <board_t.h>

namespace engine {

std::string san_to_uci(std::string& san, board_t& board);

void convert(const std::string& inputPath, const std::string& outputDir);

}


