#include "Board.h"

// Constructor for the Cell class, initializes the cell with '~' symbol and isHit to false
Cell::Cell() : symbol('~'), isHit(false) {}

// Member function of Cell Class which returns whether the cell has been hit or not (t/f)
bool Cell::hitCheck() const {
    return isHit;
}

// Member function of Cell Class which marks the cell as hit
bool Cell::markHit() {
    isHit = true;
}

// Member function of Cell Class which sets the symbol of the cell
void Cell::changeSymbol(char sym) {
    symbol = sym;
}

// Member function of the board class to mark a cell as hit
void Board::setCell(int row, int col) {
    grid[row][col].markHit();
}

// Member function of the board class to set the symbol of a cell
void Board::changeSymbol(int row, int col, char sym) {
    grid[row][col].changeSymbol(sym);
}

// Member function of the board class to check if a cell has been hit
bool Board::hitCheck(int row, int col) const {
    return grid[row][col].hitCheck();
}

// Member function of the board class to check if a cell contains a ship
bool Board::containsShip(int row, int col) const {
    return grid[row][col].symbol == 'S';
}

//constructor to create the board
Board::Board() {
    grid.resize(Size, std::vector<Cell>(Size));
}


// Function to place ships on the board by intializing the board into a 2D Vector of Cells
void Board::playerPlaceShips() {
    std::cout << "Place your ships.\n";
    for (int ship = 0; ship < 3; ++ship) { // Assuming the player has 3 ships
        int row, col;
        std::cout << "Enter coordinates for Ship " << ship + 1 << " (row column): ";
        std::cin >> row >> col;

        // Ensure the entered coordinates are within the board's bounds
        if (row < 1 || row > Size || col < 1 || col > Size) {
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
        changeSymbol(row, col, 'S');
    }
}

// Function to place ships on the board by intializing the board into a 2D Vector of Cells
void Board::placeShips(Board& board, int numShips) {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, Size - 1);

    for (int ship = 0; ship < numShips; ++ship) {
        int row = dis(gen);
        int col = dis(gen);

        // Ensure the cell is empty before placing a ship
        while (board.grid[row][col].symbol == 'S') {
            row = dis(gen);
            col = dis(gen);
        }

        // Place the ship symbol on the board
        board.changeSymbol(row, col, 'S');
    }
}

// Function to display the current state of the board using a boolean to determine whether to display or not
void Board::display(bool showShips) const {
    std::cout << "  ";
    for (int i = 0; i < Size; ++i)
        std::cout << i + 1 << " ";
    std::cout << std::endl;

    for (int i = 0; i < Size; ++i) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < Size; ++j) {
            if (grid[i][j].hitCheck()) {
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

// Member function of the board class to activate the spotter plane power-up
void Board::activateSpotterPlane(const Board& computerBoard) {
    // Find a cell containing an unhit ship on the computer's board
    std::vector<std::pair<int, int> > unhitShipCells;
    for (int i = 0; i < Size; ++i) {
        for (int j = 0; j < Size; ++j) {
            if (computerBoard.grid[i][j].symbol == 'S' && !computerBoard.grid[i][j].hitCheck()) {
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

// Member function of the board class to handle the player's move
void Board::playerMove(Board& opponentBoard) {
    int row, col;

    std::cout << "Enter target coordinates (row column): ";
    std::cin >> row >> col;

    // Ensure the entered coordinates are within the board's bounds
    if (row < 1 || row > Size || col < 1 || col > Size) {
        std::cout << "Invalid coordinates. Try again.\n";
        playerMove(opponentBoard); // Recursively call playerMove to retry input
        return;
    }

    // Convert input coordinates to zero-based indexing
    row--;
    col--;

    // Check if the cell has already been hit
    if (opponentBoard.hitCheck(row, col)) {
        std::cout << "You've already fired at this cell. Try again.\n";
        playerMove(opponentBoard); // Recursively call playerMove to retry input
        return;
    }

    // Mark the cell as hit on the opponent's board
    opponentBoard.setCell(row, col);

    // Check if the hit cell contains a ship
    if (opponentBoard.containsShip(row, col)) {
        std::cout << "Hit! You've hit an enemy ship!\n";
    } else {
        std::cout << "Miss! You missed the enemy's ships.\n";
    }
}

// Member function of the board class to handle the computer's move
#include <random>

void Board::computerMove(Board& playerBoard) {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, Size - 1);

    int row = dis(gen);
    int col = dis(gen);

    // Check if the selected cell has already been hit
    while (playerBoard.hitCheck(row, col)) {
        row = dis(gen);
        col = dis(gen);
    }

    // Mark the cell as hit on the player's board
    playerBoard.setCell(row, col);

    // Check if the hit cell contains a ship
    if (playerBoard.containsShip(row, col)) {
        std::cout << "The computer hit your ship at position " << row + 1 << ", " << col + 1 << "!\n";
    } else {
        std::cout << "The computer missed at position " << row + 1 << ", " << col + 1 << ".\n";
    }
}

// Member function of the board class to play the game, tying together all the previously declared functions
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

// Member function of the board class to check if the game is over
bool Board::checkGameOver(const Board& board) {
    // Iterate over the board and check if all ships have been hit
    for (int row = 0; row < Size; ++row) {
        for (int col = 0; col < Size; ++col) {
            if (board.grid[row][col].symbol == 'S' && !board.grid[row][col].hitCheck()) {
                // If there's a ship cell that hasn't been hit, the game is not over
                return false;
            }
        }
    }
    // If all ship cells have been hit, the game is over
    return true;
}
