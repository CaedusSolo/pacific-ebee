#include "Battlefield.h"
#include "Cell.h"

Battlefield::Battlefield(int width, int height) : width(width), height(height) {
    grid = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        // iterate each row
        grid[i] = new Cell[width];
        for (int j = 0; j < width; ++j) {
            // iterate each column
            // since j is each col, so its x, and i is each row so its y
            grid[i][j] = Cell(Vector2D(j, i));
        }
    }
}

Battlefield::~Battlefield() {
    for (int i = 0; i < height; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

Cell* Battlefield::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return &grid[y][x];
    }
    return nullptr;
}

int Battlefield::getWidth() const {
    return width;
}

int Battlefield::getHeight() const {
    return height;
}

void Battlefield::updateCell(int x, int y, Cell cell) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = cell;
    }
}