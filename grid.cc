export module Grid;

import <vector>;
import Cell;
import Block;

using namespace std;

export class Grid {
    int rows;
    int cols;
    vector<vector<Cell>> theGrid;

public:
    Grid(int r = 18, int c = 11); // initialize with default values of grid size

    bool isValid(Block* b) const;
    void reset();
    const vector<vector<Cell>>& getCells() const;

    bool isRowFull(int row) const;
    void eraseRow(int row);
    void collapseRows();

    void placeBlock(Block* b);
    void clearFullRows(int& numCleared);

    // getters 
    int getRows() const;
    int getCols() const;
    bool isOccupied(int r, int c) const;
    bool isValidPosition(const Block& b, int baseRow, int baseCol) const;
};
