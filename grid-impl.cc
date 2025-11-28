module Grid;

import <vector>;
import Cell;
import Block;

using namespace std;

// ctor
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

// purpose: checksi f a block is currently in a valid board position
bool Grid::isValid(Block* b) const {
    if (!b) return false;
    return isValidPosition(*b, b->getRow(), b->getCol());
}

// purpose: clear entire grid to empty cells
void Grid::reset() {
    for (int i = 0; i < rows; ++i){ 
        for (int j = 0; j < cols; ++j){
            theGrid[i][j].setVal(' ');
        }
    }
}

// purpose: const reference to the 2D cell matrix
const vector<vector<Cell>>& Grid::getCells() const {
    return theGrid;
}

// purpose: true if EVERY cell in row r is occupied
bool Grid::isRowFull(int r) const {
    for (int j = 0; j < cols; ++j){ 
        if (!theGrid[r][j].isOccupied()) {
            return false;
        }
    }
    return true;
}

// purpose: clear a single row by writing ' ' into every column
void Grid::eraseRow(int r) {
    for (int j = 0; j < cols; ++j){
        theGrid[r][j].setVal(' ');
    }
}

// purpose: a row is full, erase it
//  then, shift all rows above it DOWN by one
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

// purpose: writes a block's cells directly onto the grid at its absolute coordinates
void Grid::placeBlock(Block* b) {
    // ref: absolute positions must be returned to align wiht new block impl
    for (auto [r, c] : b->getAbsoluteCells()) {
        theGrid[r][c].setVal(b->getVal());
    }
}

// purpose: counts the number of completed rows, then collapses
void Grid::clearFullRows(int& numCleared) {
    numCleared = 0;

    // just COUNT full rows; do NOT erase here
    for (int r = 0; r < rows; ++r) {
        if (isRowFull(r)) {
            ++numCleared;
        }
    }

    // now actually clear + collapse them
    if (numCleared > 0) {
        collapseRows();
    }
}

// getter
int Grid::getRows() const {
    return rows;
}
// hgetter
int Grid::getCols() const {
    return cols;
}

bool Grid::isOccupied(int r, int c) const {
    return theGrid[r][c].isOccupied();
}

// purpose: check whether placing Block b at baseRow/baseCol is valid
bool Grid::isValidPosition(const Block& b, int baseRow, int baseCol) const {
    for (auto [dr, dc] : b.getCells()) {
        int r = baseRow + dr;
        int c = baseCol + dc;

        if (r < 0 || r >= rows) return false;
        if (c < 0 || c >= cols) return false;
        if (theGrid[r][c].isOccupied()) return false;
    }
    return true;
}
