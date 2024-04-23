#include "Board.h"

Cell::Cell() : symbol('~'), isHit(false) {}

bool Cell::isCellHit() const {
    return isHit;
}

void Cell::markAsHit() {
    isHit = true;
}

void Cell::setSymbol(char sym) {
    symbol = sym;
}

Board::Board() {
    grid.resize(BOARD_SIZE, std::vector<Cell>(BOARD_SIZE));
}

void Board::playerPlaceShips() {
    std::cout << "Place your ships.\n";
    for (int ship = 0; ship < 3; ++ship) { // Assuming the player has 3 ships
        int row, col;
        std::cout << "Enter coordinates for Ship " << ship + 1 << " (row column): ";
        std::cin >> row >> col;

        // Ensure the entered coordinates are within the board's bounds
        if (row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE) {
            std::cout << "Invalid coordinates. Try again.\n";
            --ship; // Go back to the previous ship iteration
            continue;
        }

        // Convert input coordinates to zero-based indexing
        row--;
        col--;

        // Check if the cell is already occupied by a ship
        if (grid[row][col].symbol == 'S') {
            std::cout << "This cell is already occupied by a ship. Try again.\n";
            --ship; // Go back to the previous ship iteration
            continue;
        }

        // Place the ship symbol on the board
        setSymbol(row, col, 'S');
    }
}

void Board::display(bool showShips) const {
    std::cout << "  ";
    for (int i = 0; i < BOARD_SIZE; ++i)
        std::cout << i + 1 << " ";
    std::cout << std::endl;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j].isCellHit()) {
                if (grid[i][j].symbol == 'S') {
                    std::cout << "H ";
                } else {
                    std::cout << "X ";
                }
            } else if (showShips && grid[i][j].symbol == 'S') {
                std::cout << "S ";
            } else {
                std::cout << "~ ";
            }
        }
        std::cout << std::endl;
    }
}

void Board::markCell(int row, int col) {
    grid[row][col].markAsHit();
}

void Board::setSymbol(int row, int col, char sym) {
    grid[row][col].setSymbol(sym);
}

bool Board::isCellHit(int row, int col) const {
    return grid[row][col].isCellHit();
}

bool Board::isShipCell(int row, int col) const {
    return grid[row][col].symbol == 'S';
}

void Board::activateSpotterPlane(const Board& computerBoard) {
    // Find a cell containing an unhit ship on the computer's board
    std::vector<std::pair<int, int> > unhitShipCells;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (computerBoard.grid[i][j].symbol == 'S' && !computerBoard.grid[i][j].isCellHit()) {
                unhitShipCells.push_back(std::make_pair(i, j));
            }
        }
    }

    // If there are unhit ship cells, reveal the position of one of them
    if (!unhitShipCells.empty()) {
        int randomIndex = rand() % unhitShipCells.size();
        int row = unhitShipCells[randomIndex].first;
        int col = unhitShipCells[randomIndex].second;
        std::cout << "Spotter plane reveals a ship at position: " << row + 1 << ", " << col + 1 << std::endl;
    } else {
        std::cout << "Spotter plane couldn't find any unhit ships." << std::endl;
    }
}


void Board::placeShips(Board& board, int numShips) {
    for (int ship = 0; ship < numShips; ++ship) {
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;

        // Ensure the cell is empty before placing a ship
        while (board.grid[row][col].symbol == 'S') {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        }

        // Place the ship symbol on the board
        board.setSymbol(row, col, 'S');
    }
}


void Board::playerMove(Board& opponentBoard) {
    int row, col;

    std::cout << "Enter target coordinates (row column): ";
    std::cin >> row >> col;

    // Ensure the entered coordinates are within the board's bounds
    if (row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE) {
        std::cout << "Invalid coordinates. Try again.\n";
        playerMove(opponentBoard); // Recursively call playerMove to retry input
        return;
    }

    // Convert input coordinates to zero-based indexing
    row--;
    col--;

    // Check if the cell has already been hit
    if (opponentBoard.isCellHit(row, col)) {
        std::cout << "You've already fired at this cell. Try again.\n";
        playerMove(opponentBoard); // Recursively call playerMove to retry input
        return;
    }

    // Mark the cell as hit on the opponent's board
    opponentBoard.markCell(row, col);

    // Check if the hit cell contains a ship
    if (opponentBoard.isShipCell(row, col)) {
        std::cout << "Hit! You've hit an enemy ship!\n";
    } else {
        std::cout << "Miss! You missed the enemy's ships.\n";
    }
}

void Board::computerMove(Board& playerBoard) {
    int row, col;

    // Generate random coordinates for the computer's move
    row = rand() % BOARD_SIZE;
    col = rand() % BOARD_SIZE;

    // Check if the selected cell has already been hit
    while (playerBoard.isCellHit(row, col)) {
        row = rand() % BOARD_SIZE;
        col = rand() % BOARD_SIZE;
    }

    // Mark the cell as hit on the player's board
    playerBoard.markCell(row, col);

    // Check if the hit cell contains a ship
    if (playerBoard.isShipCell(row, col)) {
        std::cout << "The computer hit your ship at position " << row + 1 << ", " << col + 1 << "!\n";
    } else {
        std::cout << "The computer missed at position " << row + 1 << ", " << col + 1 << ".\n";
    }
}

void Board::playGame() {
    int turnCount = 0;
    const int maxTurns = 25; // Adjust the maximum number of turns as needed

    // Place ships on the player and computer boards
    playerPlaceShips();
    Board computerBoard;
    placeShips(computerBoard, 3);

    // Game loop
    while (true) {
        turnCount++;

        // Player's turn
        std::cout << "===== Turn " << turnCount << " =====\n";
        std::cout << "Your board:\n";
        display(true); // Show player's ships
        std::cout << "Computer's board:\n";
        computerBoard.display();

        // Check if it's a multiple of 5 to activate the spotter plane power-up
        if (turnCount % 5 == 0) {
            std::cout << "You received a spotter plane power-up!\n";
            activateSpotterPlane(computerBoard);
            // Deduct power-up usage or apply cooldown
        }

        // Player's move
        std::cout << "Player's turn:\n";
        playerMove(computerBoard); // Player's move
        if (checkGameOver(computerBoard)) { // Check if all computer's ships are sunk
            std::cout << "Congratulations! You've sunk all the computer's ships in " << turnCount << " turns!\n";
            break;
        }

        // Computer's turn
        std::cout << "Computer's turn:\n";
        computerMove(*this); // Computer's move
        if (checkGameOver(*this)) { // Check if all player's ships are sunk
            std::cout << "Oh no! The computer has sunk all your ships.\n";
            break;
        }

        // Check for maximum turns reached
        if (turnCount >= maxTurns) {
            std::cout << "Maximum turns reached. Game over.\n";
            break;
        }
    }
}

bool Board::checkGameOver(const Board& board) {
    // Iterate over the board and check if all ships have been hit
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board.grid[row][col].symbol == 'S' && !board.grid[row][col].isCellHit()) {
                // If there's a ship cell that hasn't been hit, the game is not over
                return false;
            }
        }
    }
    // If all ship cells have been hit, the game is over
    return true;
}
