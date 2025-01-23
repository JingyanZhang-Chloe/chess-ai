
#include "opening_book.h"
#include <map>
#include <string>
#include <vector>
#include <move_t.h>  // or "include/move_t.h"

namespace {

    // A small example opening book: (move-history) -> (next move).
    // Each key is the entire sequence so far (UCI) and the value is our next move in UCI.
    static const std::map<std::vector<std::string>, std::string> OPENING_BOOK = {
        { {"c2c4"}, "c7c5" },


        { {"d2d4"}, "g8f6" },

        { {"d2d4", "d7d5"}, "e2e4" },
        { {"d2d4", "g8f6"}, "c1g5" },

        { {"d2d4", "d7d5", "c2c4"}, "b8c6" },
        { {"d2d4", "d7d5", "g1f3"}, "g8f6" },
        { {"d2d4", "g8f6", "c2c4"}, "d7d6" },
        { {"d2d4", "f7f5", "e2e4"}, "f5e4" },

        { {"d2d4", "d7d5", "g1f3", "g8f6"}, "c1f4" },
        { {"d2d4", "g8f6", "c2c4", "g7g6"}, "b1c3" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3"}, "f8g7" },
        { {"d2d4", "g8f6", "c2c4", "e7e6", "g1f3"}, "b7b6" },
        { {"d2d4", "g8f6", "c2c4", "e7e6", "b1c3"}, "f8b4" },
        { {"d2d4", "g8f6", "c2c4", "c7c5", "d4d5"}, "e7e6" },
        { {"d2d4", "d7d5", "c2c4", "e7e6", "g1f3"}, "c7c5" },

        { {"d2d4", "g8f6", "c2c4", "e7e6", "g2g3", "d7d5"}, "f1g2" },
        { {"d2d4", "g8f6", "c2c4", "e7e6", "b1c3", "f8b4"}, "d1c2" },
        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "d7d5"}, "c1g5" },

