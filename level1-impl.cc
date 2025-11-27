module Level1;

import <string>;
import <cstdlib>;   // rand, srand

using namespace std;

// Level 1: The block selector will randomly choose a block with probabilities skewed such that
// S and Z blocks are selected with probability 1/12 each, and the other blocks are selected with
// probability 1/6 each.

Level1::Level1()
    : levelNumber{1} 
{}

bool Level1::isHeavy() const {
    return false;
}

void Level1::onBlockPlaced(bool clearedRows) {
    // Level 1 does nothing here
}

char Level1::generateNextBlockType() {
    // Weighted distribution:
    // 0: S     (1)
    // 1: Z     (1)
    // 2: I     (2)
    // 3: J     (2)
    // 4: L     (2)
    // 5: O     (2)
    // 6: T     (2)

    int r = rand() % 12; //NOTE: WE NEED srand(time(nullptr)) IN GAMEENGINE, it to seed rand()

    if (r == 0) {
        return 'S';
    } else if (r == 1) {
        return 'Z';
    } else if (r == 2 || r == 3) {
        return 'I';
    } else if (r == 4 || r == 5) {
        return 'J';
    } else if (r == 6 || r == 7) {
        return 'L';
    } else if (r == 8 || r == 9) {
        return 'O';
    } else { // 10 or 11
        return 'T';
    }

}

void Level1::setRandom(bool enabled) {
    // no-op for now
}
