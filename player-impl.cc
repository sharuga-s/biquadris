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

const int GRID_ROWS = 18;
const int GRID_COLS = 11;

//helper to transform type to Block
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

//levelup/down
void Player::rebuildLevel() {
    // boundaries
    if (levelNumber < 0) levelNumber = 0;
    if (levelNumber > 4) levelNumber = 4;

    // use LevelFactory to create new level
    // //preserve sequence file
    levelLogic = LevelFactory::createLevelPreservingSequence(levelNumber, levelLogic.get());
}

//spawn initial blow pairs
void Player::spawnInitialBlocks() {
    char t1 = levelLogic->generateNextBlockType();
    currBlock = createBlockFromType(t1);

    char t2 = levelLogic->generateNextBlockType();
    nextBlock = createBlockFromType(t2);

    if (currBlock && !grid.isValid(currBlock.get())) {
        isGameOver = true;
    }
}

//move nextBlock -> currBlock
void Player::promoteNextBlock() {
    currBlock = move(nextBlock);

    char nextType = levelLogic->generateNextBlockType();
    nextBlock = createBlockFromType(nextType);

    hasHeldThisTurn = false;

    if (currBlock && !grid.isValid(currBlock.get())) {
        isGameOver = true;
    }
}

//hold/swap blocks
void Player::holdBlock() {
    if (isGameOver || !currBlock) return;
    if (hasHeldThisTurn) return;

    if (!heldBlock) {
        heldBlock = move(currBlock);
        currBlock = move(nextBlock);

        char nextType = levelLogic->generateNextBlockType();
        nextBlock = createBlockFromType(nextType);
    } else {
        swap(currBlock, heldBlock); //handles unique_ptrs
    }

    currBlock->setPosition(3, 0);

    hasHeldThisTurn = true;

    if (currBlock && !grid.isValid(currBlock.get())) {
        isGameOver = true;
    }
}

//ctor -> uses LevelFactory now
Player::Player(int levelNum, const string& sequenceFile)
    : levelNumber(levelNum),
      grid(GRID_ROWS, GRID_COLS),
      justDropped{false} {
    
    //boundaries
    if (levelNumber < 0) levelNumber = 0;
    if (levelNumber > 4) levelNumber = 4;
    
    //create initial level
    levelLogic = LevelFactory::createLevel(levelNumber, sequenceFile);
    
    spawnInitialBlocks();
}

//move blocks
void Player::moveBlockLeft() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol() - 1;

    if (!grid.isValidPosition(*currBlock, r, c)) {
        return;
    }

    currBlock->setPosition(r, c);

    int totalHeavy = (currBlock->isBlockHeavy() ? 1 : 0) + (levelLogic->isHeavy() ? 1 : 0);
    if (totalHeavy > 0) {
        int rowsMoved = 0;
        
        int targetRows = 0;

        // if level is Heavy (levels 3/4): fall 1 row
        if (levelLogic->isHeavy()) {
            targetRows += 1;
        }

        // if curr block is heavy from special action: fall (an extra) 2 rows
        if (currBlock->isBlockHeavy()) {
            targetRows += 2;
        }

        
        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, c)) {
                currBlock->setPosition(newRow, c);
                rowsMoved++;
            } else {
                // drop AS SOON AS the block hits smth
                dropBlock();
                return;
            }
        }
    }

    //check if block is now fallen
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, c)) {
        dropBlock();
    }
}

//move currBlock right
void Player::moveBlockRight() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol() + 1;

    if (!grid.isValidPosition(*currBlock, r, c)) {
        return;
    }

    currBlock->setPosition(r, c);

    // apply Heavy effect
    int totalHeavy = (currBlock->isBlockHeavy() ? 1 : 0) + (levelLogic->isHeavy() ? 1 : 0);
    if (totalHeavy > 0) {
        int rowsMoved = 0;
        int targetRows = 0;

        // if level is heavy (3/4): fall 1 row
        if (levelLogic->isHeavy()) {
            targetRows += 1;
        }

        // if current block is heavy from special action: fall 2 rows
        if (currBlock->isBlockHeavy()) {
            targetRows += 2;
        }

        
        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, c)) {
                currBlock->setPosition(newRow, c);
                rowsMoved++;
            } else {
                dropBlock();
                return;
            }
        }
    }

    // check if block is now grounded
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, c)) {
        dropBlock();
    }
}

