export module GraphicsDisplay;

import Grid;
import Block;

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
    
    // X11 window stuff (will be nullptr in text-only mode)
    void* display = nullptr;  // X11 Display*
    void* window = nullptr;   // X11 Window
    void* gc = nullptr;       // X11 GC (graphics context)
    
    int cellSize = 20;  // Size of each cell in pixels
    bool initialized = false;
    
    void initX11();
    void drawCell(int x, int y, char type);
    int getColorForType(char type);
    
public:
    GraphicsDisplay(bool textOnly = false);
    ~GraphicsDisplay();
    
    void renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2);
    void renderNext(Block* next1, Block* next2);
    void renderScores(int s1, int s2, int hi1, int hi2);
    void update();  // Actually render to X11 window
};
