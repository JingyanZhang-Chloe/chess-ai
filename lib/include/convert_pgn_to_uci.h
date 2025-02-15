#pragma once
#include <string>
#include <board_t.h>

namespace engine {

std::string san_to_uci(std::string& san, board_t& board);

void convert(const std::string& inputPath, const std::string& outputDir);

void convert_dir(const std::string& inputDir, const std::string& outputDir);
}


