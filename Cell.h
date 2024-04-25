#ifndef CELL_H
#define CELL_H

class Cell {
public:
    char symbol;
    bool isHit;

    Cell();
    bool isCellHit() const;
    bool markAsHit();
    void setSymbol(char sym);
};

#endif
