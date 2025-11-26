module Grid;

import <vector>;
import Cell;
import Block;

using namespace std;

Grid::Grid(int r, int c)
    : rows{r}, cols{c},
      theGrid(r, vector<Cell>(c))
{
    for (int i = 0; i < rows; ++i) { 
        for (int j = 0; j < cols; ++j){ 
            theGrid[i][j] = Cell(i, j, ' ');
        }
    }
}

bool Grid::isValid(Block* b) const {
    // change: get absplute posns isntead 
    for (auto [r, col] : b->getAbsoluteCells()) {
        if (r < 0 || r >= rows) {
            return false;
        }
        if (col < 0 || col >= cols) {
            return false;
        }
        if (theGrid[r][col].isOccupied()) {
            return false;
        }
    }
    return true;
}

// review: i think this method is unnecessary, we could isntead do something like...
/*
in Player, when we spawn a block: 
if (!grid.isValid(next)) {
    // game over
}
the reason why isGameOver() doesnt rlly work is because this isn't a game ending condition that shoulkd be checked by Grid, but rather by GameEngine or Player
- also, the logic is worng, row 0 is a reserve row => can be occupied/partially filled during the game
- game ends when the newly generated block cannot be placed in its spawn location
    - NOT when row 0 is full, NOT when row 3 is full, NOT when any top cells are filled  
- game over = Grid::isValid(newBlock) is false immediately after spawn
*/
// bool Grid::isGameOver() const {
//     for (int j = 0; j < cols; ++j){
//         if (theGrid[0][j].isOccupied()) {
//             return true;
//         }
//     }
//     return false;
// }

void Grid::reset() {
    for (int i = 0; i < rows; ++i){ 
        for (int j = 0; j < cols; ++j){
            theGrid[i][j].setVal(' ');
        }
    }
}

const vector<vector<Cell>>& Grid::getCells() const {
    return theGrid;
}

bool Grid::isRowFull(int r) const {
    for (int j = 0; j < cols; ++j){ 
        if (!theGrid[r][j].isOccupied()) {
            return false;
        }
    }
    return true;
}

void Grid::eraseRow(int r) {
    for (int j = 0; j < cols; ++j){
        theGrid[r][j].setVal(' ');
    }
}

void Grid::collapseRows() {
    for (int r = rows - 1; r >= 0; --r) {
        if (isRowFull(r)) {
            eraseRow(r);
            for (int i = r; i > 0; --i) {
                for (int j = 0; j < cols; ++j) {
                    char above = theGrid[i - 1][j].getVal();
                    theGrid[i][j].setVal(above);
                    theGrid[i - 1][j].setVal(' ');
                }
            }
            ++r; // need to re-check the same row # before we move on
        }
    }
}

void Grid::applyBlind(bool on) {
    blindActive = on;
}

void Grid::placeBlock(Block* b) {
    // update: absolute positions must be returned to align wiht new block impl
    for (auto [r, c] : b->getAbsoluteCells()) {
        theGrid[r][c].setVal(b->getVal());
    }
}

void Grid::clearFullRows(int& numCleared) {
    numCleared = 0;
    for (int r = 0; r < rows; ++r) {
        if (isRowFull(r)) {
            eraseRow(r);
            numCleared++;
        }
    }
    if (numCleared > 0) {
        collapseRows();
    }
}