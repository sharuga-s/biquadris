export module Block;

import Grid;

import <vector>;
import <utility>;
import <algorithm>;

using namespace std;

/*
1. cells : vector<pair<int, int>>
2. row : int
3. col: int
4. heavy : bool
5. levelGenerated : int
6. rotation : int
*/

export class Block {
    protected: 
        // fields 
        vector<pair<int, int>> cells; // stores the relative offsets of each tile in the block -> NOT the absolute grid position
        int row; // absolute pos of box's top left corner -> revisit design soon :)
        int col;
        bool heavy; // gaf for level 3 and level 4
        int levelGenerated; // gaf for the scoring 
        int rotation; // int rep for orientation -> 0,1,2,3

        // helper functions
        void applyCWRotation();
        void applyCCWRotation();
        void computeSpawnPosition();
        bool tryMove(Grid& g, int new_row, int new_col);
    public:
        // block ctor
        virtual ~Block() = default;

        // movement controls (huge assumption brochacho: BOUNDARY + COLLISION CHECK DONE IN GRID)
        void moveLeft();
        void moveRight();
        void moveDown();

        // put cells in grid (final placement)
        void place(Grid& g);

        // rotation controls
        void rotateCW(Grid& g);
        void rotateCCW(Grid& g);

        // getters/accessors/setters
        const vector<pair<int,int>>& getCells() const;
        vector<pair<int,int>> getAbsoluteCells() const; // this converts the cell position to absolute grid position 
        char getVal() const = 0; // return block type -> PVM [makes this class ABSTRACT]
        bool isBlockHeavy() const; // heavy flag
        void setHeavy(bool h); // heavy flag
        int getGenerationLevel() const; // for scoring
};
