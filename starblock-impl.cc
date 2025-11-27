module StarBlock;

import StarBlock;

StarBlock::StarBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // 1x1 block for l4
    cells = { {0,0} };

    computeSpawnPosition();
    col = 5; // bc ts should spawn in the middle, override spawn position
}

char StarBlock::getVal() const {
    return '*';
}
