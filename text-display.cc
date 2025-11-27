export module TextDisplay;

import Grid;
import Block;

export class TextDisplay {
private:
    // stored data for rendering
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
    
public:
    TextDisplay();
    
    void renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2);
    void renderNext(Block* next1, Block* next2);
    void renderScores(int s1, int s2, int hi1, int hi2);
    void update();  // print ts to cout
};