#include "cell.h"

Cell::Cell() : coordinates(Vector2D(0,0)) {}
Cell::Cell(Vector2D coordinates) : coordinates(coordinates) {}

Vector2D Cell::getCoordinates() const {
    return coordinates;
}

vector<Ship> Cell::getShips() const {
    return ships;
}

void Cell::addShip(Ship ship) {
    ships.push_back(ship);
}

void Cell::removeShip(Ship ship) {
    ships.erase(std::remove(ships.begin(), ships.end(), ship), ships.end());
}

bool Cell::isEmpty() const {
    return ships.empty();
}