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

import <algorithm>;

// WORK IN PROGRESS - if this is kept, player has to import ALL of level and subclasses
void Player::rebuildLevel() {
    // delete old Level
    delete levelLogic;
    levelLogic = nullptr;

    // levelNumber boundaries
    if (levelNumber < 0) levelNumber = 0;
    if (levelNumber > 4) levelNumber = 4;

    // construct appropriate concrete Level
    if (levelNumber == 0) {
        // TODO: pass sequence filename you used at startup
        levelLogic = new Level0("sequence.txt");  // placeholder
    } else if (levelNumber == 1) {
        levelLogic = new Level1();
    } else if (levelNumber == 2) {
        levelLogic = new Level2();
    } else if (levelNumber == 3) {
        levelLogic = new Level3();
    } else { // levelNumber == 4
        levelLogic = new Level4();
    }

    // re-apply heavy status based on new level
    applyHeavy(currBlock);
    applyHeavy(nextBlock);
}

// helper fns -----------------------------------------------------------------
void Player::applyHeavy(Block* b) const {
    if (!b) return;
    bool heavy = levelLogic->isHeavy() || heavyEffects > 0;
    b->setHeavy(heavy);
}

void Player::spawnInitialBlocks() {
    currBlock = levelLogic->getNextBlock();
    applyHeavy(currBlock);

    nextBlock = levelLogic->getNextBlock();
    applyHeavy(nextBlock);

    if (currBlock && !grid.isValid(currBlock)) {
        isGameOver = true;
    }
}

void Player::promoteNextBlock() {
    delete currBlock;
    currBlock = nextBlock;

    nextBlock = levelLogic->getNextBlock();
    applyHeavy(nextBlock);

    hasHeldThisTurn = false;
    isBlind = false;

    if (currBlock && !grid.isValid(currBlock)) {
        isGameOver = true;
    }
}

void Player::clearAllBlocks() {
    delete currBlock;
    delete nextBlock;
    delete heldBlock;
    currBlock = nextBlock = heldBlock = nullptr;
}

// ctor and dtor -----------------------------------------------------------------
Player::Player(Level* logic, int lvl)
    : levelLogic{logic}, levelNumber{lvl}, grid{18, 11} {
        spawnInitialBlocks();
}

Player::~Player() { clearAllBlocks(); }

// movement logic ----------------------------------------------------------------
void Player::moveBlockLeft() {
    if (!isGameOver && currBlock) currBlock->moveLeft(grid);
}

void Player::moveBlockRight() {
    if (!isGameOver && currBlock) currBlock->moveRight(grid);
}

void Player::moveBlockDown() {
    if (!isGameOver && currBlock) currBlock->moveDown(grid);
}

void Player::rotateCW() {
    if (!isGameOver && currBlock) currBlock->rotateCW(grid);
}

void Player::rotateCCW() {
    if (!isGameOver && currBlock) currBlock->rotateCCW(grid);
}

void Player::dropBlock() {
    if (isGameOver || !currBlock) return;

    // 1. hard drop: repeat until cant fall w/ new softDrop method from Block
    while (currBlock->softDrop(grid)) {}

    // 2. place
    grid.placeBlock(currBlock);

    // 3. clear rows + compute score
    int numCleared = 0;
    grid.clearFullRows(numCleared);

    if (numCleared > 0) {
        int pts = (levelNumber + numCleared) * (levelNumber + numCleared);
        addScore(pts);
    }

    // 4. Level notifications (for Level 4 star block, etc.)
    levelLogic->onBlockPlaced(numCleared > 0);

    // 5. spawn next block
    promoteNextBlock();
}

void Player::holdBlock() {
    if (isGameOver || !currBlock) return;
    if (hasHeldThisTurn) return;

    if (!heldBlock) {
        heldBlock = currBlock;
        currBlock = nextBlock;
        nextBlock = levelLogic->getNextBlock();
        applyHeavy(nextBlock);
    } else {
        std::swap(currBlock, heldBlock); // idek if this allowed brochacho but keep for now
    }

    hasHeldThisTurn = true;

    if (!grid.isValid(currBlock)) {
        isGameOver = true;
    }
}

// getters and setters ----------------------------------------------------------
Grid& Player:: getGrid() { return grid; }

const Grid& Player::getGrid() const { return grid; }

Block* Player::getCurrentBlock() const { return currBlock; }

Block* Player::getNextBlock()    const { return nextBlock; }

Block* Player::getHeldBlock()    const { return heldBlock; }

int Player::getScore() const { return score; }

int Player::getHiScore() const { return hiScore; }

void Player::addScore(int ptsToAdd) {
    score += ptsToAdd;
    if (score > hiScore) {
        hiScore = score;
    }
}

int Player::getLevel() const { return levelNumber; }

void Player::setLevel(Level* newLogic, int newLevelNum) {
    delete levelLogic; // delete old level to prevent memory leak
    levelLogic = newLogic;
    levelNumber = newLevelNum;

    applyHeavy(currBlock);
    applyHeavy(nextBlock);
}

// effects ----------------------------------------------------------------------
void Player::applyEffect(SpecialAction* effect) {
    if (!effect || isGameOver) return;
    effect->apply(*this, grid);
}

bool Player::isHeavy() const {
    return levelLogic->isHeavy() || heavyEffects > 0;
}

// game logic --------------------------------------------------------------------
void Player::levelUp() {
    ++levelNumber;
    if (levelNumber > 4) levelNumber = 4;
    rebuildLevel();
}

void Player::levelDown() {
    --levelNumber;
    if (levelNumber < 0) levelNumber = 0;
    rebuildLevel();
}

void Player::forceNextBlock(char type) {
    // delete current next block
    delete nextBlock;
    nextBlock = nullptr;

    int lvl = levelNumber;

    switch (type) {
        case 'I':
            nextBlock = new IBlock(lvl);
            break;
        case 'J':
            nextBlock = new JBlock(lvl);
            break;
        case 'L':
            nextBlock = new LBlock(lvl);
            break;
        case 'O':
            nextBlock = new OBlock(lvl);
            break;
        case 'S':
            nextBlock = new SBlock(lvl);
            break;
        case 'Z':
            nextBlock = new ZBlock(lvl);
            break;
        case 'T':
            nextBlock = new TBlock(lvl);
            break;
        default:
            // invalid type; leave nextBlock as nullptr or regenerate from level
            nextBlock = levelLogic->getNextBlock();
            break;
    }

    // ensure heavy rules still apply
    applyHeavy(nextBlock);
}

void Player::setRandomMode(bool enabled) {
    if (levelLogic) {
        levelLogic->setRandom(enabled);
    }
}

void Player::reset() {
    grid.reset();

    score = 0;
    blindActive = false;
    heavyEffects = 0;
    isGameOver = false;
    hasHeldThisTurn = false;

    clearAllBlocks();
    spawnInitialBlocks();
}