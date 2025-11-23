module ZBlock;

import ZBlock;

ZBlock::ZBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // shape:
    // ZZ
    //  ZZ
    cells = { {0,0}, {0,1}, {1,1}, {1,2} };

    computeSpawnPosition();
}

char ZBlock::getVal() const {
    return 'Z';
}
