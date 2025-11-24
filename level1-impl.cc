// level1-impl.cc
module Level1;

import Level1;
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
static std::mt19937 rng(std::random_device{}());

Level1::Level1()
    : levelNumber{1} 
{}

bool Level1::isHeavy() const {
    return false;
}

void Level1::onBlockPlaced(bool clearedRows) {
    // Level 1 does nothing here
}

void Level1::readFile(string) {
    // Level 1 ignores sequence files
}

void Level1::genBlocksFromFile() {
    // Level 1 ignores sequence files
}

Block* Level1::getNextBlock() {
    // Weighted distribution:
    // 0: S     (1)
    // 1: Z     (1)
    // 2: I     (2)
    // 3: J     (2)
    // 4: L     (2)
    // 5: O     (2)
    // 6: T     (2)

    // sets up a distribution that we can use our RNG on
    uniform_int_distribution<int> dist(0, 11);
    // find a num between 0-11
    int r = dist(rng);

    if (r == 0) {
        return new SBlock(levelNumber);
    } 
    else if (r == 1) {
        return new ZBlock(levelNumber);
    } 
    else if (r == 2 || r == 3) {
        return new IBlock(levelNumber);
    } 
    else if (r == 4 || r == 5) {
        return new JBlock(levelNumber);
    } 
    else if (r == 6 || r == 7) {
        return new LBlock(levelNumber);
    } 
    else if (r == 8 || r == 9) {
        return new OBlock(levelNumber);
    } 
    else { //r = 10 or r = 11
        return new TBlock(levelNumber);
    }
}
