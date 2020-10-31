#pragma once
namespace Nao {

// A position on the board.
struct Point {
    int x;  // 0 is the left.
    int y;  // 0 is the top.
};

// Read a point from the user.
[[nodiscard]] Point askForPoint();
// Get an integer from the user.
[[nodiscard]] int readInt();

}