module Level2;

import <string>;
import <cstdlib>;

using namespace std;

// Level 2: All blocks are selected with equal probability

Level2::Level2()
    : levelNumber{2}
{}}

bool Level2::isHeavy() const {
    return false;
}

char Level2::generateNextBlockType() {
    
    // choose random number 0–6
    int r = rand() % 7;

    if (r == 0) {
        return 'I';
    } 
    else if (r == 1) {
        return 'J';
    } 
    else if (r == 2) {
        return 'L';
    } 
    else if (r == 3) {
        return 'O';
    } 
    else if (r == 4) {
        return 'S';
    } 
    else if (r == 5) {
        return 'Z';
    } 
    else {
        return 'T';
    }
}

void Level2::setRandom(bool enabled) {
    // no-op for now
}