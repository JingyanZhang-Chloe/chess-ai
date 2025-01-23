#include <tree_t.h>
#include <filesystem>
#include <fstream>

using namespace engine;

int main() {
	try {
		convert_dir("input_games", "output_games");
		std::optional<tree_t> acc_tree;
		
		for (auto entry : std::filesystem::directory_iterator("output_games")) {
			if (acc_tree.has_value()) {
				tree_t other { entry.path() };

				acc_tree.value().merge(other, acc_tree->root);
			}
			else {
				acc_tree = tree_t { entry.path() };
			}
		}

		std::ofstream opening_file{"opening.cpp"};

		opening_file << "include <move_t.h>\n#include <string>\n#include <unordered_mao>\n\n";

		opening_file << "const " << acc_tree.value().to_cpp();
		opening_file.close();
	}
	catch(const char* e) {
		std::cout << e;
	}
};
