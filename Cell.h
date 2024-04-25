#ifndef CELL_H
#define CELL_H

class Cell {
public:
    char symbol;
    bool isHit;

    Cell();
    bool hitCheck() const;
    bool markHit();
    void changeSymbol(char sym);
};

#endif
