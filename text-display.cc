export module TextDisplay;

import Display;
import Grid;
import Block;
import GameEngine;

export class TextDisplay : public Display {
private:
    // reference to the subject (game)
    GameEngine* game = nullptr;
    
    // cached data for rendering -- same reasn for caching as graphics
    const Grid* grid1 = nullptr;
    const Grid* grid2 = nullptr;
    Block* nextBlock1 = nullptr;
    Block* nextBlock2 = nullptr;

    Block* currBlock1 = nullptr;
    Block* currBlock2 = nullptr;

    int level1 = 0;
    int level2 = 0;
    int score1 = 0;
    int score2 = 0;
    int hiScore1 = 0;
    int hiScore2 = 0;
    bool isBlind1 = false;
    bool isBlind2 = false;
    
    // priv rendering methods
    void renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2, bool blind1, bool blind2);
    void renderNext(Block* next1, Block* next2);
    void renderScores(int s1, int s2, int hi1, int hi2);
    void update();  // atually print to cout
    
public:
    TextDisplay();
    explicit TextDisplay(GameEngine* g);
    ~TextDisplay() override = default;
    
    // observer pattern: notify method
    void notify() override;
    
    void setGame(GameEngine* g) { game = g; }
};
