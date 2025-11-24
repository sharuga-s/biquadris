module Level2;

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

// Level 2: All blocks are selected with equal probability

Level2::Level2()
    : levelNumber{2}
{}

bool Level2::isHeavy() const {
    return false;
}

void Level2::onBlockPlaced(bool clearedRows) {
    // Level 2 does nothing here
}

void Level2::readFile(string) {
    // Level 2 does not use sequence files
}

void Level2::genBlocksFromFile() {
    // Level 2 does not use sequence files
}

Block* Level2::getNextBlock() {
    
    // choose random number 0–6
    int r = rand() % 7;

    if (r == 0) {
        return new IBlock(levelNumber);
    } 
    else if (r == 1) {
        return new JBlock(levelNumber);
    } 
    else if (r == 2) {
        return new LBlock(levelNumber);
    } 
    else if (r == 3) {
        return new OBlock(levelNumber);
    } 
    else if (r == 4) {
        return new SBlock(levelNumber);
    } 
    else if (r == 5) {
        return new ZBlock(levelNumber);
    } 
    else {
        return new TBlock(levelNumber);
    }
}
