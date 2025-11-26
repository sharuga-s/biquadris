module Player;

using namespace std;

import <algorithm>;

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
        gameOver = true;
    }
}

void Player::clearAllBlocks() {
    delete currBlock;
    delete nextBlock;
    delete heldBlock;
    currBlock = nextBlock = heldBlock = nullptr;
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
void Player::moveLeft() {
    if (!gameOver && currBlock) currBlock->moveLeft(grid);
}

void Player::moveRight() {
    if (!gameOver && currBlock) currBlock->moveRight(grid);
}

void Player::moveDown() {
    if (!gameOver && currBlock) currBlock->moveDown(grid);
}

void Player::rotateCW() {
    if (!gameOver && currBlock) currBlock->rotateCW(grid);
}

void Player::rotateCCW() {
    if (!gameOver && currBlock) currBlock->rotateCCW(grid);
}

void Player::dropBlock() {
    if (gameOver || !currBlock) return;

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
    if (gameOver || !currBlock) return;
    if (hasHeldThisTurn) return;

    if (!heldBlock) {
        heldBlock = currBlock;
        currBlock = nextBlock;
        nextBlock = levelLogic->getNextBlock();
        applyHeavyTo(nextBlock);
    } else {
        std::swap(currBlock, heldBlock); // idek if this allowed brochacho but keep for now
    }

    hasHeldThisTurn = true;

    if (!grid.isValid(currBlock)) {
        gameOver = true;
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
    levelLogic = newLogic;
    levelNumber = newLevelNum;

    applyHeavyTo(currBlock);
    applyHeavyTo(nextBlock);
}

// effects ----------------------------------------------------------------------
void Player::applyEffect(SpecialAction* effect) {
    if (!effect || gameOver) return;
    effect->apply(*this, grid);
}

bool Player::isHeavy() const {
    return levelLogic->isHeavy() || heavyEffects > 0;
}

// game logic --------------------------------------------------------------------
void Player::reset() {
    grid.reset();

    score = 0;
    blindActive = false;
    heavyEffects = 0;
    gameOver = false;
    hasHeldThisTurn = false;

    clearAllBlocks();
    spawnInitialBlocks();
}