module Block;

using namespace std;

// helper fns -----------------------------------------------------------------
void Block::applyCWRotation() {
    int  max_row = 0,  max_col = 0;
    // range based fl, purpose, find max of row and cell
    for (auto [r, c] : cells) {
         max_row = max( max_row, r);
         max_col = max( max_col, c);
    }
    int height =  max_row + 1;
    int width =  max_col + 1;

    vector<pair<int,int>> new_cells;
    // range based fl -> purpose, find new cells formation after applying CW rotation
    for (auto [r, c] : cells) {
        int new_r = c;
        int new_c = width - 1 - r;
        new_cells.push_back({new_r, new_c});
    }
    cells = new_cells;
}

void Block::applyCCWRotation() {
    int  max_row = 0,  max_col = 0;
    // range based fl -> purpose, find max of row and cwll
    for (auto [r, c] : cells) {
         max_row = max( max_row, r);
         max_col = max( max_col, c);
    }
    int height =  max_row + 1;
    int width =  max_col + 1;

    vector<pair<int,int>> new_cells;
    // range based fl -> purpose, find new cells formation after applying CCW rotation
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


bool Block::tryMove(Grid& g, int new_row, int new_col) {
    int old_row = row;
    int old_col = col;

    row = new_row;
    col = new_col;

    bool can_move = g.isValid(this);

    if (!can_move) {
        row = old_row;
        col = old_col;
    }
    return can_move;
}

// movement controllers --------------------------------------------------------
void Block::moveLeft(Grid& g) {
    if (tryMove(g, row, col - 1) && heavy) {
        // heavy behaviour: extra drop after horizontal move
        tryMove(g, row + 1, col);
    }
}

void Block::moveRight(Grid& g) {
    if (tryMove(g, row, col + 1) && heavy) {
        tryMove(g, row + 1, col);
    }
}

void Block::moveDown(Grid& g) {
    // down is just a normal soft drop
    tryMove(g, row + 1, col);
}

bool Block::softDrop(Grid& g) {
    return tryMove(g, row + 1, col);
}

// placement controllers --------------------------------------------------------
void Block::place(Grid& g) {
    g.placeBlock(this);
}

// rotation controllers --------------------------------------------------------
void Block::rotateCW(Grid& g) {
    auto old = cells;

    applyCWRotation(); // protected CW helper

    if (!g.isValid(this)) {
        cells = old; // revert
    } else {
        rotation = (rotation + 1) % 4;
        if (heavy) {
            // heavy blocks also fall 1 row after rotation
            tryMove(g, row + 1, col);
        }
    }
}

void Block::rotateCCW(Grid& g) {
    auto old = cells;

    applyCCWRotation(); // protected CCW helper

    if (!g.isValid(this)) {
        cells = old; // revert
    } else {
        rotation = (rotation + 3) % 4;
        if (heavy) {
            tryMove(g, row + 1, col);
        }
    }
}

// getters/accessors/setters --------------------------------------------------------
const vector<pair<int,int>>& Block::getCells() const {
    return cells;
}

vector<pair<int,int>> Block::getAbsoluteCells() const {
    vector<pair<int,int>> abs_pos; // new vector that will store absolute positions

    // loop through every cell in the block’s relative coordinates -> store delta row and delta col
    for (auto [dr, dc] : cells) {
        // convert relative coordinates -> absolute coordinates
        abs_pos.push_back({row + dr, col + dc});
    }
    return abs_pos; // vector of all absolute grid positions
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
