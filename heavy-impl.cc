module Heavy;

import Player;
import Grid;

using namespace std;

void Heavy::apply(Player& p, Grid&) {
    if (p.getCurrentBlock()) {
        auto* block = p.getCurrentBlock();
        block->setHeavy(true);
    }
}
