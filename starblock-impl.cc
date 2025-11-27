module StarBlock;

import <utility>;
import <vector>;

StarBlock::StarBlock(int level) {
    levelGenerated = level;
    heavy = false;
    rotation = 0;

    // 1x1 block for l4
    cells = { {0,0} };

    row = 3;
    col = 5;
}

char StarBlock::getVal() const {
    return '*';
}
