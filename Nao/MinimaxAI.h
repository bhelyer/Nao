#pragma once
#include "Board.h"

namespace Nao {

// Make a move on the given board for the given player.
[[nodiscard]] Board makeComputerMove(const Board& board, PlaceValue placeValue);

}