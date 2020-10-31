#pragma once
#include "Board.h"

namespace Nao {

// Make a move on board for placeValue. If it can't, board is returned.
[[nodiscard]] Board makeSimpleMove(const Board& board, PlaceValue placeValue);

}