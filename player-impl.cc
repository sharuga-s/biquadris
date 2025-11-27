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
import Level0;
import Level1;
import Level2;
import Level3;
import Level4;

import <algorithm>;
import <memory>;   // for unique_ptr and make_unique

// =========================
//  Helper: Create new block
// =========================
std::unique_ptr<Block> Player::createBlockFromType(char type) {
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
//  Rebuild Level on levelUp/down
// =========================
void Player::rebuildLevel() {
    // Ask the current level for its sequence file (works for 0/3/4)
    std::string seqFile = levelLogic->getSequenceFile();

    // Boundaries
    if (levelNumber < 0) levelNumber = 0;
    if (levelNumber > 4) levelNumber = 4;

    // Rebuild using pure polymorphism
    switch (levelNumber) {
        case 0:
            if (seqFile.empty()) seqFile = "sequence1.txt";
            levelLogic = std::make_unique<Level0>(seqFile);
            break;

        case 1:
            levelLogic = std::make_unique<Level1>();
            break;

        case 2:
            levelLogic = std::make_unique<Level2>();
            break;

        case 3: {
            auto lvl = std::make_unique<Level3>();
            if (!seqFile.empty()) lvl->setSequenceFile(seqFile);
            levelLogic = std::move(lvl);
            break;
        }

        case 4: {
            auto lvl = std::make_unique<Level4>();
            if (!seqFile.empty()) lvl->setSequenceFile(seqFile);
            levelLogic = std::move(lvl);
            break;
        }
    }

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
    currBlock = std::move(nextBlock);

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
        heldBlock = std::move(currBlock);
        currBlock = std::move(nextBlock);

        char nextType = levelLogic->generateNextBlockType();
        nextBlock = createBlockFromType(nextType);
        applyHeavy(nextBlock.get());
    } else {
        std::swap(currBlock, heldBlock);
    }

    hasHeldThisTurn = true;

    if (currBlock && !grid.isValid(currBlock.get())) {
        isGameOver = true;
    }
}

// =========================
//  Constructor
// =========================
Player::Player(std::unique_ptr<Level> logic, int lvl)
    : levelLogic(std::move(logic)),
      levelNumber(lvl),
      grid(18, 11) {

    spawnInitialBlocks();
}

// =========================
//  Destructor — defaulted!
// =========================
Player::~Player() = default;

// =========================
//  Movement
// =========================
void Player::moveBlockLeft() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol() - 1;

    if (grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setPosition(r, c);

        // Heavy rule: extra drop after horizontal movement
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

        // Heavy rule: extra drop after horizontal movement
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

    // Manual "down" does NOT apply heavy extra-drop
    if (grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setPosition(r, c);
    }
}


// =======================================================
//  ROTATION (controller-based, with revert + heavy drop)
// =======================================================
void Player::rotateCW() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol();

    // Save old state
    auto oldCells = currBlock->getCells();
    int oldRot = currBlock->getRotation();

    // Local rotation only (no Grid)
    currBlock->rotateCWLocal();

    // If illegal, revert
    if (!grid.isValidPosition(*currBlock, r, c)) {
        currBlock->setCells(oldCells);
        currBlock->setRotation(oldRot);
        return;
    }

    // Heavy: apply forced downward movement if possible
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
//  DROP + scoring + heavy/level logic
// =========================
void Player::dropBlock() {
    if (isGameOver || !currBlock) return;

    int r = currBlock->getRow();
    int c = currBlock->getCol();

    // Fall until cannot
    while (grid.isValidPosition(*currBlock, r + 1, c)) {
        r += 1;
        currBlock->setPosition(r, c);
    }

    // Place onto grid
    grid.placeBlock(currBlock.get());

    // Blind clears on drop
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

// returns whether or not the current level is heavy + whether or not there's heavy effect in play, caused by opposite player
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

    spawnInitialBlocks();
}
