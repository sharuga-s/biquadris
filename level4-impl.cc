module Level4;

import Level;

import <fstream>;
import <vector>;
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

void Level4::genBlocksFromFile() {
    sequence.clear();
    sequenceIndex = 0;
    
    if (filename.empty()) return;
    
    ifstream in{filename};
    if (!in) return;
    
    char ch;
    while (in >> ch) {
        sequence.push_back(ch);
    }
}

void Level4::readFile(string file) {
    filename = file;
    genBlocksFromFile();
}

char Level4::generateNextBlockType() {
    if (!randomMode && !sequence.empty()) {
        char type = sequence[sequenceIndex];
        sequenceIndex = (sequenceIndex + 1) % sequence.size();  // wrap-around logic
        return type;
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
        return 'S';
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

void Level4::setSequenceFile(const std::string& file) {
    readFile(file);
}

string Level4::getSequenceFile() const {
    return filename;
}