//move currBlock down
void Player::moveBlockDown() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow() + 1;
    int c = currBlock->getCol();

    if (!grid.isValidPosition(*currBlock, r, c)) {
        // can't move down --> drop immediately
        dropBlock();
        return;
    }

    currBlock->setPosition(r, c);

    // apply Heavy effect
    bool levelHeavy = levelLogic->isHeavy();
    // special-action heavy does NOT apply to "down"

    if (levelHeavy) {
        int targetRows = 1;  // only level heavy: +1 row

        for (int i = 0; i < targetRows; ++i) {
            int newRow = currBlock->getRow() + 1;
            if (grid.isValidPosition(*currBlock, newRow, c)) {
                currBlock->setPosition(newRow, c);
            } else {
                dropBlock();
                return;
            }
        }
    }


    // check if block is now grounded
    if (!grid.isValidPosition(*currBlock, currBlock->getRow() + 1, c)) {
        dropBlock();
    }
}

//rotate currBlock clockWise
void Player::rotateCW() {
    if (isGameOver || !currBlock) return;

    int oldRow = currBlock->getRow();
    int oldCol = currBlock->getCol();
    auto oldCells = currBlock->getCells();
    int oldRot = currBlock->getRotation();

    //position of the lower left corner of the smallest rectangle containing the block is preserved
    int oldMinRow = 999;
    int oldMaxRow = -1;
    int oldMinCol = 999;
    int oldMaxCol = -1;

    for (auto [r, c] : oldCells) {
        if (r < oldMinRow) oldMinRow = r;
        if (r > oldMaxRow) oldMaxRow = r;
        if (c < oldMinCol) oldMinCol = c;
        if (c > oldMaxCol) oldMaxCol = c;
    }

    currBlock->rotateCWLocal();

    //boundary info to meet the bottom left corner rule for rotations
    int newMinRow = 999, newMaxRow = -1;
    int newMinCol = 999, newMaxCol = -1;
    for (auto [r, c] : currBlock->getCells()) {
        if (r < newMinRow) newMinRow = r;
        if (r > newMaxRow) newMaxRow = r;
        if (c < newMinCol) newMinCol = c;
        if (c > newMaxCol) newMaxCol = c;
    }

    int oldBottomLeftRow = oldRow + oldMaxRow;
    int oldBottomLeftCol = oldCol + oldMinCol;

    // choose new (row,col) so that bottom-left stays fixed
    int newRow = oldBottomLeftRow - newMaxRow;
    int newCol = oldBottomLeftCol - newMinCol;

    currBlock->setPosition(newRow, newCol);

    // if rotation is illegal, revert
    if (!grid.isValidPosition(*currBlock, newRow, newCol)) {
        currBlock->setCells(oldCells);
        currBlock->setRotation(oldRot);
        currBlock->setPosition(oldRow, oldCol);
        return;
    }

    if (levelLogic->isHeavy()) {
        int hr = currBlock->getRow() + 1;
        if (grid.isValidPosition(*currBlock, hr, newCol)) {
            currBlock->setPosition(hr, newCol);
        } else {
            dropBlock();
            return;
        }
    }

    // now check if it’s grounded after heavy fall
    int belowRow = currBlock->getRow() + 1;
    if (!grid.isValidPosition(*currBlock, belowRow, newCol)) {
        dropBlock();
    }
}
//rotate currBlock counterClockWise
void Player::rotateCCW() {
    if (isGameOver || !currBlock) return;

    int oldRow = currBlock->getRow();
    int oldCol = currBlock->getCol();
    auto oldCells = currBlock->getCells();
    int oldRot = currBlock->getRotation();

    int oldMinRow = 999;
    int oldMaxRow = -1;
    int oldMinCol = 999;
    int oldMaxCol = -1;
    for (auto [r, c] : oldCells) {
        if (r < oldMinRow) oldMinRow = r;
        if (r > oldMaxRow) oldMaxRow = r;
        if (c < oldMinCol) oldMinCol = c;
        if (c > oldMaxCol) oldMaxCol = c;
    }

    currBlock->rotateCCWLocal();

    int newMinRow = 999, newMaxRow = -1;
    int newMinCol = 999, newMaxCol = -1;
    for (auto [r, c] : currBlock->getCells()) {
        if (r < newMinRow) newMinRow = r;
        if (r > newMaxRow) newMaxRow = r;
        if (c < newMinCol) newMinCol = c;
        if (c > newMaxCol) newMaxCol = c;
    }

    int oldBottomLeftRow = oldRow + oldMaxRow;
    int oldBottomLeftCol = oldCol + oldMinCol;

    int newRow = oldBottomLeftRow - newMaxRow;
    int newCol = oldBottomLeftCol - newMinCol;

    currBlock->setPosition(newRow, newCol);

    if (!grid.isValidPosition(*currBlock, newRow, newCol)) {
        currBlock->setCells(oldCells);
        currBlock->setRotation(oldRot);
        currBlock->setPosition(oldRow, oldCol);
        return;
    }

    //accomodate heavy levels, where it should fall even on rotations
    if (levelLogic->isHeavy()) {
        int hr = currBlock->getRow() + 1;
        if (grid.isValidPosition(*currBlock, hr, newCol)) {
            currBlock->setPosition(hr, newCol);
        } else {
            dropBlock();
            return;
        }
    }

    // now check if it’s grounded after heavy fall
    int belowRow = currBlock->getRow() + 1;
    if (!grid.isValidPosition(*currBlock, belowRow, newCol)) {
        dropBlock();
    }

}

