module SBlock;

import <utility>;
import <vector>;

SBlock::SBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // initial shape:
    //  SS
    // SS
    cells = { {0,1}, {0,2}, {1,0}, {1,1} };

    computeSpawnPosition();
}

char SBlock::getVal() const {
    return 'S';
}
