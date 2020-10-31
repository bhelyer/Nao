#include "Point.h"
#include <iostream>

Nao::Point Nao::askForPoint() {
    std::cout << "Position (x y)> ";
    return Nao::Point{readInt(), readInt()};
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