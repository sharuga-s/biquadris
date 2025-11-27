export module GraphicsDisplay;

import Grid;
import Block;
import xwindow;

import <memory>;

using namespace std;

export class GraphicsDisplay {
private:
    // Stored data for rendering
    const Grid* grid1 = nullptr;
    const Grid* grid2 = nullptr;
    Block* nextBlock1 = nullptr;
    Block* nextBlock2 = nullptr;
    int level1 = 0;
    int level2 = 0;
    int score1 = 0;
    int score2 = 0;
    int hiScore1 = 0;
    int hiScore2 = 0;
    
    // Xwindow wrapper
    unique_ptr<Xwindow> xw;
    
    int cellSize = 20;  // Size of each cell in pixels
    bool textOnly = false;
    
    void drawCell(int x, int y, char type);
    int getColorForType(char type);
    void drawBlock(Block* block, int offsetX, int offsetY);
    
public:
    GraphicsDisplay(bool textOnly = false);
    ~GraphicsDisplay() = default;
    
    void renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2, bool blind1, bool blind2);
    void renderNext(Block* next1, Block* next2);
    void renderScores(int s1, int s2, int hi1, int hi2);
    void update();  // Actually render to X11 window
};