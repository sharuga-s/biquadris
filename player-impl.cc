module Player;

import IBlock;
import JBlock;
import LBlock;
import OBlock;
import SBlock;
import ZBlock;
import TBlock;
import StarBlock;
import LevelFactory;  

import <algorithm>;
import <memory>;
import <string>;
import <iostream>;

using namespace std;

// =========================
//  Helper: Create new block
// =========================
unique_ptr<Block> Player::createBlockFromType(char type) {
    switch (type) {
        case 'I': return make_unique<IBlock>(levelNumber);
        case 'J': return make_unique<JBlock>(levelNumber);
        case 'L': return make_unique<LBlock>(levelNumber);
        case 'O': return make_unique<OBlock>(levelNumber);
        case 'S': return make_unique<SBlock>(levelNumber);
        case 'Z': return make_unique<ZBlock>(levelNumber);
        case 'T': return make_unique<TBlock>(levelNumber);
        case '*': return make_unique<StarBlock>(levelNumber);
    }
    return nullptr;
}

// =========================
//  Rebuild Level on levelUp/down - NOW USES FACTORY
// =========================
void Player::rebuildLevel() {
    // Clamp boundaries
    if (levelNumber < 0) levelNumber = 0;
    if (levelNumber > 4) levelNumber = 4;

    // Use factory to create new level, preserving sequence file
    levelLogic = LevelFactory::createLevelPreservingSequence(levelNumber, levelLogic.get());
}

// =========================
 //  Spawn initial block pair
// =========================
void Player::spawnInitialBlocks() {
    char t1 = levelLogic->generateNextBlockType();
    currBlock = createBlockFromType(t1);

    char t2 = levelLogic->generateNextBlockType();
    nextBlock = createBlockFromType(t2);

    if (currBlock && !grid.isValid(currBlock.get())) {
        isGameOver = true;
    }
}

// =========================
//  Promote nextBlock → currBlock
// =========================
void Player::promoteNextBlock() {
    currBlock = move(nextBlock);

    char nextType = levelLogic->generateNextBlockType();
    nextBlock = createBlockFromType(nextType);

    hasHeldThisTurn = false;

    if (currBlock && !grid.isValid(currBlock.get())) {
        isGameOver = true;
    }
}

// =========================
//  Hold / Swap blocks
// =========================
void Player::holdBlock() {
    if (isGameOver || !currBlock) return;
    if (hasHeldThisTurn) return;

    if (!heldBlock) {
        heldBlock = move(currBlock);
        currBlock = move(nextBlock);

        char nextType = levelLogic->generateNextBlockType();
        nextBlock = createBlockFromType(nextType);
    } else {
        swap(currBlock, heldBlock);
    }

    currBlock->setPosition(3, 0);

    hasHeldThisTurn = true;

    if (currBlock && !grid.isValid(currBlock.get())) {
        isGameOver = true;
    }
}

// =========================
//  Constructor - NOW USES FACTORY
// =========================
Player::Player(int levelNum, const string& sequenceFile)
    : levelNumber(levelNum),
      grid(18, 11),
      justDropped{false} {
    
    // Clamp level number
    if (levelNumber < 0) levelNumber = 0;
    if (levelNumber > 4) levelNumber = 4;
    
    // Use factory to create initial level
    levelLogic = LevelFactory::createLevel(levelNumber, sequenceFile);
    
    spawnInitialBlocks();
}

// =========================
//  Movement
// =========================
// Replace the movement methods in player-impl.cc with these:

// Replace the movement methods in player-impl.cc with these:

void Player::moveBlockLeft() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol() - 1;

    if (!grid.isValidPosition(*currBlock, r, c)) {
        return;  // Can't move left
    }

    // Move left is valid
    currBlock->setPosition(r, c);

    // Apply Heavy effect (moves down 2 rows per heavy source)
    int totalHeavy = (currBlock->isBlockHeavy() ? 1 : 0) + (levelLogic->isHeavy() ? 1 : 0);
    if (totalHeavy > 0) {
        int rowsMoved = 0;
        int targetRows = 2 * totalHeavy;
        
        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, c)) {
                currBlock->setPosition(newRow, c);
                rowsMoved++;
            } else {
                // Hit something - drop immediately
                dropBlock();
                return;
            }
        }
    }

    // After move (with or without heavy), check if block is now grounded
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, c)) {
        dropBlock();
    }
}

