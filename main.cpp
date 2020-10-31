/* A command line game of noughts and crosses.
 * An experiment in functional-ish, immutable-ish C++.
 */
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include "Nao/Game.h"
#include "Nao/MinimaxAI.h"

namespace Nao {

/* This plays the game. It's basically, our main, leaving main
 * to catch any uncaught exceptions.
 */
void playGame();

}

int main() {
    std::srand(std::time(nullptr)); // NOLINT(cert-msc51-cpp)
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
    auto currentPlayer = PlaceValue::Nought;
    auto result = GameResult::Incomplete;

    const PlaceValue humanPlayer = (std::rand() % 2 == 0) ? PlaceValue::Nought : PlaceValue::Cross; // NOLINT(cert-msc50-cpp)
    if (humanPlayer == PlaceValue::Nought) {
        std::cout << "You're going first.\n";
    } else {
        std::cout << "You're going second.\n";
    }

    while (result == GameResult::Incomplete) {
        board.print();

        if (currentPlayer == humanPlayer) {
            Point point{0, 0};
            do {
                point = askForPoint();
            } while (!validatePlayerPoint(board, point));
            board = board.set(point, currentPlayer);
        } else {
            board = makeComputerMove(board, currentPlayer);
        }

        currentPlayer = flip(currentPlayer);
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