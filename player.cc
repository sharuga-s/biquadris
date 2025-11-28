export module Player;

import Level;
import Grid;
import Block;

import <memory>;
import <string>;

using namespace std;

/*
relationships:
- OWNS-A [aggregation] SpecialAction
- HAS-A [composition] Block
- HAS-A [composition] Grid
- HAS-A [composition] Level 
*/

export class SpecialAction;

export class SpecialAction;

export class Player {
    private:
        // fields 
        int score = 0; // player curr score
        int hiScore = 0; // player maximum in one 'session'
        unique_ptr<Level> levelLogic; // stores level logic
        int levelNumber; 
        Grid grid; // owns-a
        // blocks owned by player at an instnance
        unique_ptr<Block> currBlock = nullptr;
        unique_ptr<Block> nextBlock = nullptr;
        unique_ptr<Block> heldBlock = nullptr;
        bool hasHeldThisTurn = false; // esnsure at most one hold per turn
        bool isBlind = false;
        bool isGameOver = false;
        bool specialActionTriggered = false;  
        int numSpecialActions = 0;
        bool justDropped = false;

        // helper functions
        void spawnInitialBlocks();
        void promoteNextBlock();
        unique_ptr<Block> createBlockFromType(char type);
        void rebuildLevel();
    public:
        // ctor 
        Player(int levelNum, const string& sequenceFile = "");
        ~Player() = default; // dtor

        // non-copyable but movable (needed for std::vector<Player>)
        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;

        Player(Player&&) noexcept = default;
        Player& operator=(Player&&) noexcept = default;

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
        void addScore(int ptsToAdd);
        bool getBlind() const;
        void setBlind(bool b);
        Level* getLevelLogic() const;
        bool getGameOver() const;
        bool isHeavy() const;
        bool hasJustDropped() const;
        void clearJustDropped();

        // effects
        void applyEffect(SpecialAction* effect); 

        // game logic
        void incLevel(); // move level up
        void decLevel(); // move level down
        void forceNextBlock(char type); // FORCE next block special action
        void setRandomMode(bool enabled);
        void reset();

        // special action state
        bool hasSpecialAction() const;
        void clearSpecialAction();
        int getNumSpecialActions() const;
        void useOneSpecialAction();
};

export class SpecialAction {
    public:
        virtual ~SpecialAction() = default;
        virtual void apply(Player& p, Grid& g) = 0;
};
