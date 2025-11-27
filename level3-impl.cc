module Level3;

import Level;

import <string>;
import <cstdlib>;

using namespace std;

// Level 3: The block selector will randomly choose a block with probabilities skewed such that
// S and Z blocks are selected with probability 2/9 each, and the other blocks are selected with
// probability 1/9 each. Moreover, blocks generated in level 3 are “heavy”: every command to
// move or rotate the block will be followed immediately and automatically by a downward move
// of one row (if possible)

Level3::Level3()
    : levelNumber{3}, sequenceIndex{0}
{}

bool Level3::isHeavy() const {
    return true;
}

void Level3::onBlockPlaced(bool clearedRows) {
    // nothing happens
}

void Level3::genBlocksFromFile() {
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

void Level3::readFile(string file) {
    filename = file;
    genBlocksFromFile();
}

char Level3::generateNextBlockType() {
    
    if (!randomMode && !sequence.empty()) {
        char type = sequence[sequenceIndex];
        sequenceIndex = (sequenceIndex + 1) % sequence.size();  // wrap-around logic
        return type;
    }
  
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

void Level3::setRandom(bool enabled) {
    randomMode = enabled;
}

void Level3::getSequenceFile(const std::string& file) override {
    readFile(file);   
}

string Level3::getSequenceFile() const override {
    return filename;
}
