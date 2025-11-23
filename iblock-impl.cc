module IBlock;

import IBlock;

IBlock::IBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // initial orientation: 4 wide
    cells = { {0,0}, {0,1}, {0,2}, {0,3} };

    computeSpawnPosition(); // gets abs grid posn
}

char IBlock::getVal() const {
    return 'I';
}
