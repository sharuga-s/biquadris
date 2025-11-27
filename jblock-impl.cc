module JBlock;

import <utility>;
import <vector>;

JBlock::JBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // initial shape from pdf:
    //   J
    //   JJJ
    cells = { {0,0}, {1,0}, {1,1}, {1,2} };

    computeSpawnPosition();
}

char JBlock::getVal() const {
    return 'J';
}
