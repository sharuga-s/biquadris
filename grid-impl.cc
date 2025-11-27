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
    if (!b) return false;
    return isValidPosition(*b, b->getRow(), b->getCol());
}

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

void Grid::placeBlock(Block* b) {
    // update: absolute positions must be returned to align wiht new block impl
    for (auto [r, c] : b->getAbsoluteCells()) {
        theGrid[r][c].setVal(b->getVal());
    }
}

void Grid::clearFullRows(int& numCleared) {
    numCleared = 0;

    // Just COUNT full rows; do NOT erase here
    for (int r = 0; r < rows; ++r) {
        if (isRowFull(r)) {
            ++numCleared;
        }
    }

    // Now actually clear + collapse them
    if (numCleared > 0) {
        collapseRows();
    }
}

int Grid::getRows() const {
    return rows;
}

int Grid::getCols() const {
    return cols;
}

bool Grid::isOccupied(int r, int c) const {
    return theGrid[r][c].isOccupied();
}

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