void Player::moveBlockRight() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol() + 1;

    if (!grid.isValidPosition(*currBlock, r, c)) {
        return;  // Can't move right
    }

    // Move right is valid
    currBlock->setPosition(r, c);

    // Apply Heavy effect
    int totalHeavy = (currBlock->isBlockHeavy() ? 1 : 0) + (levelLogic->isHeavy() ? 1 : 0);
    if (totalHeavy > 0) {
        int rowsMoved = 0;
        int targetRows = 2 * totalHeavy;
        
        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, c)) {
                currBlock->setPosition(newRow, c);
                rowsMoved++;
            } else {
                // Hit something - drop immediately
                dropBlock();
                return;
            }
        }
    }

    // After move (with or without heavy), check if block is now grounded
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, c)) {
        dropBlock();
    }
}

void Player::moveBlockDown() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow() + 1;
    int c = currBlock->getCol();

    if (!grid.isValidPosition(*currBlock, r, c)) {
        // Can't move down - drop immediately
        dropBlock();
        return;
    }

    // Move down is valid
    currBlock->setPosition(r, c);

    // Apply Heavy effect
    int totalHeavy = (currBlock->isBlockHeavy() ? 1 : 0) + (levelLogic->isHeavy() ? 1 : 0);
    if (totalHeavy > 0) {
        int rowsMoved = 0;
        int targetRows = 2 * totalHeavy;
        
        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, c)) {
                currBlock->setPosition(newRow, c);
                rowsMoved++;
            } else {
                // Hit something - drop immediately
                dropBlock();
                return;
            }
        }
    }

    // After move (with or without heavy), check if block is now grounded
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, c)) {
        dropBlock();
    }
}

void Player::rotateCW() {
    if (isGameOver || !currBlock) return;

    int oldRow = currBlock->getRow();
    int oldCol = currBlock->getCol();
    auto oldCells = currBlock->getCells();
    int oldRot = currBlock->getRotation();

    // Apply rotation
    currBlock->rotateCWLocal();

    // Check if rotation is valid
    if (!grid.isValidPosition(*currBlock, oldRow, oldCol)) {
        // Revert rotation
        currBlock->setCells(oldCells);
        currBlock->setRotation(oldRot);
        currBlock->setPosition(oldRow, oldCol);
        return;
    }

    // Rotation is valid, apply Heavy effect
    int totalHeavy = (currBlock->isBlockHeavy() ? 1 : 0) + (levelLogic->isHeavy() ? 1 : 0);
    if (totalHeavy > 0) {
        int rowsMoved = 0;
        int targetRows = 2 * totalHeavy;
        
        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, oldCol)) {
                currBlock->setPosition(newRow, oldCol);
                rowsMoved++;
            } else {
                // Hit something - drop immediately
                dropBlock();
                return;
            }
        }
    }

    // After rotation (with or without heavy), check if block is now grounded
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, oldCol)) {
        dropBlock();
    }
}

