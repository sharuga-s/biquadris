module Block;

import <vector>;
import <utility>;
import <algorithm>;

using namespace std;

// helper fns -----------------------------------------------------------------
void Block::applyCWRotation() { // purpose: apply clockwise rotation on the block

    int maxRow = 0, maxCol = 0;

    // range-based for loop that iterates through all cells
    for (auto [r, c] : cells) {
        maxRow = max(maxRow, r);
        maxCol = max(maxCol, c);
    }

    int width = maxCol + 1;

    // pair to store new cells
    vector<pair<int,int>> newCells; 

    for (auto [r, c] : cells) {
        int newR = c;
        int newC = width - 1 - r;
        newCells.push_back({newR, newC});
    }

    // reassign
    cells = newCells;
}

void Block::applyCCWRotation() { // purpose: apply c-clockwise rotation on the block
    int maxRow = 0, maxCol = 0;

    // range-based for loop that iterates through all cells
    for (auto [r, c] : cells) {
        maxRow = max(maxRow, r);
        maxCol = max(maxCol, c);
    }
    
    int height = maxRow + 1;

    // pair to store new cells
    vector<pair<int,int>> newCells;
    for (auto [r, c] : cells) {
        int newR = height - 1 - c;
        int newC = r;
        newCells.push_back({newR, newC});
    }

    // reassign
    cells = newCells;
}

void Block::computeSpawnPosition() { // purpose: find spawn position of the block
    int maxRow = 0;

    for (auto [r, c] : cells) {
        maxRow = max(maxRow, r);
    }

    int height = maxRow + 1;

    // bottom row of block must be at board row 3 (reserve row condition)
    row = 3 - (height - 1);
    // block appears at the leftmost column
    col = 0;
}

// rotation controllers (geometry only) ---------------------------------------
void Block::rotateCWLocal() { // purpose: rotation states
    applyCWRotation();
    rotation = (rotation + 1) % 4;
}

void Block::rotateCCWLocal() {  // purpose: rotation states
    applyCCWRotation();
    rotation = (rotation + 3) % 4;
}

// getters/accessors/setters ---------------------------------------------------
const vector<pair<int,int>>& Block::getCells() const {
    return cells;
}

void Block::setCells(const vector<pair<int,int>>& newCells) {
    cells = newCells;
}

vector<pair<int,int>> Block::getAbsoluteCells() const {
    vector<pair<int,int>> absPos;

    for (auto [dr, dc] : cells) {
        absPos.push_back({row + dr, col + dc});
    }
    return absPos;
}

bool Block::isBlockHeavy() const {
    return heavy;
}

void Block::setHeavy(bool h) {
    heavy = h;
}

int Block::getGenerationLevel() const {
    return levelGenerated;
}
