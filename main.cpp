/* A command line game of noughts and crosses.
 * An experiment in functional-ish, immutable-ish C++.
 */

// We'll represent the board as a vector of values.
#include <vector>
// Exceptional circumstances will be indicated with std::exception.
#include <stdexcept>
// For both input and output.
#include <iostream>
// We use std::count when evaluating the board.
#include <algorithm>
// We'll check some assumptions with assert.
#include <cassert>

// We'll start with the forward declarations.
namespace Nao {

// The values any place can have are as follows:
enum class PlaceValue {
    None,    // There's nothing in the given spot.
    Nought,  // There's a nought (O) in the given spot.
    Cross,   // There's a cross (X) in the given spot.
};

// Put a single char representation of each value onto the given stream.
std::ostream& operator<<(std::ostream& os, PlaceValue placeValue);

// A position on the board.
struct Point {
    int x;  // 0 is the left.
    int y;  // 0 is the top.
};

/* We'll wrap the PlaceValue vector in a small struct.
 * While won't be mutating a board, we won't be enforcing
 * it with accessors -- most instances will just be const, instead.
 */
struct Board {
    // First up, the data members.
public:
    /* The places array contains all of the values for this board.
     * For a board
     *   [00] [10] [20]
     *   [01] [11] [21]
     *   [02] [12] [22]
     * We'll store it in a single dimension:
     *   [00 10 20 01 11 21 02 12 22]
     */
    std::vector<PlaceValue> places;
    int tilesWide;  // How many tiles are in each row.
    int tilesHigh;  // How many tiles are in each column.

    // The constructor is next.
public:
    /* Construct a Board of the given dimensions, using the given places vector.
     * If the size of the vector does not equal the product of the dimensions,
     * if the dimensions are not equal, or if the dimensions are <= to 0,
     * an std::exception shall be thrown.
     */
    Board(int tilesWide_, int tilesHigh_, std::vector<PlaceValue> places_);

    /* Public member functions.
     * These have to be const, but making them free or static seems silly.
     */
public:

    /* Print the board to stdout with a blank line after it.
     * Obviously output makes this not a pure function, but I'm
     * not running a monastery here.
     */
    void print() const;

    /* Returns a new board that differs from the input board in that [x, y] is set to placeValue.
     * See the places field comment above for how to interpret a given (x, y) coordinate.
     * If x or y is out of bounds, a std::exception will be thrown.
     */
    [[nodiscard]] Board set(Point point, PlaceValue placeValue) const;

    /* Get the value at the given point.
     * If the point is out of bounds, an exception is thrown.
     */
    [[nodiscard]] PlaceValue get(Point point) const;

    // Returns true if the given point is in bounds.
    [[nodiscard]] bool isInBounds(Point point) const;
};

/* This plays the game. It's basically, our main, leaving main
 * to catch any uncaught exceptions.
 */
void playGame();

// Read a point from the user.
[[nodiscard]] Point askForPoint();
// Get an integer from the user.
[[nodiscard]] int readInt();
// If point is an invalid move, inform the user and return false. Otherwise returns true.
[[nodiscard]] bool validatePlayerPoint(const Board& board, Point point);

// Nought => Cross, Cross => Nought, otherwise throws std::exception.
[[nodiscard]] PlaceValue flip(PlaceValue placeValue);

// The result of evaluating a board.
enum class GameResult {
    Incomplete,  // Play should continue.
    Draw,
    NoughtWins,
    CrossWins,
};

[[nodiscard]] GameResult evaluate(const Board& board);
[[nodiscard]] GameResult checkRun(const Board& board, Point startPoint, int xDelta, int yDelta);

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

// ...and now the implementations of the prior declarations.

std::ostream& Nao::operator<<(std::ostream& os, PlaceValue placeValue) {
    switch (placeValue) {
    case PlaceValue::None: return os << '_';
    case PlaceValue::Nought: return os << 'O';
    case PlaceValue::Cross: return os << 'X';
    default: throw std::exception{"operator<<: Unknown PlaceValue."};
    }
}

Nao::Board::Board(int tilesWide_, int tilesHigh_, std::vector<PlaceValue> places_) :
    tilesWide{tilesWide_}, tilesHigh{tilesHigh_}, places{std::move(places_)} {
    if (tilesWide_ != tilesHigh_) {
        throw std::exception{"Board::Board: non square board specified."};
    }
    if (tilesWide_ <= 0) {
        throw std::exception{"Board::Board: zero or negative sized board specified."};
    }
    if (places.size() != tilesWide * tilesHigh) {
        throw std::exception{"Board::Board: invalid size places vector."};
    }
}

Nao::Board Nao::Board::set(Point point, PlaceValue placeValue) const {
    std::vector<PlaceValue> newPlaces = places;
    if (!isInBounds(point)) {
        throw std::exception{"Board::set: out of bounds access."};
    }
    const size_t i = point.y * tilesWide + point.x;
    newPlaces[i] = placeValue;
    return Board{tilesWide, tilesHigh, newPlaces};
}

Nao::PlaceValue Nao::Board::get(Point point) const {
    if (!isInBounds(point)) {
        throw std::exception{"Board::get: out of bounds access."};
    }
    const size_t i = point.y * tilesWide + point.x;
    return places[i];
}

bool Nao::Board::isInBounds(Point point) const {
    return point.x >= 0 && point.x < tilesWide && point.y >= 0 && point.y < tilesHigh;
}

void Nao::Board::print() const {
    for (int y = 0; y < tilesHigh; ++y) {
        for (int x = 0; x < tilesWide; ++x) {
            std::cout << places[y * tilesWide + x] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';  // The blank line we promised.
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

Nao::Point Nao::askForPoint() {
    std::cout << "Position (x y)> ";
    return Nao::Point{readInt(), readInt()};
}

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

int Nao::readInt() {
    int i;
    while (true) {
        if (std::cin >> i) {
            return i;
        }
        std::cout << "Please enter a number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

Nao::PlaceValue Nao::flip(PlaceValue placeValue) {
    if (placeValue == PlaceValue::Nought) {
        return PlaceValue::Cross;
    } else if (placeValue == PlaceValue::Cross) {
        return PlaceValue::Nought;
    } else {
        throw std::exception{"flip: passed a bad placeValue."};
    }
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