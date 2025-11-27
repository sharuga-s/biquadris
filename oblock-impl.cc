module OBlock;

import <utility>;
import <vector>;

OBlock::OBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // initial shape:
    // OO
    // OO
    cells = { {0,0}, {0,1}, {1,0}, {1,1} };

    computeSpawnPosition();
}

char OBlock::getVal() const {
    return 'O';
}
