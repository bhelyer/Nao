#include "Game.h"
#include <cassert>
#include <iostream>

bool Nao::validatePlayerPoint(const Board& board, Point point) {
    if (!board.isInBounds(point)) {
        std::cout << "Given play is out of bounds.\n";
        return false;
    }
    if (board.get(point) != PlaceValue::None) {
        std::cout << "Given play is not onto an empty space.\n";
        return false;
    }
    return true;
}

Nao::GameResult Nao::evaluate(const Board& board) {
    assert(board.tilesWide == board.tilesHigh);

    // Check for horizontal and vertical runs.
    for (int v = 0; v < board.tilesHigh; ++v) {
        const Point startHorizontalPoint{0, v};
        const GameResult horizontalResult = checkRun(board, startHorizontalPoint, 1, 0);
        if (horizontalResult != GameResult::Incomplete) {
            return horizontalResult;
        }

        const Point startVerticalPoint{v, 0};
        const GameResult verticalResult = checkRun(board, startVerticalPoint, 0, 1);
        if (verticalResult != GameResult::Incomplete) {
            return verticalResult;
        }
    }

    // Check for the two diagonals.
    const Point backslashPoint{0, 0};
    const GameResult backslashResult = checkRun(board, backslashPoint, 1, 1);
    if (backslashResult != GameResult::Incomplete) {
        return backslashResult;
    }
    const Point forwardslashPoint{board.tilesWide - 1, 0};
    const GameResult forwardslashResult = checkRun(board, forwardslashPoint, -1, 1);
    if (forwardslashResult != GameResult::Incomplete) {
        return forwardslashResult;
    }

    // Otherwise, check for a draw.
    if (std::count(std::begin(board.places), std::end(board.places), PlaceValue::None) > 0) {
        return GameResult::Incomplete;
    } else {
        return GameResult::Draw;
    }
}

Nao::GameResult Nao::checkRun(const Board& board, Point startPoint, int xDelta, int yDelta) {
    const PlaceValue place = board.get(startPoint);
    if (place == PlaceValue::None) {
        return Nao::GameResult::Incomplete;
    }

    const Point endPoint{startPoint.x + xDelta * (board.tilesWide - 1), startPoint.y + yDelta * (board.tilesHigh - 1)};
    Point point = startPoint;
    while (point.x != endPoint.x || point.y != endPoint.y) {
        point = Point{point.x + xDelta, point.y + yDelta};
        if (board.get(point) != place) {
            return Nao::GameResult::Incomplete;
        }
    }

    if (place == PlaceValue::Nought) {
        return Nao::GameResult::NoughtWins;
    } else {
        return Nao::GameResult::CrossWins;
    }
}