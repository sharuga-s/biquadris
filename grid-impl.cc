module Grid;

import <vector>;
import Cell;
import Block;

Grid::Grid(int r, int c)
    : rows{r}, cols{c},
      theGrid(r, std::vector<Cell>(c))
{
    for (int i = 0; i < rows; ++i) { 
        for (int j = 0; j < cols; ++j){ 
            theGrid[i][j] = Cell(i, j, ' ');
        }
    }
}

bool Grid::isValid(Block* b) const {
    for (const Cell& c : b->getCells()) {
        int r = c.getRow();
        int col = c.getCol();

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

bool Grid::isGameOver() const {
    for (int j = 0; j < cols; ++j){
        if (theGrid[0][j].isOccupied()) {
            return true;
        }
    }
    return false;
}

void Grid::reset() {
    for (int i = 0; i < rows; ++i){ 
        for (int j = 0; j < cols; ++j){
            theGrid[i][j].setVal(' ');
        }
    }
}

const std::vector<std::vector<Cell>>& Grid::getCells() const {
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
    for (const Cell& c : b->getCells()) {
        int r = c.getRow();
        int col = c.getCol();
        theGrid[r][col].setVal(b->getVal());
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