#pragma once
#include "Board.h"

namespace Nao {

// The result of evaluating a board.
enum class GameResult {
    Incomplete,  // Play should continue.
    Draw,
    NoughtWins,
    CrossWins,
};

// If point is an invalid move, inform the user and return false. Otherwise returns true.
[[nodiscard]] bool validatePlayerPoint(const Board& board, Point point);
// Evaluate a single board state.
[[nodiscard]] GameResult evaluate(const Board& board);
// Check for a run of non empty PlaceValues.
[[nodiscard]] GameResult checkRun(const Board& board, Point startPoint, int xDelta, int yDelta);

}