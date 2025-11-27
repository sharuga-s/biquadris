export module TextDisplay;

import Display;
import Grid;
import Block;

// Forward declaration to avoid circular dependency
export class GameEngine;

export class TextDisplay : public Display {
private:
    // Reference to the subject (game)
    GameEngine* game = nullptr;
    
    // Cached data for rendering
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
    
    // Internal rendering methods
    void renderBoard(const Grid& g1, const Grid& g2, int lvl1, int lvl2);
    void renderNext(Block* next1, Block* next2);
    void renderScore(int s1, int s2, int hi1, int hi2);
    void update();  // Actually print to cout
    
public:
    TextDisplay();
    explicit TextDisplay(GameEngine* g);
    ~TextDisplay() override = default;
    
    // Observer pattern: notify method
    void notify() override;
    
    void setGame(GameEngine* g) { game = g; }
};