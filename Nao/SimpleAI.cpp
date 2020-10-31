#include "SimpleAI.h"

Nao::Board Nao::makeSimpleMove(const Board& board, PlaceValue placeValue) {
    for (int y = 0; y < board.tilesHigh; ++y) {
        for (int x = 0; x < board.tilesWide; ++x) {
            const Point point{x, y};
            if (board.get(point) == PlaceValue::None) {
                return board.set(point, placeValue);
            }
        }
    }
    return board;
}