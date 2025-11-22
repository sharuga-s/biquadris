export module Cell;

export class Cell {
    int row;
    int col;
    char val;

    public:
        // ' ' represents empty
        Cell(int r = 0, int c = 0, char v = ' ')
            : row{r}, col{c}, val{v} {}

        int  getRow() const;
        int  getCol() const;
        void setRow(int r);
        void setCol(int c);

        char getVal() const;
        void setVal(char v);

        bool isOccupied() const;
    };
