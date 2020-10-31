#pragma once
#include <ostream>

namespace Nao {

// The values any place can have are as follows:
enum class PlaceValue {
    None,    // There's nothing in the given spot.
    Nought,  // There's a nought (O) in the given spot.
    Cross,   // There's a cross (X) in the given spot.
};

// Put a single char representation of each value onto the given stream.
std::ostream& operator<<(std::ostream& os, PlaceValue placeValue);

// Nought => Cross, Cross => Nought, otherwise throws std::exception.
[[nodiscard]] PlaceValue flip(PlaceValue placeValue);

}