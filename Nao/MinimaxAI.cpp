#include "MinimaxAI.h"
#include <vector>
#include "Game.h"

namespace Nao {

using Score = int;

struct Move {
    Score score;
    Point point;
};

// Score the board for the given player.
[[nodiscard]] Score score(const Board& board, PlaceValue placeValue);
// Score a given line. xDelta and yDelta are the value that is stepped from startPoint each time.
[[nodiscard]] Score scoreLine(const Board& board, PlaceValue placeValue, Point startPoint, int xDelta, int yDelta);
// Count the number of elements that are a given placeValue for a given line.
[[nodiscard]] int countPlaceValuesInLine(const Board& board, PlaceValue placeValue, Point startPoint, int xDelta, int yDelta);
// Generate a list of open points.
[[nodiscard]] std::vector<Point> getOpenPoints(const Board& board);
// Run minimax to the given depth. me => the AI PlaceValue, placeValue => the PlaceValue to consider.
[[nodiscard]] Move minimax(const Board& board, int depth, PlaceValue me, PlaceValue placeValue);


}

Nao::Board Nao::makeComputerMove(const Board& board, PlaceValue placeValue) {
    const Move move = minimax(board, 2, placeValue, placeValue);
    return board.set(move.point, placeValue);
}

Nao::Move Nao::minimax(const Board& board, int depth, PlaceValue me, PlaceValue placeValue) {
    std::vector<Point> moves = getOpenPoints(board);
    Score bestScore = me == placeValue ? std::numeric_limits<Score>::min() : std::numeric_limits<Score>::max();
    Point bestPoint{-1, -1};

    if (moves.empty() || depth == 0) {
        bestScore = score(board, me);
    } else {
        for (Point point : moves) {
            const Board proposedBoard = board.set(point, placeValue);
            if (placeValue == me) {
                const Score proposedScore = minimax(proposedBoard, depth - 1, me, flip(me)).score;
                if (proposedScore > bestScore) {
                    bestScore = proposedScore;
                    bestPoint = point;
                }
            } else {
                const Score proposedScore = minimax(proposedBoard, depth - 1, me, me).score;
                if (proposedScore < bestScore) {
                    bestScore = proposedScore;
                    bestPoint = point;
                }
            }
        }
    }

    return Move{bestScore, bestPoint};
}

Nao::Score Nao::score(const Board& board, PlaceValue placeValue) {
    Score score = 0;
    for (int i = 0; i < board.tilesHigh; ++i) {
        score += scoreLine(board, placeValue, Point{0, i}, 1, 0);
        score -= scoreLine(board, flip(placeValue), Point{0, i}, 1, 0);
        score += scoreLine(board, placeValue, Point{i, 0}, 0, 1);
        score -= scoreLine(board, flip(placeValue), Point{i, 0}, 0, 1);
    }
    score += scoreLine(board, placeValue, Point{0, 0}, 1, 1);
    score -= scoreLine(board, flip(placeValue), Point{0, 0}, 1, 1);
    score += scoreLine(board, placeValue, Point{board.tilesWide - 1, 0}, -1, 1);
    score -= scoreLine(board, flip(placeValue), Point{board.tilesWide - 1, 0}, -1, 1);

    return score;
}

Nao::Score Nao::scoreLine(const Board& board, PlaceValue placeValue, Point startPoint, int xDelta, int yDelta) {
    const int run = countPlaceValuesInLine(board, placeValue, startPoint, xDelta, yDelta);
    if (run == board.tilesWide) {
        return run * 100;
    } else if (run == board.tilesWide - 1) {
        return run * 10;
    } else {
        return run;
    }
}

int Nao::countPlaceValuesInLine(const Board& board, PlaceValue placeValue, Point startPoint, int xDelta, int yDelta) {
    Point point = startPoint;
    const Point endPoint{point.x + xDelta * board.tilesWide, point.y + yDelta * board.tilesHigh};

    int run = 0;
    while (point != endPoint) {
        const PlaceValue positionValue = board.get(point);
        if (positionValue == placeValue) {
            ++run;
        }
        point.x += xDelta;
        point.y += yDelta;
    }

    return run;
}

std::vector<Nao::Point> Nao::getOpenPoints(const Board& board) {
    std::vector<Nao::Point> results;

    const GameResult result = evaluate(board);
    if (result == GameResult::CrossWins || result == GameResult::NoughtWins) {
        // If someone has won, there are no valid moves.
        return results;
    }

    for (int y = 0; y < board.tilesHigh; ++y) {
        for (int x = 0; x < board.tilesWide; ++x) {
            const Point point{x, y};
            if (board.get(point) == PlaceValue::None) {
                results.push_back(point);
            }
        }
    }

    return results;
}