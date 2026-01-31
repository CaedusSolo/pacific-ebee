#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

class Battlefield {
    private:
        Cell** grid; // 2D array of Cells
        int width;
        int height;
    public:
        Battlefield(int width, int height);
        ~Battlefield();
        Cell* getCell(int x, int y) const;
        int getWidth() const;
        int getHeight() const;
        void updateCell(int x, int y, Cell cell);
};
#endif