module Cell;

// getters and setters

int Cell::getRow() const {
    return row;
}

int Cell::getCol() const {
    return col;
}

void Cell::setRow(int r) {
    row = r;
}

void Cell::setCol(int c) {
    col = c;
}

char Cell::getVal() const {
    return val;
}

void Cell::setVal(char v) {
    val = v;
}

bool Cell::isOccupied() const {
    return val != ' ';
}
