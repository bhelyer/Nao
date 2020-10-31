/* A command line game of noughts and crosses.
 * An experiment in functional-ish, immutable-ish C++.
 */
#include <iostream>
#include <stdexcept>
#include "Nao/Game.h"

namespace Nao {

/* This plays the game. It's basically, our main, leaving main
 * to catch any uncaught exceptions.
 */
void playGame();

}

int main() {
    try {
        Nao::playGame();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "An error occurred.\n";
        return 1;
    }
    return 0;
}

void Nao::playGame() {
    // We start by creating a 3x3 board, initialising it to PlaceValue::None.
    const std::vector<PlaceValue> places(3*3, PlaceValue::None);
    Board board{3, 3, places};
    auto player = PlaceValue::Nought;
    auto result = GameResult::Incomplete;


    while (result == GameResult::Incomplete) {
        board.print();

        Point point{0, 0};
        do {
            point = askForPoint();
        } while (!validatePlayerPoint(board, point));

        board = board.set(point, player);
        player = flip(player);
        result = evaluate(board);
    }

    board.print();

    if (result == GameResult::CrossWins) {
        std::cout << "X wins!\n";
    } else if (result == GameResult::NoughtWins) {
        std::cout << "O wins!\n";
    } else {
        std::cout << "It's a draw.\n";
    }
}