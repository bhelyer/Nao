#pragma once
#include <vector>
#include "PlaceValue.h"
#include "Point.h"

namespace Nao {

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

}