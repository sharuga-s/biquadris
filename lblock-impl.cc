module LBlock;

import LBlock;

LBlock::LBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // initial shape:
    //      L
    //    LLL
    cells = { {0,2}, {1,0}, {1,1}, {1,2} };

    computeSpawnPosition();
}

char LBlock::getVal() const {
    return 'L';
}