void Player::rotateCCW() {
    if (isGameOver || !currBlock) return;

    int oldRow = currBlock->getRow();
    int oldCol = currBlock->getCol();
    auto oldCells = currBlock->getCells();
    int oldRot = currBlock->getRotation();

    // Apply rotation
    currBlock->rotateCCWLocal();

    // Check if rotation is valid
    if (!grid.isValidPosition(*currBlock, oldRow, oldCol)) {
        // Revert rotation
        currBlock->setCells(oldCells);
        currBlock->setRotation(oldRot);
        currBlock->setPosition(oldRow, oldCol);
        return;
    }

    // Rotation is valid, apply Heavy effect
    int totalHeavy = (currBlock->isBlockHeavy() ? 1 : 0) + (levelLogic->isHeavy() ? 1 : 0);
    if (totalHeavy > 0) {
        int rowsMoved = 0;
        int targetRows = 2 * totalHeavy;
        
        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, oldCol)) {
                currBlock->setPosition(newRow, oldCol);
                rowsMoved++;
            } else {
                // Hit something - drop immediately
                dropBlock();
                return;
            }
        }
    }

    // After rotation (with or without heavy), check if block is now grounded
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, oldCol)) {
        dropBlock();
    }
}
// =========================
//  DROP + scoring
// =========================
void Player::dropBlock() {
    if (isGameOver || !currBlock) return;

    clearSpecialAction();

    int r = currBlock->getRow();
    int c = currBlock->getCol();

    while (grid.isValidPosition(*currBlock, r + 1, c)) {
        r += 1;
        currBlock->setPosition(r, c);
    }

    grid.placeBlock(currBlock.get());

    if (currBlock) currBlock->setHeavy(false);

    int numCleared = 0;
    grid.clearFullRows(numCleared);

    // Blind should clear at the END of this player's drop
    if (isBlind) {
        isBlind = false;
    }

    if (numCleared > 0) {
        int pts = (levelNumber + numCleared) * (levelNumber + numCleared);
        addScore(pts);
    }

    levelLogic->onBlockPlaced(numCleared > 0);

    if (numCleared >= 2) {
        specialActionTriggered = true;
        numSpecialActions = numCleared;
    }

    justDropped = true;

    promoteNextBlock();
}

// =========================
//  Getters
// =========================
Grid& Player::getGrid() { return grid; }
const Grid& Player::getGrid() const { return grid; }

Block* Player::getCurrentBlock() const { return currBlock.get(); }
Block* Player::getNextBlock() const { return nextBlock.get(); }
Block* Player::getHeldBlock() const { return heldBlock.get(); }

int Player::getScore() const { return score; }
int Player::getHiScore() const { return hiScore; }

void Player::addScore(int pts) {
    score += pts;
    if (score > hiScore) hiScore = score;
}

int Player::getLevel() const { return levelNumber; }

bool Player::getBlind() const { return isBlind; }
void Player::setBlind(bool b) { isBlind = b; }

Level* Player::getLevelLogic() const { return levelLogic.get(); }

bool Player::getGameOver() const { return isGameOver; }

bool Player::hasJustDropped() const {
    return justDropped;
}

void Player::clearJustDropped() {
    justDropped = false;
}

// =========================
//  Special effects
// =========================
void Player::applyEffect(SpecialAction* effect) {
    if (!effect || isGameOver) return;
    effect->apply(*this, grid);
}

bool Player::isHeavy() const {
    return levelLogic->isHeavy();
}

bool Player::hasSpecialAction() const {
    return specialActionTriggered;
}

void Player::clearSpecialAction() {
    specialActionTriggered = false;
    numSpecialActions = 0;
}

int Player::getNumSpecialActions() const {
    return numSpecialActions;
}

void Player::useOneSpecialAction() {
    if (numSpecialActions > 0) {
        --numSpecialActions;
        if (numSpecialActions == 0) {
            specialActionTriggered = false;
        }
    }
}

// =========================
//  Level up/down
// =========================
void Player::incLevel() {
    levelNumber++;
    if (levelNumber > 4) levelNumber = 4;
    rebuildLevel();
}

void Player::decLevel() {
    levelNumber--;
    if (levelNumber < 0) levelNumber = 0;
    rebuildLevel();
}

// =========================
//  Force next block
// =========================
void Player::forceNextBlock(char type) {
    nextBlock = createBlockFromType(type);
}

// =========================
//  Random mode toggle
// =========================
void Player::setRandomMode(bool enabled) {
    if (levelLogic) levelLogic->setRandom(enabled);
}

// =========================
//  Full reset between games
// =========================
void Player::reset() {
    grid.reset();

    score = 0;
    isBlind = false;
    isGameOver = false;
    hasHeldThisTurn = false;
    specialActionTriggered = false;
    numSpecialActions = 0;
    justDropped = false;

    currBlock.reset();
    nextBlock.reset();
    heldBlock.reset();

    rebuildLevel();

    spawnInitialBlocks();
}
