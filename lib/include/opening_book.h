#pragma once
#include <optional>
#include <vector>
#include <string>
#include <move_t.h> // or "include/move_t.h" if needed

namespace engine {

/**
 * @brief Given the entire history of moves in UCI format, return a “book” move if available.
 *
 * @param move_history a list of moves so far in UCI notation (e.g. {"e2e4", "c7c5"}).
 * @return std::optional<engine::move_t> A move if found in the book, otherwise std::nullopt.
 */

std::optional<engine::move_t> get_opening_move(
    const std::vector<std::string>& move_history
);

} 


