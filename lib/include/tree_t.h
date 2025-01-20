#pragma once
#include <iostream>
#include <convert_pgn_to_uci.h>
#include <move_t.h>

namespace engine {

struct tree_t{
    struct node_t{
        std::unordered_map<move_t, std::shared_ptr<node_t>> transition_map;
        int white_wins;
        int draws;
        int white_losses;
    };

    tree_t(const std::string& inputfile);

    std::shared_ptr<node_t> root;
};


}



