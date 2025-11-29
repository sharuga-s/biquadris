export module GraphicsDisplay;

import Display;
import Grid;
import Block;
import xwindow;
import GameEngine;  

import <memory>;
import <vector>;
import <string>;

using namespace std;

// idea: we used caching here to prevent unnecessary redraws -> when a player moves, notify() method gets called, triggering a redraw
// to prevent this, we SKIP redraws in our render methods by checking our cache and redrawing only if necessary
// the GUI was also much smoother and cleaner with this approach! :) 

export class GraphicsDisplay : public Display {
    private:
        GameEngine* game = nullptr;
        unique_ptr<Xwindow> xw;
        bool textOnly;

        // grid dimensions
        static const int rows = 18;
        static const int cols = 11;
        static const int visibleTopRow = 0;   // show ALL rows including reserve (0-2)
        static const int visibleRows = 18;  // all 18 rows
        static const int cellSize = 25;

        // window / layout
        int windowWidth = 800;
        int windowHeight = 800;  

        int boardTop = 80;
        int board1Left = 60;
        int board2Left = 60 + cols * cellSize + 120;

        // cached board contents f
        vector<vector<char>> prevBoard1;
        vector<vector<char>> prevBoard2;

        // cached next blocks for optimization
        Block* prevNextBlock1 = nullptr;
        Block* prevNextBlock2 = nullptr;
        
        // cached held blocks for optimization
        Block* prevHeldBlock1 = nullptr;
        Block* prevHeldBlock2 = nullptr;

        // cached metadata (for skipping redraw)
        int  prevScore1  = -1, prevScore2  = -1;
        int  prevHi1     = -1, prevHi2     = -1;
        int  prevLevel1  = -1, prevLevel2  = -1;
        bool prevBlind1  = false, prevBlind2 = false;

        // setup & helpers
        void initGraphics();
        void clearBackground();

        // drawing helpers
        void drawCell(int boardIndex, int row, int col, char type, bool cellBlind);
        int getColorForBlockType(char type);
        void drawNextBlockPreview(Block* blk, int boardIndex, int nextY);
        void drawHeldBlockPreview(Block* blk, int boardIndex, int heldY);
        void renderBoards(const Grid& g1, const Grid& g2, Block* curr1, Block* curr2, bool blind1, bool blind2);
        void renderNext(Block* next1, Block* next2);
        void renderHeld(Block* held1, Block* held2);
        void renderScores(int s1, int s2, int hi1, int hi2, int lvl1, int lvl2);

    public:
        GraphicsDisplay(bool textOnly = false);
        explicit GraphicsDisplay(GameEngine* g, bool textOnly = false);

        ~GraphicsDisplay() override = default;

        void notify() override;

        void setGame(GameEngine* g);
};