        { {"d2d4", "d7d5", "c2c4", "e7e6", "b1c3", "g8f6", "g1f3"}, "c7c5" },
        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "d7d5", "c4d5"}, "f6d5" },
        { {"d2d4", "g8f6", "c2c4", "e7e6", "b1c3", "f8b4", "e2e3"}, "c7c5" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "g1f3", "d7d6"}, "g2g3" },
        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6"}, "f2f4" },
        { {"d2d4", "f7f5", "c2c4", "g8f6", "g2g3", "g7g6", "f1g2", "f8g7"}, "g1f3" },
        { {"d2d4", "g8f6", "c2c4", "e7e6", "b1c3", "f8b4", "a2a3", "b4c3"}, "b2c3" },
        { {"d2d4", "g8f6", "c2c4", "e7e6", "b1c3", "f8b4", "e2e3", "c7c5"}, "g1e2" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "f1e2"}, "e8g8" },
        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3"}, "e8g8" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "f1e2", "e8g8"}, "c1g5" },
        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3", "e8g8"}, "f1e2" },

        { {"d2d4", "d7d5", "c2c4", "e7e6", "b1c3", "g8f6", "c1g5", "f8e7", "e2e3", "e8g8", "g1f3"}, "b8d7" },
        { {"d2d4", "f7f5", "c2c4", "g8f6", "g2g3", "e7e6", "f1g2", "f8e7", "g1f3", "e8g8", "e1g1"}, "d7d5" },
        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3", "e8g8", "f1e2"}, "e7e5" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3", "e8g8", "f1e2", "e7e5"}, "e1g1" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3", "e8g8", "f1e2", "e7e5", "e1g1"}, "b8c6" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3", "e8g8", "f1e2", "e7e5", "e1g1", "b8c6"}, "d4d5" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3", "e8g8", "f1e2", "e7e5", "e1g1", "b8c6", "d4d5"}, "c6e7" },

        { {"d2d4", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "e2e4", "d7d6", "g1f3", "e8g8", "f1e2", "e7e5", "e1g1", "b8c6", "d4d5", "c6e7"}, "b2b4" },



        { {"g1f3"}, "d7d5" },

        { {"g1f3", "d7d5"}, "c2c4" },
        



        { {"e2e4"}, "e7e5" },

        { {"e2e4", "e7e5"}, "g1f3" },
        { {"e2e4", "c7c5"}, "g1f3" },
        { {"e2e4", "e7e6"}, "d2d4" },
        { {"e2e4", "c7c6"}, "d2d4" },
        { {"e2e4", "d7d6"}, "d2d4" },

        { {"e2e4", "e7e5", "g1f3"}, "b8c6" },
        { {"e2e4", "c7c5", "g1f3"}, "d7d6" },
        { {"e2e4", "e7e6", "d2d4"}, "d7d5" },
        { {"e2e4", "c7c6", "d2d4"}, "d7d5" },
        { {"e2e4", "d7d6", "d2d4"}, "g8f6" },

        { {"e2e4", "e7e5", "g1f3", "b8c6"}, "d2d4" },
        { {"e2e4", "c7c5", "g1f3", "d7d6"}, "d2d4" },
        { {"e2e4", "e7e5", "g1f3", "g8f6"}, "f3e5" },
        { {"e2e4", "e7e6", "d2d4", "d7d5"}, "b1c3" },
        { {"e2e4", "c7c5", "g1f3", "e7e6"}, "d2d4" },
        { {"e2e4", "c7c6", "d2d4", "d7d5"}, "b1c3" },
        { {"e2e4", "c7c5", "g1f3", "b8c6"}, "d2d4" },
        { {"e2e4", "d7d6", "d2d4", "g8f6"}, "b1c3" },
        
        { {"e2e4", "e7e5", "g1f3", "b8c6", "d2d4"}, "e5d4" },
        { {"e2e4", "c7c5", "g1f3", "d7d6", "d2d4"}, "c5d4" },
        { {"e2e4", "e7e5", "g1f3", "g8f6", "f3e5"}, "d7d6" },
        { {"e2e4", "e7e6", "d2d4", "d7d5", "b1c3"}, "g8f6" },
        { {"e2e4", "c7c5", "g1f3", "e7e6", "d2d4"}, "c5d4" },
        { {"e2e4", "c7c6", "d2d4", "d7d5", "b1c3"}, "d5e4" },
        { {"e2e4", "c7c5", "g1f3", "b8c6", "d2d4"}, "c5d4" },
        { {"e2e4", "d7d6", "d2d4", "g8f6", "b1c3"}, "g7g6" },

        { {"e2e4", "e7e5", "g1f3", "b8c6", "d2d4", "e5d4"}, "f3d4" },
        { {"e2e4", "c7c5", "g1f3", "d7d6", "d2d4", "c5d4"}, "f3d4" },
        { {"e2e4", "e7e5", "g1f3", "g8f6", "f3e5", "d7d6"}, "e5f3" },
        { {"e2e4", "e7e6", "d2d4", "d7d5", "b1c3", "g8f6"}, "c1g5" },
        { {"e2e4", "c7c5", "g1f3", "e7e6", "d2d4", "c5d4"}, "f3d4" },
        { {"e2e4", "c7c6", "d2d4", "d7d5", "b1c3", "d5e4"}, "c3e4" },
        { {"e2e4", "c7c5", "g1f3", "b8c6", "d2d4", "c5d4"}, "f3d4" },
        { {"e2e4", "d7d6", "d2d4", "g8f6", "b1c3", "g7g6"}, "f2f4" },

        { {"e2e4", "e7e5", "g1f3", "b8c6", "d2d4", "e5d4", "f3d4"}, "g8f6" },
        { {"e2e4", "c7c5", "g1f3", "d7d6", "d2d4", "c5d4", "f3d4"}, "g8f6" },
        { {"e2e4", "e7e5", "g1f3", "g8f6", "f3e5", "d7d6", "e5f3"}, "f6e4" },
        { {"e2e4", "e7e6", "d2d4", "d7d5", "b1c3", "g8f6", "c1g5"}, "d5e4" },
        { {"e2e4", "c7c5", "g1f3", "e7e6", "d2d4", "c5d4", "f3d4"}, "a7a6" },
        { {"e2e4", "c7c6", "d2d4", "d7d5", "b1c3", "d5e4", "c3e4"}, "c8f5" },
        { {"e2e4", "c7c5", "g1f3", "b8c6", "d2d4", "c5d4", "f3d4"}, "g7g6" },

        { {"e2e4", "e7e5", "g1f3", "b8c6", "d2d4", "e5d4", "f3d4", "g8f6"}, "d4c6" },
        { {"e2e4", "c7c5", "g1f3", "d7d6", "d2d4", "c5d4", "f3d4", "g8f6"}, "b1c3" },
        { {"e2e4", "e7e5", "g1f3", "g8f6", "f3e5", "d7d6", "e5f3", "f6e4"}, "d2d4" },

        { {"e2e4", "e7e5", "g1f3", "b8c6", "d2d4", "e5d4", "f3d4", "g8f6", "d4c6"}, "b7c6" },
        { {"e2e4", "c7c5", "g1f3", "d7d6", "d2d4", "c5d4", "f3d4", "g8f6", "b1c3"}, "b8c6" },

        { {"e2e4", "e7e5", "g1f3", "b8c6", "d2d4", "e5d4", "f3d4", "g8f6", "d4c6", "b7c6"}, "e4e5" },
        { {"e2e4", "c7c5", "g1f3", "d7d6", "d2d4", "c5d4", "f3d4", "g8f6", "b1c3", "b8c6"}, "c1g5" },
        


    };
}

namespace engine {

std::optional<move_t> get_opening_move(
    const std::vector<std::string>& move_history
) {
    // 1) Look up this exact sequence in the book
    auto it = OPENING_BOOK.find(move_history);
    if (it != OPENING_BOOK.end()) {
        // 2) Found a matching line, so return the next move
        const std::string& uci_move = it->second;
        return move_t(uci_move); // construct a move_t from the UCI string
    }

    // 3) If we get here, no match in the opening book
    return std::nullopt;
}

} 
