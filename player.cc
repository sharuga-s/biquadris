export module Player;

import Level;
import Grid;
import SpecialAction;
import Block;

// other imports here do not forget

/*
1. score : int
2. hiScore : int
3. level : Level* 
4. currBlock : Block*
5. nextBlock : Block*
6. heldBlock: Block*
7. isHolding : bool
8. grid : Grid
9. isBlind : bool
10. gameOver : bool
11. alive : bool
12. heavy : int (0, 1, 2)

relationships:
- OWNS-A [aggregation] SpecialAction
- HAS-A [composition] Block
- HAS-A [composition] Grid
- HAS-A [composition] Level 
*/

export class Player {
    private:
        // fields 
        int score = 0; // player curr score
        int hiScore = 0; // player maximum in one 'session'
        Level* levelLogic; // stores level logic
        int levelNumber; 
        Grid grid; // owns-a
        // blocks owned by player at an instnance
        Block* currBlock = nullptr;
        Block* nextBlock = nullptr;
        Block* heldBlock = nullptr;
        bool hasHeldThisTurn = false; // esnsure at most one hold per turn
        bool isBlind = false;
        bool isGameOver = false;
        int heavyEffects = 0;

        // helper functions
        void spawnInitialBlocks();
        void promoteNextBlock();
        void applyHeavy(Block* b) const;
        void clearAllBlocks();
        Block* createBlockFromType(char type);
    public:
        // ctor and dtor
        Player(Level* levelImpl, int levelNum);
        ~Player();

        // movement logic for a block
        void moveBlockLeft();
        void moveBlockRight();
        void moveBlockDown(); 
        void rotateCW();
        void rotateCCW();
        void dropBlock();
        void holdBlock();
        bool softDrop(Grid &g);

        // accessors/getters and setters
        Grid& getGrid();
        const Grid& getGrid() const;
        Block* getCurrentBlock() const;
        Block* getNextBlock() const;
        Block* getHeldBlock() const;
        int getScore() const;
        int getHiScore() const;
        int getLevel() const;
        void setLevel(Level* newLevelImpl, int newLevelNum);
        void addScore(int ptsToAdd);
        void setBlind(bool b);
        bool getBlind() const;

        // effects
        void applyEffect(SpecialAction* effect); 

        // game logic
        void incLevel(); // move level up
        void decLevel(); // move level down
        void forceNextBlock(char type); // FORCE next block special action
        void setRandomMode(bool enabled);
        void reset();
};