#include "PlaceValue.h"

std::ostream& Nao::operator<<(std::ostream& os, PlaceValue placeValue) {
    switch (placeValue) {
    case PlaceValue::None: return os << '_';
    case PlaceValue::Nought: return os << 'O';
    case PlaceValue::Cross: return os << 'X';
    default: throw std::exception{"operator<<: Unknown PlaceValue."};
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