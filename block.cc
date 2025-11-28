export module Block;

import <vector>;
import <utility>;
import <algorithm>;

using namespace std;

    export class Block {
    protected:
        // fields
        vector<pair<int, int>> cells;  // relative offsets
        int row; // absolute top-left row
        int col; // absolute top-left col
        bool heavy; // level 3/4 or special action
        int levelGenerated; // for scoring
        int rotation; // 0,1,2,3

        // rotation helpers
        void applyCWRotation();
        void applyCCWRotation();
        void computeSpawnPosition();

    public:
        virtual ~Block() = default;

        // --- position helpers (no Grid) ---
        int  getRow() const { return row; }
        int  getCol() const { return col; }
        void setPosition(int r, int c) { row = r; col = c; }

        // --- rotation helpers (no Grid) ---
        int  getRotation() const { return rotation; }
        void setRotation(int r) { rotation = r % 4; }

        // apply a raw rotation to the shape only
        // note: player is responsible for checking validity & reverting if needed
        void rotateCWLocal();
        void rotateCCWLocal();

        // cell access
        const vector<pair<int,int>>& getCells() const;
        void setCells(const vector<pair<int,int>>& newCells);

        // converts to absolute grid coordinates using the block's row/col
        vector<pair<int,int>> getAbsoluteCells() const;

        virtual char getVal() const = 0;   // block type

        // heavy + scoring info
        bool isBlockHeavy() const;
        void setHeavy(bool h);
        int  getGenerationLevel() const;
};
