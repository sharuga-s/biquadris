// level2-impl.cc
module Level2;

import Level2;
import Block;

import IBlock;
import JBlock;
import LBlock;
import OBlock;
import SBlock;
import ZBlock;
import TBlock;

import <random>;
import <string>;

using namespace std;

// RNG
static mt19937 rng(random_device{}());

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
    uniform_int_distribution<int> dist(0, 6);
    int r = dist(rng);

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
