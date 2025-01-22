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

        void print();
    };

    tree_t(const std::string& inputfile);

    void single_print();

    tree_t merge(tree_t& tree, std::shared_ptr<node_t> current_node_for_this);

    void print();

    void print_children(std::shared_ptr<node_t> node);

    void print_moves(std::shared_ptr<node_t> node);

    std::shared_ptr<node_t> root;
};


}



