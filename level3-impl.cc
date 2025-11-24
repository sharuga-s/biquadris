module Level3;

import Level3;
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
    uniform_int_distribution<int> dist(0, 11);
    int r = dist(rng);

    if (r == 0) {
        return new SBlock(levelNumber);
    } else if (r == 1) {
        return new ZBlock(levelNumber);
    } else if (r == 2 || r == 3) {
        return new IBlock(levelNumber);
    } else if (r == 4 || r == 5) {
        return new JBlock(levelNumber);
    } else if (r == 6 || r == 7) {
        return new LBlock(levelNumber);
    } else if (r == 8 || r == 9) {
        return new OBlock(levelNumber);
    } else {
        return new TBlock(levelNumber);
    }
}
