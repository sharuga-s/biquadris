export module Grid;

import <vector>;
import Cell;
import Block;

using namespace std;

export class Grid {
    int rows;
    int cols;
    bool blindActive = false;

    vector<vector<Cell>> theGrid;

public:
    Grid(int r = 18, int c = 11);

    bool isValid(Block* b) const;
    void reset();
    const vector<vector<Cell>>& getCells() const;

    bool isRowFull(int row) const;
    void eraseRow(int row);
    void collapseRows();

    // specialAction logic
    void applyBlind(bool on);
    void placeBlock(Block* b);
    void clearFullRows(int& numCleared);
};
