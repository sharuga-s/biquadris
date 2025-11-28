export module GraphicsDisplay;

import Display;
import Grid;
import Block;
import xwindow;
import GameEngine;  // CRITICAL: Import GameEngine module

import <memory>;
import <vector>;
import <string>;

using namespace std;

export class GraphicsDisplay : public Display {
private:
    GameEngine* game = nullptr;
    unique_ptr<Xwindow> xw;
    bool textOnly;

    // Grid dimensions
    static constexpr int rows          = 18;
    static constexpr int cols          = 11;
    static constexpr int visibleTopRow = 0;   // Show ALL rows including reserve (0-2)
    static constexpr int visibleRows   = 18;  // All 18 rows
    static constexpr int cellSize      = 25;

    // Window / layout
    int windowWidth  = 800;
    int windowHeight = 800;  // Increased to fit held block section

    int boardTop     = 80;
    int board1Left   = 60;
    int board2Left   = 60 + cols * cellSize + 120;

    // Cached board contents for dirty checking
    vector<vector<char>> prevBoard1;
    vector<vector<char>> prevBoard2;

    // Cached next blocks for optimization
    Block* prevNextBlock1 = nullptr;
    Block* prevNextBlock2 = nullptr;
    
    // Cached held blocks for optimization
    Block* prevHeldBlock1 = nullptr;
    Block* prevHeldBlock2 = nullptr;

    // Cached metadata (for skipping redraw)
    int  prevScore1  = -1, prevScore2  = -1;
    int  prevHi1     = -1, prevHi2     = -1;
    int  prevLevel1  = -1, prevLevel2  = -1;
    bool prevBlind1  = false, prevBlind2 = false;

    // Setup & helpers
    void initGraphics();
    void clearBackground();

    // Drawing helpers
    void drawCell(int boardIndex, int row, int col, char type, bool cellBlind);
    void renderBoards(const Grid& g1, const Grid& g2, Block* curr1, Block* curr2, bool blind1, bool blind2);
    void renderNext(Block* next1, Block* next2);
    void renderHeld(Block* held1, Block* held2);
    void renderScores(int s1, int s2, int hi1, int hi2, int lvl1, int lvl2);

public:
    GraphicsDisplay(bool textOnly = false);
    explicit GraphicsDisplay(GameEngine* g, bool textOnly = false);

    ~GraphicsDisplay() override = default;

    void notify() override;

    void setGame(GameEngine* g) { game = g; }
};
