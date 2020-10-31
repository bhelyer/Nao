#include "Board.h"
#include <iostream>

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