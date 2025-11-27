module Level4;

import Block;

import IBlock;
import JBlock;
import LBlock;
import OBlock;
import SBlock;
import ZBlock;
import TBlock;
import StarBlock;

import <string>;
import <cstdlib>;

using namespace std;

// Level 4: In addition to the rules of Level 3, in Level 4 there is an external constructive force:
// every time you place 5 (and also 10, 15, etc.) blocks without clearing at least one row, a
// 1x1 block (indicated by * in text, and by the colour brown in graphics) is dropped onto your
// game board in the centre column. Once dropped, it acts like any other block: if it completes
// a row, the row disappears. So if you do not act quickly, these blocks will work to eventually
// split your screen in two, making the game difficult to play

Level4::Level4()
    : levelNumber{4}, noClearStreak{0}
{}

string Level4::getFileName const {
    return "";
}

bool Level4::isHeavy() const {
    return true;
}

void Level4::onBlockPlaced(bool clearedRows) {
    if (clearedRows) {
        //if rows were cleared, then our streak is reset
        noClearStreak = 0;
    } else {
        //if not rows were cleared, we increment our streak to keep track of how many turns go by without clearing rows
        noClearStreak = noClearStreak + 1;
    }
}

void Level4::readFile(string) {
    // Level 4 ignores sequence files
}

void Level4::genBlocksFromFile() {
    // Level 4 ignores sequence files
}

char Level4::generateNextBlockType() {
    if (!randomMode) {
        // TODO: use sequence from file via genBlocksFromFile()
    }

    // special star block after 5 turns without clearing any rows
    if (noClearStreak == 5) {
        noClearStreak = 0;
        return '*'; //NOTE: we need to make sure that this block gets placed in the center col
    }

    //same random distribution as Level 3
    int r = rand() % 9;

    if (r == 0 || r == 1) {
        // S = 2/9
        return new 'S';
    } else if (r == 2 || r == 3) {
        // Z = 2/9
        return 'Z';
    } else if (r == 4) {
        return 'I';
    } else if (r == 5) {
        return 'J';
    } else if (r == 6) {
        return 'L';
    } else if (r == 7) {
        return 'O';
    } else { // r == 8
        return 'T';
    }
}


void Level4::setRandom(bool enabled) {
    randomMode = enabled;
}