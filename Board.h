#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Cell.h"
#include <random>

const int Size = 5;

class Board {
private:
    std::vector<std::vector<Cell> > grid;

public:
    Board();
    void display(bool showShips = false) const;
    void setCell(int row, int col);
    void changeSymbol(int row, int col, char sym);
    bool hitCheck(int row, int col) const;
    bool containsShip(int row, int col) const;
    void activateSpotterPlane(const Board& computerBoard);
    void placeShips(Board& board, int numShips);
    void playerPlaceShips();
    void playerMove(Board& opponentBoard);
    void computerMove(Board& playerBoard);
    bool checkGameOver(const Board& board);
    void playGame();
};

#endif
