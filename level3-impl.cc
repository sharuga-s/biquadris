module Level3;

import Block;

import IBlock;
import JBlock;
import LBlock;
import OBlock;
import SBlock;
import ZBlock;
import TBlock;

import <string>;
import <cstdlib>;

using namespace std;

// Level 3: The block selector will randomly choose a block with probabilities skewed such that
// S and Z blocks are selected with probability 2/9 each, and the other blocks are selected with
// probability 1/9 each. Moreover, blocks generated in level 3 are “heavy”: every command to
// move or rotate the block will be followed immediately and automatically by a downward move
// of one row (if possible)

Level3::Level3()
    : levelNumber{3}
{}

bool Level3::isHeavy() const {
    return true;
}

void Level3::onBlockPlaced(bool clearedRows) {
    // Level 3 does nothing special here
}

void Level3::readFile(string) {
    // Level 3 does not use sequence files
}

void Level3::genBlocksFromFile() {
    // Level 3 does not use sequence files
}

Block* Level3::getNextBlock() {
    if (!randomMode) {
        // TODO: use sequence from file via genBlocksFromFile()
    }
    
    int r = rand() % 9;

    if (r == 0 || r == 1) {
        // S = 2/9
        return new SBlock(levelNumber);
    } else if (r == 2 || r == 3) {
        // Z = 2/9
        return new ZBlock(levelNumber);
    } else if (r == 4) {
        return new IBlock(levelNumber);
    } else if (r == 5) {
        return new JBlock(levelNumber);
    } else if (r == 6) {
        return new LBlock(levelNumber);
    } else if (r == 7) {
        return new OBlock(levelNumber);
    } else { // r == 8
        return new TBlock(levelNumber);
    }
}

void Level3::setRandom(bool enabled) {
    randomMode = enabled;
}