#ifdef  CELL_H
#define CELL_H

#include "vector2d.h"
#include "vector"

using namespace std;

class Cell{
    private:
        Vector2D coordinates;
        vector<Ship> ships;    // ships in this cell
    public:
        Cell();
        Cell(Vector2D coordinates);
        bool isEmpty() const;
        Vector2D getCoordinates() const;
        vector<Ship> getShips() const;
        void addShip(Ship ship);
        void removeShip(Ship ship);
};

#endif
