export module Player;

import Level;
import Grid;
import SpecialAction;
import Block;

import <memory>;

using namespace std;

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
        unique_ptr<Level> levelLogic; // stores level logic
        int levelNumber; 
        Grid grid; // owns-a
        // blocks owned by player at an instnance
        unique_ptr<Level> currBlock = nullptr;
        unique_ptr<Block> nextBlock = nullptr;
        unique_ptr<Block> heldBlock = nullptr;
        bool hasHeldThisTurn = false; // esnsure at most one hold per turn
        bool isBlind = false;
        bool isGameOver = false;
        int heavyEffects = 0;

        // helper functions
        void spawnInitialBlocks();
        void promoteNextBlock();
        void applyHeavy(Block* b);
        void clearAllBlocks();
        unique_ptr<Block> createBlockFromType(char type);
    public:
        // ctor 
        Player(Level* levelImpl, int levelNum);

        // movement logic for a block
        void moveBlockLeft();
        void moveBlockRight();
        void moveBlockDown(); 
        void rotateCW();
        void rotateCCW();
        void dropBlock();
        void holdBlock();

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
        bool getBlind() const;
        void setBlind(bool b);
        Level* Player::getLevelLogic() const;

        // effects
        void applyEffect(SpecialAction* effect); 
        void incrementHeavyEffects();  
        void decrementHeavyEffects();  
        bool isHeavy() const; 

        // game logic
        void incLevel(); // move level up
        void decLevel(); // move level down
        void forceNextBlock(char type); // FORCE next block special action
        void setRandomMode(bool enabled);
        void reset();

        // special action state
        bool hasSpecialAction() const { return specialActionTriggered; }
        void clearSpecialAction() { specialActionTriggered = false; }
        int getNumSpecialActions() const { return numSpecialActions; }
};