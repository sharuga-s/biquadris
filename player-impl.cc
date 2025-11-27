module Player;

using namespace std;

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
//  Apply heavy logic
// =========================
void Player::applyHeavy(Block* b) {
    if (!b) return;
    bool heavy = levelLogic->isHeavy() || heavyEffects > 0;
    b->setHeavy(heavy);
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

    // Update heavy state on existing blocks
    applyHeavy(currBlock.get());
    applyHeavy(nextBlock.get());
}

// =========================
//  Spawn initial block pair
// =========================
void Player::spawnInitialBlocks() {
    char t1 = levelLogic->generateNextBlockType();
    currBlock = createBlockFromType(t1);
    applyHeavy(currBlock.get());

    char t2 = levelLogic->generateNextBlockType();
    nextBlock = createBlockFromType(t2);
    applyHeavy(nextBlock.get());

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
    applyHeavy(nextBlock.get());

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
        applyHeavy(nextBlock.get());
    } else {
        swap(currBlock, heldBlock);
    }

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
      grid(18, 11) {
    
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
void Player::moveBlockLeft() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol() - 1;

    if (grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setPosition(r, c);

        if (currBlock->isBlockHeavy()) {
            int hr = r + 1;
            if (grid.isValidPosition(*currBlock, hr, c)) {
                currBlock->setPosition(hr, c);
            }
        }
    }
}

void Player::moveBlockRight() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol() + 1;

    if (grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setPosition(r, c);

        if (currBlock->isBlockHeavy()) {
            int hr = r + 1;
            if (grid.isValidPosition(*currBlock, hr, c)) {
                currBlock->setPosition(hr, c);
            }
        }
    }
}

void Player::moveBlockDown() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow() + 1;
    int c = currBlock->getCol();

    if (grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setPosition(r, c);
    }
}

// =========================
//  ROTATION
// =========================
void Player::rotateCW() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol();

    auto oldCells = currBlock->getCells();
    int oldRot = currBlock->getRotation();

    currBlock->rotateCWLocal();

    if (!grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setCells(oldCells);
        currBlock->setRotation(oldRot);
        return;
    }

    if (currBlock->isBlockHeavy()) {
        int hr = r + 1;
        if (grid.isValidPosition(*currBlock, hr, c)) {
            currBlock->setPosition(hr, c);
        }
    }
}

void Player::rotateCCW() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol();

    auto oldCells = currBlock->getCells();
    int oldRot = currBlock->getRotation();

    currBlock->rotateCCWLocal();

    if (!grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setCells(oldCells);
        currBlock->setRotation(oldRot);
        return;
    }

    if (currBlock->isBlockHeavy()) {
        int hr = r + 1;
        if (grid.isValidPosition(*currBlock, hr, c)) {
            currBlock->setPosition(hr, c);
        }
    }
}

// =========================
//  DROP + scoring
// =========================
void Player::dropBlock() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol();

    while (grid.isValidPosition(*currBlock, r + 1, c)) {
        r += 1;
        currBlock->setPosition(r, c);
    }

    grid.placeBlock(currBlock.get());

    isBlind = false;

    int numCleared = 0;
    grid.clearFullRows(numCleared);

    if (numCleared > 0) {
        int pts = (levelNumber + numCleared) * (levelNumber + numCleared);
        addScore(pts);
    }

    levelLogic->onBlockPlaced(numCleared > 0);

    if (numCleared >= 2) {
        specialActionTriggered = true;
        numSpecialActions = numCleared;
    }

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

bool Player::isGameOver() const { return isGameOver; }

// =========================
//  Special effects
// =========================
void Player::applyEffect(SpecialAction* effect) {
    if (!effect || isGameOver) return;
    effect->apply(*this, grid);
}

void Player::incrementHeavyEffects() {
    heavyEffects++;
    applyHeavy(currBlock.get());
    applyHeavy(nextBlock.get());
}

void Player::decrementHeavyEffects() {
    if (heavyEffects > 0) heavyEffects--;
    applyHeavy(currBlock.get());
    applyHeavy(nextBlock.get());
}

bool Player::isHeavy() const {
    return levelLogic->isHeavy() || heavyEffects > 0;
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
    applyHeavy(nextBlock.get());
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
    heavyEffects = 0;
    isGameOver = false;
    hasHeldThisTurn = false;
    specialActionTriggered = false;
    numSpecialActions = 0;

    currBlock.reset();
    nextBlock.reset();
    heldBlock.reset();

    rebuildLevel();

    spawnInitialBlocks();
}