module Level4;

import Level;

import <fstream>;
import <vector>;
import <string>;
import <cstdlib>;

using namespace std;

Level4::Level4()
    : levelNumber{4} {}

bool Level4::isHeavy() const {
    // Level 4 inherits Level 3's heavy behaviour:
    // - For ALL moves (left/right/down)
    // - For rotations
    return true;
}

// ==========================================================
// Handle consecutive no-clear placements for star logic
// ==========================================================
void Level4::onBlockPlaced(bool clearedRows) {
    if (clearedRows) {
        noClearStreak = 0;
    } else {
        noClearStreak++;

        // Every 5 consecutive no-clear blocks → drop a star next
        if (noClearStreak % 5 == 0) {
            pendingStars++;
        }
    }
}

// ==========================================================
// Generate the next block type:
// 1. Pending star? → return '*'
// 2. Sequence mode → sequence file values
// 3. Random mode → Level 3 distribution
// ==========================================================
char Level4::generateNextBlockType() {

    // 1. Stars take priority
    if (pendingStars > 0) {
        pendingStars--;
        return '*';
    }

    // 2. If sequence mode is enabled
    if (!randomMode && !sequence.empty()) {
        char type = sequence[sequenceIndex];
        sequenceIndex = (sequenceIndex + 1) % sequence.size();
        return type;
    }

    // 3. Level 3 weighted random distribution
    int r = rand() % 9;

    if (r == 0 || r == 1) {
        return 'S';       // 2/9
    } else if (r == 2 || r == 3) {
        return 'Z';       // 2/9
    } else if (r == 4) {
        return 'I';       // 1/9
    } else if (r == 5) {
        return 'J';       // 1/9
    } else if (r == 6) {
        return 'L';       // 1/9
    } else if (r == 7) {
        return 'O';       // 1/9
    } else { // r == 8
        return 'T';       // 1/9
    }
}

// ==========================================================
// Sequence mode support
// ==========================================================
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

void Level4::setRandom(bool enabled) {
    randomMode = enabled;
}

void Level4::setSequenceFile(const std::string& file) {
    readFile(file);
}

string Level4::getSequenceFile() const {
    return filename;
}
