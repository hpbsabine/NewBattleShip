#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Cell.h"
#include <random>

const int BOARD_SIZE = 5;

class Board {
private:
    std::vector<std::vector<Cell> > grid;

public:
    Board();
    void display(bool showShips = false) const;
    void markCell(int row, int col);
    void setSymbol(int row, int col, char sym);
    bool isCellHit(int row, int col) const;
    bool isShipCell(int row, int col) const;
    void activateSpotterPlane(const Board& computerBoard);
    void placeShips(Board& board, int numShips);
    void playerPlaceShips();
    void playerMove(Board& opponentBoard);
    void computerMove(Board& playerBoard);
    bool checkGameOver(const Board& board);
    void playGame();
};

#endif