// drop + scoring logic
void Player::dropBlock() {
    if (isGameOver || !currBlock) return;

    clearSpecialAction();

    int r = currBlock->getRow();
    int c = currBlock->getCol();

    // drop currBblock to lowest valid row
    while (grid.isValidPosition(*currBlock, r + 1, c)) {
        r += 1;
        currBlock->setPosition(r, c);
    }

    grid.placeBlock(currBlock.get());
    if (currBlock) currBlock->setHeavy(false);

    int numCleared = 0;
    grid.clearFullRows(numCleared);

    // blind effect clears at the end of drop
    if (isBlind) isBlind = false;

    // add points for cleared rows (note**dependent on Level)
    if (numCleared > 0) {
        int pts = (levelNumber + numCleared) * (levelNumber + numCleared);
        addScore(pts);
    }

    // notify level (for streak tracking)
    levelLogic->onBlockPlaced(numCleared > 0);

    // level 4 star logic (drop a star in middle for every 5 blocks placed without a cleared row)
        if (levelLogic->shouldDropStar()) {
        // center column of the board
        int midCol = grid.getCols() / 2;

        auto star = createBlockFromType('*');
        star->setPosition(0, midCol); 

        int sr = 0;
        while (grid.isValidPosition(*star, sr + 1, midCol)) {
            sr++;
            star->setPosition(sr, midCol);
        }

        grid.placeBlock(star.get());

        int extraCleared = 0;
        grid.clearFullRows(extraCleared);
        if (extraCleared > 0) {
            int pts = (levelNumber + extraCleared) * (levelNumber + extraCleared);
            addScore(pts);
        }
    }

    if (numCleared >= 2) {
        specialActionTriggered = true;
        numSpecialActions = numCleared;
    }

    justDropped = true;
    promoteNextBlock();

    if (isBlind) isBlind = false;
}

//getters
Grid& Player::getGrid() { return grid; }
const Grid& Player::getGrid() const { return grid; }

Block* Player::getCurrentBlock() const { return currBlock.get(); }
Block* Player::getNextBlock() const { return nextBlock.get(); }
Block* Player::getHeldBlock() const { return heldBlock.get(); }

int Player::getScore() const { return score; }
int Player::getHiScore() const { return hiScore; }

//note:depdendent on Levels and their rules
void Player::addScore(int pts) {
    score += pts;
    if (score > hiScore) {
        hiScore = score;
    }
}

int Player::getLevel() const { return levelNumber; }

bool Player::getBlind() const { return isBlind; }
void Player::setBlind(bool b) { isBlind = b; }

Level* Player::getLevelLogic() const { return levelLogic.get(); }

bool Player::getGameOver() const { return isGameOver; }

//needed for blocks that heavy specialAction drops
bool Player::hasJustDropped() const {
    return justDropped;
}

//after heavy effect is over with
void Player::clearJustDropped() {
    justDropped = false;
}

// special effects
void Player::applyEffect(SpecialAction* effect) {
    if (!effect || isGameOver) return;
    effect->apply(*this, grid);
}

//checks if associated Level is heavy
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

// level up and down
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

// force special action logic
void Player::forceNextBlock(char type) {
    currBlock = createBlockFromType(type);

    if (!currBlock) return;

    if (!grid.isValid(currBlock.get())) {
        isGameOver = true;
    }

}

// random mode toggle
void Player::setRandomMode(bool enabled) {
    if (levelLogic) levelLogic->setRandom(enabled);
}

// reset btwn games
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
