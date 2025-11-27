module Block;

import <vector>;
import <utility>;
import <algorithm>;

using namespace std;

// helper fns -----------------------------------------------------------------
void Block::applyCWRotation() {
    int max_row = 0, max_col = 0;

    for (auto [r, c] : cells) {
        max_row = max(max_row, r);
        max_col = max(max_col, c);
    }
    int height = max_row + 1;
    int width  = max_col + 1;

    vector<pair<int,int>> new_cells;
    for (auto [r, c] : cells) {
        int new_r = c;
        int new_c = width - 1 - r;
        new_cells.push_back({new_r, new_c});
    }
    cells = new_cells;
}

void Block::applyCCWRotation() {
    int max_row = 0, max_col = 0;

    for (auto [r, c] : cells) {
        max_row = max(max_row, r);
        max_col = max(max_col, c);
    }
    int height = max_row + 1;
    int width  = max_col + 1;

    vector<pair<int,int>> new_cells;
    for (auto [r, c] : cells) {
        int new_r = height - 1 - c;
        int new_c = r;
        new_cells.push_back({new_r, new_c});
    }
    cells = new_cells;
}

void Block::computeSpawnPosition() {
    int max_row = 0;

    for (auto [r, c] : cells) {
        max_row = max(max_row, r);
    }

    int height = max_row + 1;

    // bottom row of block must be at board row 3
    row = 3 - (height - 1);
    // block appears at the leftmost column
    col = 0;
}

// rotation controllers (geometry only) ---------------------------------------
void Block::rotateCWLocal() {
    applyCWRotation();
    rotation = (rotation + 1) % 4;
}

void Block::rotateCCWLocal() {
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
    vector<pair<int,int>> abs_pos;

    for (auto [dr, dc] : cells) {
        abs_pos.push_back({row + dr, col + dc});
    }
    return abs_pos;
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
