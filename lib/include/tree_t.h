#pragma once
#include <convert_pgn_to_uci.h>
#include <board_t.h>
#include <move_t.h>
#include <memory>
#include <functional>

namespace engine {

struct tree_t{
    struct node_t{
        std::unordered_map<move_t, std::shared_ptr<node_t>> transition_map;
        int white_wins;
        int draws;
        int white_losses;

        void print();

		int score();
    };

    tree_t(const std::string& inputfile);

    tree_t(std::shared_ptr<node_t> root);

    void single_print();

    void merge(tree_t& tree, std::shared_ptr<node_t> current_node_for_this);

    void print();

    void print_children(std::shared_ptr<node_t> node);

    void print_moves(std::shared_ptr<node_t> node);

    std::shared_ptr<node_t> root;

	std::string to_cpp();

	void traverse(std::function<void(node_t&, board_t&)>);
	void traverse(std::function<void(node_t&, board_t&)>, board_t&);
};


}



