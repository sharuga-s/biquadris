module TBlock;

import TBlock;

TBlock::TBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // shape:
    // TTT
    //  T
    cells = { {0,0}, {0,1}, {0,2}, {1,1} };

    computeSpawnPosition();
}

char TBlock::getVal() const {
    return 'T';
}
